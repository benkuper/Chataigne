import contextlib
import hashlib
import importlib.util
import io
import json
import os
from pathlib import Path
import stat
import sys
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import MagicMock, patch
import zipfile

sys.dont_write_bytecode = True
spec = importlib.util.spec_from_file_location("upload", Path(__file__).with_name("upload.py"))
upload = importlib.util.module_from_spec(spec)
spec.loader.exec_module(upload)


class RemoteFile(io.BytesIO):
    def prefetch(self, **options):
        pass


class FakeSFTP:
    def __init__(self, corrupt=False, truncate=False, rename_failure=False):
        self.files = {}
        self.modes = {}
        self.corrupt = corrupt
        self.truncate = truncate
        self.rename_failure = rename_failure

    def stat(self, path):
        if path.endswith(("/user/data", "/support/pdb")):
            return SimpleNamespace(st_mode=stat.S_IFDIR | 0o755)
        if path not in self.files:
            raise FileNotFoundError(path)
        return SimpleNamespace(st_size=len(self.files[path]))

    def putfo(self, source, path, **options):
        data = source.read()
        if self.corrupt:
            data = b"!" + data[1:]
        if self.truncate:
            data = data[:-1]
        self.files[path] = data

    def open(self, path, mode):
        return RemoteFile(self.files[path])

    def chmod(self, path, mode):
        self.modes[path] = mode

    def posix_rename(self, source, destination):
        if self.rename_failure:
            raise PermissionError("test rename failure")
        self.files[destination] = self.files.pop(source)
        self.modes[destination] = self.modes.pop(source)

    def remove(self, path):
        if path not in self.files:
            raise FileNotFoundError(path)
        del self.files[path]


