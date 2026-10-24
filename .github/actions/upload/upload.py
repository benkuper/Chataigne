"""Publish verified packages and crash symbols over authenticated SFTP."""

from contextlib import contextmanager
import errno
import hashlib
import json
import os
from pathlib import Path
import posixpath
import re
import stat
import sys
import time
import uuid
import zipfile

import paramiko


def checksum(stream):
    digest = hashlib.sha256()
    for chunk in iter(lambda: stream.read(1024 * 1024), b""):
        digest.update(chunk)
    return digest.hexdigest()


def valid_filename(name):
    return re.fullmatch(r"Chataigne-[A-Za-z0-9_.-]+", name) is not None and ".." not in name


@contextmanager
def payload(path, kind):
    if not path.is_file() or path.stat().st_size <= 0 or not valid_filename(path.name):
        raise ValueError("A nonempty Chataigne release file is required")
    if kind == "pdb":
        if not path.name.endswith(".pdb.zip"):
            raise ValueError("Symbols must be a PDB ZIP")
        with zipfile.ZipFile(path) as archive:
            entries = archive.infolist()
            if (len(entries) != 1 or entries[0].filename != path.name[:-4]
                    or entries[0].file_size <= 0 or entries[0].file_size > 512 * 1024 * 1024
                    or entries[0].flag_bits & 1):
                raise ValueError("The ZIP must contain its matching PDB at the root")
            entry = entries[0]
            with archive.open(entry) as source:
                digest = checksum(source)
            with archive.open(entry) as source:
                # Preserve the crash server's raw-PDB layout; SSH compression
                # avoids needing the HTTP ZIP extraction handler during upload.
                yield source, entry.filename, entry.file_size, digest
    elif kind == "package" and path.suffix.lower() in (".exe", ".pkg", ".appimage"):
        with path.open("rb") as source:
            digest = checksum(source)
            source.seek(0)
            yield source, path.name, path.stat().st_size, digest
    else:
        raise ValueError("Unsupported upload kind or package extension")


def configuration(raw):
    config = json.loads(raw)
    if not isinstance(config, dict):
        raise ValueError("SFTP configuration must be an object")
    for key in ("host", "username", "password", "remotePath", "hostKey"):
        if not isinstance(config.get(key), str) or not config[key].strip():
            raise ValueError("Incomplete SFTP configuration")
    config["port"] = int(config.get("port", 22))
    if not 1 <= config["port"] <= 65535:
        raise ValueError("Invalid SFTP port")
    root = config["remotePath"]
    if not root.startswith("/") or root == "/" or posixpath.normpath(root) != root:
        raise ValueError("An explicit normalized project directory is required")
    return config


def connect(config):
    client = paramiko.SSHClient()
    host = config["host"] if config["port"] == 22 else f'[{config["host"]}]:{config["port"]}'
    entry = paramiko.hostkeys.HostKeyEntry.from_line(host + " " + config["hostKey"])
    if entry is None or entry.key is None:
        raise ValueError("A pinned SSH host key is required")
    client.get_host_keys().add(host, entry.key.get_name(), entry.key)
    client.set_missing_host_key_policy(paramiko.RejectPolicy())
    try:
        client.connect(config["host"], port=config["port"], username=config["username"],
                       password=config["password"], look_for_keys=False, allow_agent=False,
                       timeout=30, banner_timeout=30, auth_timeout=30, compress=True)
    except Exception:
        client.close()
        raise
    return client


def publish(sftp, root, source, filename, size, digest, kind):
    relative = "support/pdb" if kind == "pdb" else "user/data"
    directory = posixpath.join(root, relative)
    if not valid_filename(filename) or not stat.S_ISDIR(sftp.stat(directory).st_mode):
        raise ValueError("Invalid upload destination")
    destination = posixpath.join(directory, filename)
    temporary = posixpath.join(directory, f".{filename}.{uuid.uuid4().hex}.upload")
    try:
        sftp.putfo(source, temporary, file_size=size, confirm=True)
        if sftp.stat(temporary).st_size != size:
            raise ValueError("Incomplete SFTP upload")
        with sftp.open(temporary, "rb") as uploaded:
            uploaded.prefetch(file_size=size, max_concurrent_requests=16)
            if checksum(uploaded) != digest:
                raise ValueError("Uploaded file checksum mismatch")
        sftp.chmod(temporary, 0o600 if kind == "pdb" else 0o644)
        # A failed transfer or verification never truncates an existing release.
        sftp.posix_rename(temporary, destination)
        if sftp.stat(destination).st_size != size:
            raise ValueError("Published file size mismatch")
    finally:
        try:
            sftp.remove(temporary)
        except OSError:
            pass
    return relative + "/" + filename


def main():
    try:
        config = configuration(os.environ["UPLOAD_SFTP_CONFIG"])
        path = Path(os.environ["UPLOAD_FILE"])
        kind = os.environ.get("UPLOAD_KIND", "package")
        with payload(path, kind) as (_, filename, size, digest):
            pass
    except (KeyError, ValueError, OSError, zipfile.BadZipFile, RuntimeError):
        print("Upload failed: invalid configuration, file, or PDB archive.", file=sys.stderr)
        return 1

    for attempt in range(3):
        client = None
        try:
            client = connect(config)
            with client.open_sftp() as sftp:
                sftp.get_channel().settimeout(90)
                with payload(path, kind) as (source, current_name, current_size, current_digest):
                    if (current_name, current_size, current_digest) != (filename, size, digest):
                        raise ValueError("Local upload changed during publication")
                    relative = publish(sftp, config["remotePath"], source, filename, size, digest, kind)
            print(f"Uploaded and SHA-256 verified: {relative} ({size} bytes; {digest})")
            return 0
        except (paramiko.AuthenticationException, paramiko.BadHostKeyException, ValueError,
                zipfile.BadZipFile, RuntimeError):
            print("Upload failed: authentication, host-key, archive, or integrity validation failed.", file=sys.stderr)
            return 1
        except (OSError, paramiko.SSHException) as error:
            # Exceptions can contain private connection data. Never print them.
            if getattr(error, "errno", None) in (errno.EACCES, errno.ENOENT) or attempt == 2:
                print("Upload failed: SFTP connection or destination unavailable.", file=sys.stderr)
                return 1
            print(f"SFTP transfer interrupted; retry {attempt + 1}/2.", file=sys.stderr)
            time.sleep(2 * (attempt + 1))
        finally:
            if client is not None:
                client.close()
    return 1


if __name__ == "__main__":
    sys.exit(main())
