"""Black-box native MCP transport checks; no third-party Python packages required."""
import concurrent.futures
import http.client
import json
import os
from pathlib import Path
import socket
import subprocess
import sys
import tempfile
import time


def main():
    executable = str(Path(sys.argv[1]).resolve())
    hidden = {"creationflags": subprocess.CREATE_NO_WINDOW} if os.name == "nt" else {}
    with tempfile.TemporaryDirectory(prefix="organic-mcp-http-") as directory:
        args = [executable, "--store", directory]
        result = subprocess.run(args + ["--create-client"], capture_output=True, text=True, check=True, **hidden)
        token = result.stdout.strip().split()[-1]
        with socket.socket() as reservation:
            reservation.bind(("127.0.0.1", 0))
            port = reservation.getsockname()[1]
        process = subprocess.Popen(args + ["--port", str(port)], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, **hidden)
        try:
            deadline = time.monotonic() + 10
            while True:
                try:
                    with socket.create_connection(("127.0.0.1", port), timeout=0.1):
                        break
                except OSError:
                    if process.poll() is not None or time.monotonic() > deadline:
                        raise AssertionError("Native MCP example did not start")
                    time.sleep(0.05)

            def request(method, params=None, version="2026-07-28", session=None, override=None, http_method="POST", raw=None):
                params = dict(params or {})
                headers = {"Authorization": "Bearer " + token, "Accept": "application/json, text/event-stream", "Content-Type": "application/json"}
                if version == "2026-07-28":
                    params["_meta"] = {"io.modelcontextprotocol/protocolVersion": version, "io.modelcontextprotocol/clientInfo": {"name": "test", "version": "1"}, "io.modelcontextprotocol/clientCapabilities": {}}
                    headers["Mcp-Method"] = method
                    if method in ("tools/call", "resources/read", "prompts/get"):
                        headers["Mcp-Name"] = params.get("name", params.get("uri", ""))
                if version:
                    headers["MCP-Protocol-Version"] = version
                if session:
                    headers["Mcp-Session-Id"] = session
                headers.update(override or {})
                body = raw if raw is not None else json.dumps({"jsonrpc": "2.0", "id": 1, "method": method, "params": params})
                connection = http.client.HTTPConnection("127.0.0.1", port, timeout=5)
                connection.request(http_method, "/mcp", body if http_method == "POST" else None, headers)
                response = connection.getresponse()
                if response.getheader("Content-Type", "").startswith("text/event-stream"):
                    return connection, response
                content = response.read()
                data = json.loads(content) if content else None
                status, response_headers = response.status, dict(response.getheaders())
                connection.close()
                return status, data, response_headers

            status, data, headers = request("server/discover")
            assert status == 200 and data["result"]["supportedVersions"][0] == "2026-07-28"
            assert data["result"]["resultType"] == "complete" and "Mcp-Session-Id" not in headers
            assert request("tools/list")[1]["result"]["tools"][0]["name"] == "organic_example_inspect"
            assert request("tools/call", {"name": "organic_example_inspect", "arguments": {}})[1]["result"]["structuredContent"]["value"] == 42
            assert request("resources/templates/list")[1]["result"]["resourceTemplates"][0]["uriTemplate"] == "organic://example/{field}"
            assert json.loads(request("resources/read", {"uri": "organic://example/%76alue"})[1]["result"]["contents"][0]["text"])["value"] == 42
            assert request("tools/list", override={"Origin": "https://evil.example"})[0] == 403
            assert request("tools/list", override={"Host": "evil.example"})[0] == 403
            assert request("tools/list", override={"Authorization": "Bearer " + "0" * 64})[0] == 401
            assert request("tools/list", override={"Mcp-Method": "tools/call"})[1]["error"]["code"] == -32020
            assert request("tools/list", version="1900-01-01")[1]["error"]["code"] == -32022
            assert request("tools/list", raw="[")[1]["error"]["code"] == -32700
            assert request("tools/list", raw="[]")[1]["error"]["code"] == -32600
            assert request("tools/list", raw="[" * 5000 + "]" * 5000)[1]["error"]["code"] == -32600
            assert request("missing/method")[0] == 404
            assert request("tools/list", http_method="GET")[0] == 405
            connection, response = request("subscriptions/listen", {"notifications": {"resourceSubscriptions": ["organic://clock"]}})
            assert response.status == 200
            events = []
            while len(events) < 2:
                line = response.readline().decode()
                if line.startswith("data: "):
                    events.append(json.loads(line[6:]))
            assert events[0]["method"] == "notifications/subscriptions/acknowledged"
            assert events[1]["method"] == "notifications/resources/updated"
            response.close(); connection.close()
            for version in ("2025-11-25", "2025-06-18"):
                status, data, headers = request("initialize", {"protocolVersion": version}, version=None)
                session = headers["Mcp-Session-Id"]
                assert status == 200 and data["result"]["protocolVersion"] == version
                assert request("tools/list", version=version, session=session)[0] == 400
                assert request("notifications/initialized", version=version, session=session, raw='{"jsonrpc":"2.0","method":"notifications/initialized"}')[0] == 202
                assert request("tools/list", version=version, session=session)[0] == 200
                assert request("resources/templates/list", version=version, session=session)[1]["result"]["resourceTemplates"][0]["uriTemplate"] == "organic://example/{field}"
                assert json.loads(request("resources/read", {"uri": "organic://example/value"}, version=version, session=session)[1]["result"]["contents"][0]["text"])["value"] == 42
                assert request("resources/subscribe", {"uri": "organic://clock"}, version=version, session=session)[0] == 200
                connection, response = request("", version=version, session=session, http_method="GET")
                assert response.status == 200
                response.close(); connection.close()
                assert request("", version=version, session=session, http_method="DELETE")[0] == 204
                assert request("tools/list", version=version, session=session)[0] == 404
            with concurrent.futures.ThreadPoolExecutor(max_workers=8) as pool:
                outcomes = list(pool.map(lambda _: request("tools/list")[0], range(80)))
                assert all(status == 200 for status in outcomes)
            print("Passed native MCP HTTP checks: modern/legacy, security, malformed traffic, SSE, disconnects, concurrent clients")
        finally:
            process.terminate()
            process.wait(timeout=10)


if __name__ == "__main__":
    main()