class UploadTest(unittest.TestCase):
    def test_package_and_pdb_destinations_and_contents(self):
        for kind, name, content, mode in [("package", "Chataigne-linux-x64-1.0.AppImage", b"compressed app", 0o644),
                                           ("pdb", "Chataigne-win-x64-1.0.pdb.zip", b"raw crash symbols", 0o600)]:
            with self.subTest(kind=kind), tempfile.TemporaryDirectory() as directory:
                path = Path(directory, name)
                if kind == "pdb":
                    with zipfile.ZipFile(path, "w", zipfile.ZIP_DEFLATED) as archive:
                        archive.writestr(name[:-4], content)
                else:
                    path.write_bytes(content)
                sftp = FakeSFTP()
                with upload.payload(path, kind) as (source, filename, size, digest):
                    relative = upload.publish(sftp, "/chataigne", source, filename, size, digest, kind)
                target = "/chataigne/" + relative
                self.assertEqual(sftp.files, {target: content})
                self.assertEqual(sftp.modes[target], mode)
                self.assertEqual(relative, ("support/pdb/" + name[:-4]) if kind == "pdb" else "user/data/" + name)

    def test_failed_transfer_preserves_existing_release_and_removes_staging(self):
        for failure in ("corrupt", "truncate", "rename_failure"):
            with self.subTest(failure=failure):
                sftp = FakeSFTP(**{failure: True})
                target = "/chataigne/user/data/Chataigne-test.pkg"
                sftp.files[target] = b"previous release"
                content = b"new release"
                with self.assertRaises((ValueError, OSError)):
                    upload.publish(sftp, "/chataigne", io.BytesIO(content), "Chataigne-test.pkg",
                                   len(content), hashlib.sha256(content).hexdigest(), "package")
                self.assertEqual(sftp.files, {target: b"previous release"})

    def test_pdb_archive_rejects_unsafe_or_mismatched_entries(self):
        for names in [["../Chataigne-test.pdb"], ["other.pdb"], ["Chataigne-test.pdb", "extra.txt"], []]:
            with self.subTest(names=names), tempfile.TemporaryDirectory() as directory:
                path = Path(directory, "Chataigne-test.pdb.zip")
                with zipfile.ZipFile(path, "w") as archive:
                    for name in names:
                        archive.writestr(name, b"symbols")
                with self.assertRaises(ValueError), upload.payload(path, "pdb"):
                    pass

    def test_invalid_file_never_connects(self):
        for name, content, kind in [("Chataigne-empty.pkg", b"", "package"), ("bad-name.pkg", b"x", "package"),
                                     ("Chataigne-test.php", b"x", "package"), ("Chataigne-test.pkg", b"x", "unknown")]:
            with self.subTest(name=name, kind=kind), tempfile.TemporaryDirectory() as directory:
                path = Path(directory, name)
                path.write_bytes(content)
                with self.assertRaises(ValueError), upload.payload(path, kind):
                    pass
        self.assertFalse(upload.valid_filename("../Chataigne-test.pkg"))
        self.assertFalse(upload.valid_filename("Chataigne-../test.pkg"))
        config = dict(host="example.invalid", username="user", password="secret", remotePath="/chataigne", hostKey="pin")
        with patch.dict(os.environ, {"UPLOAD_SFTP_CONFIG": json.dumps(config), "UPLOAD_FILE": "missing.pkg"}, clear=True), \
                patch.object(upload, "connect") as connect, contextlib.redirect_stderr(io.StringIO()):
            self.assertEqual(upload.main(), 1)
            connect.assert_not_called()

    def test_configuration_requires_explicit_root_and_pin(self):
        config = dict(host="example.invalid", username="user", password="secret", remotePath="/chataigne", hostKey="pin")
        self.assertEqual(upload.configuration(json.dumps(config))["port"], 22)
        with self.assertRaises(ValueError):
            upload.configuration("[]")
        for field, value in [("remotePath", "/"), ("remotePath", "/chataigne/../other"),
                             ("remotePath", "relative"), ("hostKey", ""), ("port", 0)]:
            with self.subTest(field=field, value=value), self.assertRaises(ValueError):
                upload.configuration(json.dumps({**config, field: value}))

    def test_connection_pins_host_key_and_does_not_use_agent(self):
        key = upload.paramiko.RSAKey.generate(1024)
        config = dict(host="example.invalid", port=2222, username="user", password="secret",
                      remotePath="/chataigne", hostKey=key.get_name() + " " + key.get_base64())
        client = MagicMock()
        with patch.object(upload.paramiko, "SSHClient", return_value=client):
            self.assertIs(upload.connect(config), client)
        client.get_host_keys.return_value.add.assert_called_once()
        self.assertEqual(client.get_host_keys.return_value.add.call_args.args[0], "[example.invalid]:2222")
        self.assertIsInstance(client.set_missing_host_key_policy.call_args.args[0], upload.paramiko.RejectPolicy)
        self.assertFalse(client.connect.call_args.kwargs["allow_agent"])
        self.assertFalse(client.connect.call_args.kwargs["look_for_keys"])
        self.assertTrue(client.connect.call_args.kwargs["compress"])

    def test_error_output_never_reveals_configuration(self):
        secret = "private-password-never-log"
        config = dict(host="example.invalid", username="user", password=secret, remotePath="/chataigne", hostKey="pin")
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory, "Chataigne-test.pkg")
            path.write_bytes(b"package")
            output = io.StringIO()
            env = {"UPLOAD_SFTP_CONFIG": json.dumps(config), "UPLOAD_FILE": str(path), "UPLOAD_KIND": "package"}
            for error in [upload.paramiko.AuthenticationException(secret), upload.paramiko.SSHException(secret)]:
                with patch.dict(os.environ, env), patch.object(upload, "connect", side_effect=error) as connect, \
                        patch.object(upload.time, "sleep"), contextlib.redirect_stdout(output), contextlib.redirect_stderr(output):
                    self.assertEqual(upload.main(), 1)
                self.assertEqual(connect.call_count, 1 if isinstance(error, upload.paramiko.AuthenticationException) else 3)
            self.assertNotIn(secret, output.getvalue())
            self.assertNotIn(config["host"], output.getvalue())


if __name__ == "__main__":
    unittest.main()
