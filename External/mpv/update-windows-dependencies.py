"""Replace libmpv and its notices in existing Windows dependency ZIPs."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import zipfile


def digest(data):
    return hashlib.sha256(data).hexdigest()


def update(runtime, archive):
    dll = (runtime / "libmpv-2.dll").read_bytes()
    manifest = json.loads((runtime / "mpv-licenses/runtime.json").read_text())
    if digest(dll) != manifest["sha256"] or len(dll) != manifest["bytes"]:
        raise ValueError("Runtime does not match its manifest")
    archive = archive.resolve(strict=True)
    temporary = archive.with_name(archive.name + ".mpv.tmp")
    if temporary.parent.resolve() != archive.parent:
        raise ValueError("Temporary archive must stay in the dependency directory")
    previous = digest(archive.read_bytes())
    expected = {}
    with zipfile.ZipFile(archive) as source, zipfile.ZipFile(
        temporary, "w", compression=zipfile.ZIP_DEFLATED, compresslevel=9
    ) as target:
        for entry in source.infolist():
            if entry.filename == "libmpv-2.dll" or entry.filename.startswith("mpv-licenses/"):
                continue
            if entry.filename in expected:
                raise ValueError("Duplicate archive member: " + entry.filename)
            content = source.read(entry)
            expected[entry.filename] = digest(content)
            target.writestr(entry, content, compresslevel=9)
        replacements = {"libmpv-2.dll": dll}
        for notice in sorted((runtime / "mpv-licenses").rglob("*")):
            if notice.is_file():
                replacements[notice.relative_to(runtime).as_posix()] = notice.read_bytes()
        for name, content in replacements.items():
            expected[name] = digest(content)
            # Stable metadata avoids uploading an identical runtime on every push.
            entry = zipfile.ZipInfo(name, (2026, 6, 1, 0, 0, 0))
            entry.compress_type = zipfile.ZIP_DEFLATED
            entry.external_attr = 0o100644 << 16
            target.writestr(entry, content, compresslevel=9)
    with zipfile.ZipFile(temporary) as result:
        if result.testzip() is not None:
            raise ValueError("Dependency archive failed its integrity check")
        actual = {entry.filename: digest(result.read(entry)) for entry in result.infolist()}
        if actual != expected:
            raise ValueError("Dependency archive members changed unexpectedly")
    updated = digest(temporary.read_bytes())
    os.replace(temporary, archive)
    result = {"archive": archive.name, "bytes": archive.stat().st_size,
              "sha256": updated, "changed": updated != previous}
    print(json.dumps(result))
    return result


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", required=True, type=Path)
    parser.add_argument("archives", nargs="+", type=Path)
    args = parser.parse_args()
    for path in args.archives:
        update(args.runtime.resolve(strict=True), path)
