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

Native integrations may use `--credential-store DIRECTORY` instead of an
environment variable. It reads the `bridge-credential` record from OrganicUI's
protected store (current-user DPAPI on Windows, private owner-only files on Unix).
Desktop JSON then contains the directory path, not the bearer. The native bridge
tests cover a real authenticated connection using this route. The developer
validation instance supports `mcp_authoring_test --desktop-connection`; its
credential has Inspect access and is listed in Global Settings > MCP > Clients.

Build a bundle with `package_bridge.py BINARY OUTPUT --platform PLATFORM --arch ARCH`.
Packaging verifies the manifest, executable contents, and executable permission.
The format follows the [MCPB manifest](https://github.com/modelcontextprotocol/mcpb/blob/main/MANIFEST.md).

## ChatGPT inside Chataigne

Use **Continue with ChatGPT** in the assistant or **Global Settings > ChatGPT
Assistant**. This uses regular browser sign-in and your ChatGPT plan. It requires
no Platform API key, hosted MCP connector, or tunnel. See [assistant setup](ASSISTANT.md).

Hosted ChatGPT and tunnel setup are outside this release's agreed scope.

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
