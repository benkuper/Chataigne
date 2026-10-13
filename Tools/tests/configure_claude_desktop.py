"""Configure the native bridge without putting its bearer in Desktop's JSON."""
import argparse
import json
from pathlib import Path
import shutil

parser = argparse.ArgumentParser()
parser.add_argument("config", type=Path)
parser.add_argument("bridge", type=Path)
parser.add_argument("credential_store", type=Path)
args = parser.parse_args()
if not args.bridge.is_file() or not (args.credential_store / "bridge-credential.private").is_file():
    parser.error("Build/start the Chataigne Desktop validation instance first")
data = json.loads(args.config.read_text(encoding="utf-8-sig")) if args.config.exists() else {}
servers = data.setdefault("mcpServers", {})
if "chataigne" in servers:
    parser.error("An existing chataigne Desktop entry must be reviewed before replacement")
backup = args.config.with_suffix(".before-chataigne.json")
if backup.exists():
    parser.error("A previous Desktop backup exists; review it before configuring again")
if args.config.exists():
    shutil.copy2(args.config, backup)
servers["chataigne"] = {"command": str(args.bridge.resolve()), "args": ["--endpoint", "http://127.0.0.1:42002/mcp", "--credential-store", str(args.credential_store.resolve())]}
args.config.parent.mkdir(parents=True, exist_ok=True)
temporary = args.config.with_suffix(".pending-chataigne.json")
temporary.write_text(json.dumps(data, indent=2) + "\n", encoding="utf-8")
temporary.replace(args.config)
print("Claude Desktop configuration saved; bearer remains in protected Chataigne settings storage.")
