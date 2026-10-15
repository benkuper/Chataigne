import contextlib
import importlib.util
import io
import json
import hashlib
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
import zipfile
from unittest.mock import patch

sys.dont_write_bytecode = True
spec = importlib.util.spec_from_file_location("upload", Path(__file__).with_name("upload.py"))
upload = importlib.util.module_from_spec(spec)
spec.loader.exec_module(upload)


class UploadTest(unittest.TestCase):
    def test_pdb_zip_checksum_and_contents(self):
        with tempfile.TemporaryDirectory() as directory:
            archive = Path(directory, "symbols.pdb.zip")
            with zipfile.ZipFile(archive, "w", zipfile.ZIP_DEFLATED) as stream:
                stream.writestr("symbols.pdb", b"uncompressed symbols")
            self.assertEqual(upload.pdb_archive_checksum(archive), hashlib.sha256(b"uncompressed symbols").hexdigest())
            with zipfile.ZipFile(archive, "w") as stream:
                stream.writestr("../symbols.pdb", b"bad path")
            with self.assertRaises(ValueError):
                upload.pdb_archive_checksum(archive)
            with zipfile.ZipFile(archive, "w") as stream:
                stream.writestr("first.pdb", b"one")
                stream.writestr("second.pdb", b"two")
            with self.assertRaises(ValueError):
                upload.pdb_archive_checksum(archive)

    def test_structured_server_diagnostics(self):
        secret = "private-test-password"
        body = json.dumps({"schema": "chataigne-upload-v1", "ok": False,
            "code": "post_too_large", "message": "POST too large " + secret,
            "request_id": "pdb-test", "details": {"post_max_size": "128M", "pass": secret,
                "write_errors": ["Disk quota exceeded " + secret]}}).encode()
        output = io.StringIO()
        with contextlib.redirect_stderr(output):
            self.assertTrue(upload.print_server_diagnostic(body, secret))
        self.assertIn("post_too_large", output.getvalue())
        self.assertIn("128M", output.getvalue())
        self.assertIn("Disk quota exceeded", output.getvalue())
        self.assertNotIn(secret, output.getvalue())
        self.assertNotIn('"pass"', output.getvalue())

    def test_upload_responses(self):
        secret = "private-test-password"
        cases = [
            (0, b"File is valid, and was successfully uploaded.\n", 0),
            (0, f"Pass is invalid or error retrieving data : {secret}".encode(), 1),
            (0, b"Possible file upload attack!", 1),
            (0, b"<html>Proxy error</html>", 1),
            (22, b"HTTP failure", 1),
        ]
        with tempfile.TemporaryDirectory() as directory:
            package = Path(directory, "package.pdb")
            package.write_bytes(b"test symbols")
            env = {"UPLOAD_FILE": str(package), "UPLOAD_URL": "https://example.invalid/upload", "UPLOAD_PASSWORD": secret}
            for returncode, response, expected in cases:
                with self.subTest(returncode=returncode, expected=expected):
                    output = io.StringIO()
                    result = subprocess.CompletedProcess([], returncode, response, b"")
                    with patch.dict(os.environ, env), patch.object(upload.subprocess, "run", return_value=result), contextlib.redirect_stdout(output), contextlib.redirect_stderr(output):
                        self.assertEqual(upload.main(), expected)
                    self.assertNotIn(secret, output.getvalue())

    def test_missing_file_does_not_upload(self):
        env = {"UPLOAD_FILE": "missing-file.pdb", "UPLOAD_URL": "https://example.invalid", "UPLOAD_PASSWORD": "test"}
        with patch.dict(os.environ, env), patch.object(upload.subprocess, "run") as run, contextlib.redirect_stderr(io.StringIO()):
            self.assertEqual(upload.main(), 1)
            run.assert_not_called()


if __name__ == "__main__":
    unittest.main()
