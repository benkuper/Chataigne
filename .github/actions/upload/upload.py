"""Upload packages without treating HTTP or PHP-level failures as success."""

import os
import json
import hashlib
from pathlib import Path
import subprocess
import sys
import zipfile


def pdb_archive_checksum(path):
    with zipfile.ZipFile(path) as archive:
        entries = archive.infolist()
        if (len(entries) != 1 or not entries[0].filename.lower().endswith(".pdb")
                or "/" in entries[0].filename or "\\" in entries[0].filename):
            raise ValueError("The PDB ZIP must contain exactly one .pdb file at its root.")
        digest = hashlib.sha256()
        with archive.open(entries[0]) as stream:
            for chunk in iter(lambda: stream.read(1024 * 1024), b""):
                digest.update(chunk)
        return digest.hexdigest()


def print_server_diagnostic(body, password):
    # The new PHP handler emits one JSON record. A retried request can leave
    # several records in curl's stdout, so use the last recognized response.
    for line in reversed(body.splitlines()):
        try:
            response = json.loads(line)
        except (ValueError, UnicodeError):
            continue
        if not isinstance(response, dict) or response.get("schema") != "chataigne-upload-v1" or response.get("ok") is not False:
            continue
        redact = lambda text: str(text).replace(password, "<redacted>")[:2000]
        print(f"Upload failed [{redact(response.get('code', 'unknown'))}]: {redact(response.get('message', 'Server rejected the upload.'))}", file=sys.stderr)
        details = response.get("details", {})
        keys = ("content_length_bytes", "post_max_size", "upload_max_filesize",
                "post_fields_received", "files_received", "upload_error", "file_bytes",
                "compressed_bytes", "zip_error", "write_errors",
                "temporary_storage", "destination_storage")
        if isinstance(details, dict):
            safe_details = {key: details[key] for key in keys if key in details}
            print(redact(json.dumps(safe_details, sort_keys=True)), file=sys.stderr)
        print(f"Server request ID: {redact(response.get('request_id', 'unavailable'))}", file=sys.stderr)
        return True
    return False


def main():
    path = Path(os.environ["UPLOAD_FILE"])
    url = os.environ["UPLOAD_URL"]
    password = os.environ["UPLOAD_PASSWORD"]
    if not path.is_file() or path.stat().st_size == 0:
        print("Upload failed: the package is missing or empty.", file=sys.stderr)
        return 1
    if not url or not password:
        print("Upload failed: URL and password are required.", file=sys.stderr)
        return 1

    checksum = None
    if path.name.lower().endswith(".pdb.zip"):
        try:
            checksum = pdb_archive_checksum(path)
        except (OSError, ValueError, zipfile.BadZipFile, RuntimeError):
            print("Upload failed: the PDB ZIP is invalid, unreadable, or does not contain one root-level PDB.", file=sys.stderr)
            return 1
    arguments = ["curl", "--fail-with-body", "--silent", "--show-error", "--retry", "3",
                 "--retry-delay", "2", "--form", f"appFile=@{path}",
                 "--form-string", f"pass={password}"]
    if checksum is not None:
        arguments += ["--form-string", f"sha256={checksum}"]
    result = subprocess.run(
        arguments + [url],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE,
    )
    # Legacy endpoints may echo the password. Print only recognized structured
    # diagnostics, with the supplied credential redacted, never raw responses.
    if print_server_diagnostic(result.stdout, password):
        return 1
    if result.returncode != 0:
        print(f"Upload failed: curl exited with code {result.returncode}.", file=sys.stderr)
        return 1
    if b"File is valid, and was successfully uploaded." not in result.stdout:
        print("Upload failed: the server did not acknowledge the file. Check the endpoint's upload secret and file limits.", file=sys.stderr)
        return 1
    print(f"Uploaded {path.name} ({path.stat().st_size} bytes); server confirmed success.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
