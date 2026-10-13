"""Exercise the native stdio bridge against the native independent MCP server."""
import json
import os
from pathlib import Path
import queue
import socket
import subprocess
import sys
import tempfile
import threading
import time


def main():
    example, bridge = (str(Path(path).resolve()) for path in sys.argv[1:3])
    hidden = {"creationflags": subprocess.CREATE_NO_WINDOW} if os.name == "nt" else {}
    with tempfile.TemporaryDirectory(prefix="mcp-bridge-") as directory:
        created = subprocess.run([example, "--store", directory, "--create-client"], capture_output=True, text=True, check=True, **hidden)
        token = created.stdout.strip().split()[-1]
        subprocess.run([example, "--store", directory, "--protected-client"], capture_output=True, check=True, **hidden)
        with socket.socket() as reservation:
            reservation.bind(("127.0.0.1", 0)); port = reservation.getsockname()[1]
        server = subprocess.Popen([example, "--store", directory, "--port", str(port)], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, **hidden)
        clients = []
        try:
            deadline = time.monotonic() + 10
            while True:
                try:
                    with socket.create_connection(("127.0.0.1", port), timeout=0.1): break
                except OSError:
                    if time.monotonic() > deadline: raise AssertionError("example did not start")
                    time.sleep(0.05)
            for version, protected_store in (("2026-07-28", False), ("2025-11-25", False), ("2025-06-18", False), ("2025-11-25", True)):
                environment = dict(os.environ, CHATAIGNE_MCP_TOKEN=token)
                arguments = [bridge, "--endpoint", f"http://127.0.0.1:{port}/mcp"]
                if protected_store:
                    environment.pop("CHATAIGNE_MCP_TOKEN", None)
                    arguments += ["--credential-store", directory]
                process = subprocess.Popen(arguments, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, env=environment, **hidden)
                clients.append(process)
                messages = queue.Queue()

                def read_output(stdout, target):
                    for line in stdout:
                        try: target.put(json.loads(line))
                        except json.JSONDecodeError: target.put({"invalid_stdout": line})

                reader = threading.Thread(target=read_output, args=(process.stdout, messages), daemon=True); reader.start()

                def send(method, params=None, identifier=1):
                    params = dict(params or {})
                    if version == "2026-07-28":
                        params["_meta"] = {"io.modelcontextprotocol/protocolVersion": version, "io.modelcontextprotocol/clientInfo": {"name": "bridge-test", "version": "1"}, "io.modelcontextprotocol/clientCapabilities": {}}
                    message = {"jsonrpc": "2.0", "method": method, "params": params}
                    if identifier is not None: message["id"] = identifier
                    process.stdin.write(json.dumps(message) + "\n"); process.stdin.flush()

                def response(identifier, timeout=8):
                    deadline = time.monotonic() + timeout
                    while time.monotonic() < deadline:
                        try: result = messages.get(timeout=max(0.01, deadline - time.monotonic()))
                        except queue.Empty: break
                        assert "invalid_stdout" not in result, "bridge wrote non-protocol stdout"
                        if result.get("id") == identifier: return result
                    raise AssertionError(f"bridge response timed out for {version}, id {identifier}; exit={process.poll()}")

                if version == "2026-07-28":
                    send("server/discover", identifier=1)
                    assert response(1)["result"]["resultType"] == "complete"
                else:
                    send("initialize", {"protocolVersion": version, "capabilities": {}, "clientInfo": {"name": "bridge-test", "version": "1"}}, 1)
                    assert response(1)["result"]["protocolVersion"] == version
                    send("notifications/initialized", identifier=None)
                send("tools/list", identifier=2)
                assert response(2)["result"]["tools"][0]["name"] == "organic_example_inspect"
                send("tools/call", {"name": "organic_example_inspect", "arguments": {}}, 3)
                assert response(3)["result"]["structuredContent"]["value"] == 42
                if version == "2026-07-28":
                    send("subscriptions/listen", {"notifications": {"resourceSubscriptions": ["organic://clock"]}}, 4)
                    first = messages.get(timeout=5)
                    assert first["method"] == "notifications/subscriptions/acknowledged"
                    changed = messages.get(timeout=5)
                    assert changed["method"] == "notifications/resources/updated"
                    send("notifications/cancelled", {"requestId": 4}, identifier=None)
                else:
                    send("resources/subscribe", {"uri": "organic://clock"}, 4)
                    assert "result" in response(4)
                    changed = messages.get(timeout=5)
                    assert changed["method"] == "notifications/resources/updated"
                send("ping", identifier=5)
                assert "result" in response(5)
                process.stdin.close()
                process.wait(timeout=8)
                assert process.returncode == 0, "bridge did not shut down cleanly"
                reader.join(timeout=2)
                assert token not in process.stderr.read(), "credential leaked to bridge diagnostics"
            print("Passed native stdio bridge checks for modern and both legacy MCP versions, protected Desktop credentials, subscriptions, cancellation, and EOF teardown")
        finally:
            for process in clients:
                if process.poll() is None: process.kill(); process.wait(timeout=5)
            server.terminate(); server.wait(timeout=10)


if __name__ == "__main__":
    main()
