"""Add verified video runtimes and notices without changing other ZIP members."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import zipfile

ROOT = Path(__file__).resolve().parent.parent


def digest(data):
    return hashlib.sha256(data).hexdigest()


def payload(platform):
    result = {}
    if platform == "win-x64":
        sources = [("ndi/lib/win/x64", "*.dll", ""), ("omt/lib/win/x64", "*.dll", "")]
    elif platform == "win7-x64":
        # OMT NativeAOT requires Windows 10 or later.
        sources = [("ndi/lib/win/x64", "*.dll", "")]
    elif platform == "osx":
        sources = [("ndi/lib/osx", "*.dylib", "Frameworks/"), ("omt/lib/osx", "*.dylib", "Frameworks/")]
    else:
        arch = platform.removeprefix("linux-")
        sources = [(f"ndi/lib/linux/{arch}", "*.so*", "lib/")]
        if arch != "armhf":
            sources.append((f"omt/lib/linux/{arch}", "*.so*", "lib/"))
    for directory, pattern, prefix in sources:
        files = list((ROOT / "External" / directory).glob(pattern))
        if not files:
            raise FileNotFoundError(directory)
        for path in files:
            result[prefix + path.name] = path.read_bytes()
    for directory in ("ndi", "omt"):
        for path in (ROOT / "External" / directory).glob("LICENSE*"):
            result[f"video-licenses/{directory}/{path.name}"] = path.read_bytes()
    for path in (ROOT / "External/omt/lib/linux").glob("*/VMX-LICENSE.txt"):
        result["video-licenses/omt/VMX-LICENSE.txt"] = path.read_bytes()
    result["video-licenses/README.md"] = (ROOT / "External/VIDEO-RUNTIMES.md").read_bytes()
    manifest = {name: {"bytes": len(data), "sha256": digest(data)} for name, data in sorted(result.items())}
    result["video-licenses/manifest.json"] = json.dumps(manifest, indent=2).encode() + b"\n"
    return result


def update(archive, platform):
    replacement = payload(platform)
    temporary = archive.with_suffix(".video.tmp")
    expected = {}
    # Preserve every old member except a previous copy of these runtime files/notices.
    with zipfile.ZipFile(temporary, "w", zipfile.ZIP_DEFLATED, compresslevel=9) as target:
        if archive.exists():
            with zipfile.ZipFile(archive) as source:
                for entry in source.infolist():
                    if entry.filename in replacement or entry.filename.startswith("video-licenses/"):
                        continue
                    if entry.filename in expected:
                        raise ValueError("Duplicate ZIP member: " + entry.filename)
                    content = source.read(entry)
                    expected[entry.filename] = digest(content)
                    target.writestr(entry, content)
        for name, content in replacement.items():
            expected[name] = digest(content)
            entry = zipfile.ZipInfo(name, (2026, 10, 9, 0, 0, 0))
            entry.compress_type = zipfile.ZIP_DEFLATED
            entry.external_attr = 0o100644 << 16
            target.writestr(entry, content)
    with zipfile.ZipFile(temporary) as result:
        assert result.testzip() is None, "ZIP integrity failure"
        assert {i.filename: digest(result.read(i)) for i in result.infolist()} == expected
    os.replace(temporary, archive)
    return {"archive": archive.name, "bytes": archive.stat().st_size, "sha256": digest(archive.read_bytes())}


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("directory", type=Path)
    args = parser.parse_args()
    plans = [(f"Chataigne-{platform}-{config}-dependencies.zip", platform)
             for platform in ("win-x64", "win7-x64") for config in ("debug", "release")]
    plans += [("Chataigne-linux-dependencies.zip", "linux-x64"),
              ("Chataigne-linux-aarch64-dependencies.zip", "linux-arm64"),
              ("Chataigne-linux-armhf-dependencies.zip", "linux-armhf"),
              ("Chataigne-osx-dependencies.zip", "osx")]
    # Check all input runtimes before touching the existing archives.
    for _, platform in plans:
        payload(platform)
    manifest = [update(args.directory / name, platform) for name, platform in plans]
    (args.directory / "video-dependencies-manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print(json.dumps(manifest, indent=2))
