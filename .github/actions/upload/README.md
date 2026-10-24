# Release uploads

This action publishes packages to `user/data` and matching crash symbols to
`support/pdb` beneath the configured project root. SFTP avoids the hosting
HTTP request-size limit. Files are staged beside their destination, read back
to verify SHA-256, and atomically renamed after verification succeeds.

Set the repository Actions secret `RELEASE_SFTP_CONFIG` to a JSON object with
`host`, `port`, `username`, `password`, `remotePath`, and `hostKey`. Obtain the
host key from an existing verified connection; `hostKey` contains the OpenSSH
key type and base64 public key. Unknown or changed host keys are rejected.
Keep credentials out of this directory and all tracked files.

The action accepts `kind: package` (default) or `kind: pdb`. Symbol uploads
accept a ZIP containing exactly its matching PDB at the archive root. The
PDB is streamed to its existing uncompressed server location over a connection
that requests SSH compression. No PHP upload handler or server setting changes
are needed. Transient SFTP failures are retried twice; failed validation leaves
any existing release file intact.

Run `python -m unittest discover -s .github/actions/upload -p test_upload.py`
after installing `requirements.txt` to validate destination selection, archive
validation, checksum failures, atomic replacement, host-key pinning, and
credential-safe error handling.
