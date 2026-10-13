"""Opt-in installed-client checks, with no model turns.

Use EXAMPLE_BINARY for an isolated foundation server, or:
--running ENDPOINT BRIDGE_BINARY PROTECTED_CREDENTIAL_DIRECTORY
to check the real Chataigne engine without exposing its bearer in configuration.
"""
import json
import os
from pathlib import Path
import queue
import shutil
import socket
import subprocess
import sys
import tempfile
import threading
import time


def main():
    running = sys.argv[1] == "--running"
    example = None if running else str(Path(sys.argv[1]).resolve())
    codex, claude = shutil.which("codex"), shutil.which("claude")
    if not codex or not claude:
        raise SystemExit("Install Codex and Claude Code to run actual client interoperability checks.")
    hidden = {"creationflags": subprocess.CREATE_NO_WINDOW} if os.name == "nt" else {}
    with tempfile.TemporaryDirectory(prefix="mcp-client-interop-") as directory:
        root = Path(directory)
        server, token = None, None
        if running:
            endpoint, bridge, credential_store = sys.argv[2:5]
            bridge_arguments = ["--endpoint", endpoint, "--credential-store", str(Path(credential_store).resolve())]
            port = int(endpoint.split(":")[2].split("/")[0])
        else:
            created = subprocess.run([example, "--store", str(root / "server"), "--create-client"], capture_output=True, text=True, check=True, **hidden)
            token = created.stdout.strip().split()[-1]
            with socket.socket() as reservation:
                reservation.bind(("127.0.0.1", 0)); port = reservation.getsockname()[1]
            endpoint = f"http://127.0.0.1:{port}/mcp"
            server = subprocess.Popen([example, "--store", str(root / "server"), "--port", str(port)], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, **hidden)
        client = None
        try:
            deadline = time.monotonic() + 10
            while True:
                try:
                    with socket.create_connection(("127.0.0.1", port), timeout=0.1): break
                except OSError:
                    if time.monotonic() > deadline: raise AssertionError("example did not start")
                    time.sleep(0.05)
            environment = dict(os.environ, CLAUDE_CONFIG_DIR=str(root / "claude-config"))
            if token:
                environment["CHATAIGNE_MCP_TOKEN"] = token
            if running:
                configuration = ["--transport", "stdio", "chataigne-regression", "--", bridge, *bridge_arguments]
            else:
                configuration = ["--transport", "http", "chataigne-regression", endpoint, "--header", "Authorization: Bearer ${CHATAIGNE_MCP_TOKEN}"]
            added = subprocess.run([claude, "mcp", "add", "--scope", "user", *configuration], cwd=root, env=environment, capture_output=True, text=True, timeout=30, **hidden)
            assert added.returncode == 0, "Claude Code could not load the test configuration"
            checked = subprocess.run([claude, "mcp", "list"], cwd=root, env=environment, capture_output=True, text=True, timeout=30, **hidden)
            assert checked.returncode == 0 and "Connected" in checked.stdout, "Claude Code did not authenticate and discover the MCP server"
            print("Claude Code: actual authenticated health check passed")
            if running:
                override = 'mcp_servers={chataigne_regression={command=' + json.dumps(bridge) + ',args=' + json.dumps(bridge_arguments) + '}}'
            else:
                override = 'mcp_servers={chataigne_regression={url=' + json.dumps(endpoint) + ',bearer_token_env_var="CHATAIGNE_MCP_TOKEN"}}'
            client = subprocess.Popen([codex, "-c", override, "-c", "plugins={}", "app-server", "--listen", "stdio://"], cwd=root, env=environment,
                                      stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL, text=True, **hidden)
            messages = queue.Queue()

            def reader():
                for line in client.stdout:
                    try: messages.put(json.loads(line))
                    except json.JSONDecodeError: pass

            threading.Thread(target=reader, daemon=True).start()

            def rpc(method, params, identifier):
                client.stdin.write(json.dumps({"id": identifier, "method": method, "params": params}) + "\n"); client.stdin.flush()
                deadline = time.monotonic() + 40
                while time.monotonic() < deadline:
                    response = messages.get(timeout=max(0.1, deadline - time.monotonic()))
                    if response.get("id") == identifier:
                        if "error" in response: raise AssertionError(f"Codex {method} failed: {response['error'].get('message', 'RPC error')}")
                        return response["result"]
                raise AssertionError("Codex RPC timed out")

            rpc("initialize", {"clientInfo": {"name": "chataigne_interop", "version": "1"}, "capabilities": {"experimentalApi": True}}, 1)
            client.stdin.write('{"method":"initialized"}\n'); client.stdin.flush()
            status = rpc("mcpServerStatus/list", {"serverName": "chataigne_regression", "detail": "full"}, 2)
            rows = status.get("data", [])
            assert len(rows) == 1 and rows[0].get("tools"), "Codex did not discover MCP tools"
            thread = rpc("thread/start", {"cwd": directory, "ephemeral": True, "sandbox": "read-only", "approvalPolicy": "never"}, 3)
            thread_id = thread["thread"]["id"]
            tool = "chataigne_project" if running else "organic_example_inspect"
            result = rpc("mcpServer/tool/call", {"threadId": thread_id, "server": "chataigne_regression", "tool": tool, "arguments": {}}, 4)
            assert ("specialModules" if running else "42") in json.dumps(result), "Codex did not execute the read-only MCP tool"
            print("Codex: actual discovery and direct read-only MCP call passed; no model turn started")
            client.stdin.close(); client.wait(timeout=15)
        finally:
            if client and client.poll() is None: client.terminate(); client.wait(timeout=10)
            if server:
                server.terminate(); server.wait(timeout=10)


if __name__ == "__main__":
    main()
