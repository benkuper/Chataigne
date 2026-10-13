"""Package the native bridge as a self-contained Claude Desktop MCP bundle."""
import argparse
import hashlib
import json
from pathlib import Path
import stat
import zipfile


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("binary", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--platform", choices=["win32", "darwin", "linux"], required=True)
    parser.add_argument("--arch", required=True)
    args = parser.parse_args()
    binary = args.binary.resolve(strict=True)
    executable = "server/chataigne-mcp-bridge" + (".exe" if args.platform == "win32" else "")
    manifest = {
        "manifest_version": "0.3", "name": "chataigne-mcp", "version": "1.0.0",
        "display_name": f"Chataigne MCP ({args.arch})",
        "description": "Connect Claude Desktop to an explicitly selected running Chataigne instance.",
        "author": {"name": "Ben Kuper"}, "license": "GPL-3.0",
        "repository": {"type": "git", "url": "https://github.com/benkuper/Chataigne"},
        "compatibility": {"platforms": [args.platform]}, "tools_generated": True,
        "server": {"type": "binary", "entry_point": executable, "mcp_config": {
            "command": "${__dirname}/" + executable,
            "args": ["--endpoint", "${user_config.endpoint}", "--token-env", "CHATAIGNE_MCP_TOKEN"],
            "env": {"CHATAIGNE_MCP_TOKEN": "${user_config.credential}"},
        }},
        "user_config": {
            "endpoint": {"type": "string", "title": "Running Chataigne endpoint", "required": True,
                         "default": "http://127.0.0.1:42002/mcp", "description": "Select the exact running instance shown in its MCP Connections panel."},
            "credential": {"type": "string", "title": "Chataigne client credential", "required": True,
                           "sensitive": True, "description": "Create a client in Chataigne's MCP Connections panel and copy its credential."},
        },
    }
    readme = """Chataigne MCP native bridge

Open Chataigne's MCP Connections panel, create a client with the desired access,
copy its credential, and enable the listener. Install this bundle in Claude
Desktop and enter the exact endpoint and credential. The bridge connects to that
instance only. It requires no Node.js, Python, or model account to run.

Access is enforced by Chataigne. Rotate or revoke credentials in its connection
panel. Full Access includes scripts and system commands. Advanced execution is
not a filesystem sandbox. External assistants keep their own approval rules.
"""
    args.output.parent.mkdir(parents=True, exist_ok=True)
    binary_data = binary.read_bytes()
    with zipfile.ZipFile(args.output, "w", zipfile.ZIP_DEFLATED) as bundle:
        bundle.writestr("manifest.json", json.dumps(manifest, indent=2) + "\n")
        entry = zipfile.ZipInfo(executable); entry.create_system = 3
        entry.external_attr = (stat.S_IFREG | 0o755) << 16
        bundle.writestr(entry, binary_data)
        bundle.writestr("README.txt", readme)
        bundle.writestr("LICENSE", (Path(__file__).resolve().parents[2] / "LICENSE").read_text(encoding="utf-8"))
    with zipfile.ZipFile(args.output) as bundle:
        assert set(bundle.namelist()) == {"manifest.json", executable, "README.txt", "LICENSE"}
        assert hashlib.sha256(bundle.read(executable)).digest() == hashlib.sha256(binary_data).digest()
        assert json.loads(bundle.read("manifest.json"))["server"]["entry_point"] == executable
    print(f"Created and verified {args.output}")


if __name__ == "__main__":
    main()
