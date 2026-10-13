# Connect an assistant to Chataigne

Open **MCP Connections** from Chataigne's panel menu. Create one client per
assistant, choose its profile and approved folders, copy its credential, and
enable the listener. The default endpoint is `http://127.0.0.1:42002/mcp`.
Choose distinct ports when running multiple instances.

Inspect reads the project and diagnostics. Control operates existing behavior,
including authored actions that invoke scripts or system commands. Edit adds
authoring; introducing executable behavior requires the separate Advanced grant.
Full Access includes all supported actions and accessible paths. Advanced
execution is not a filesystem sandbox. Grants survive reconnects, project
changes, and restarts. Rotate or revoke them in this panel. External assistants
keep their own approval rules.

## Codex

Store the copied credential in the client process's `CHATAIGNE_MCP_TOKEN`
environment variable, then run:

```text
codex mcp add chataigne --url http://127.0.0.1:42002/mcp --bearer-token-env-var CHATAIGNE_MCP_TOKEN
```

The panel also copies this TOML configuration:

```toml
[mcp_servers.chataigne]
url = "http://127.0.0.1:42002/mcp"
bearer_token_env_var = "CHATAIGNE_MCP_TOKEN"
```

Use the actual selected port. See [Codex MCP setup](https://developers.openai.com/resources/docs-mcp).

## Claude Code

Set the same client-specific environment variable and copy the panel's
configuration into the client's `.mcp.json`:

```json
{
  "mcpServers": {
    "chataigne": {
      "type": "http",
      "url": "http://127.0.0.1:42002/mcp",
      "headers": {"Authorization": "Bearer ${CHATAIGNE_MCP_TOKEN}"}
    }
  }
}
```

Approve this project server in Claude Code, then check `claude mcp list`.
[Claude's MCP documentation](https://code.claude.com/docs/en/mcp) explains
configuration scopes and environment expansion. Keep credentials out of shared
configuration and source control.

## Claude Desktop and other stdio clients

Install the platform's `chataigne-mcp.mcpb` bundle in Claude Desktop. Enter the
endpoint displayed by the running Chataigne instance and its client credential.
The bundle includes the native bridge and needs no Node.js or Python runtime.
Windows installs the bundle beside Chataigne. macOS includes it under
`Contents/Resources/mcp`; Linux AppImages include it under
`usr/share/chataigne/mcp`.

Other stdio clients can launch:

```text
chataigne-mcp-bridge --endpoint http://127.0.0.1:42002/mcp --token-env CHATAIGNE_MCP_TOKEN
```

The bridge never discovers or chooses another instance automatically. It writes
only JSON-RPC messages to stdout. It forwards both modern and legacy lifecycle
and subscription traffic, with a limit of eight active requests and 1 MiB
messages. Closing stdin cancels pending connections and stops it.

Build a bundle with `package_bridge.py BINARY OUTPUT --platform PLATFORM --arch ARCH`.
Packaging verifies the manifest, executable contents, and executable permission.
The format follows the [MCPB manifest](https://github.com/modelcontextprotocol/mcpb/blob/main/MANIFEST.md).

## Hosted ChatGPT through Secure MCP Tunnel

Follow the current [Secure MCP Tunnel guide](https://developers.openai.com/api/docs/guides/secure-mcp-tunnels).
This setup needs a Platform tunnel ID, a separate runtime API key, tunnel
permissions, and an associated ChatGPT workspace that permits custom MCP
connections. A ChatGPT subscription alone does not supply these credentials.

1. Create and associate the tunnel in Platform tunnel settings.
2. Obtain the current native `tunnel-client` and run its `help quickstart`.
3. Configure its local stdio profile to launch `chataigne-mcp-bridge` with the
   selected endpoint and Chataigne credential environment variable. This keeps
   local Host/Origin checks and credentials under the native bridge's control.
4. Run the tunnel's `doctor --explain` check, then keep its client running.
5. Add a custom MCP server in ChatGPT, choose the Tunnel connection, and select
   the associated tunnel. Use Chataigne's Inspect profile for an initial check.

The tunnel's Platform credential and the Chataigne client credential are
separate. Use the tunnel's private outbound connection; direct unauthenticated
LAN listeners and public plugin-directory publication are outside this setup.

## Troubleshooting

The panel's authenticated connection test requires a newly created or rotated
credential. Existing secrets cannot be recovered from the stored fingerprints.
HTTP 401 means the credential is missing, revoked, or incorrect. HTTP 403 means
the Host, Origin, or grant is rejected. Port binding failures identify a stopped
listener or a port already used by another instance. A stale handle or revision
requires fresh project discovery and inspection. A full queue returns HTTP 429.

Diagnose from observed warnings, logs, file/reference checks, and value
observations. Sending a test command or emitting output is a separate control
operation. Activity history contains operation names and status, never bearer
credentials or request bodies.
