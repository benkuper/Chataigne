# Native MCP development tools

The optional OrganicUI MCP foundation is C++17. It uses the existing SimpleWeb
HTTP implementation; the server and native example need no JavaScript or Python
runtime. Other OrganicUI applications retain `ORGANICUI_USE_MCP=0` by default.
Enabling it requires `juce_simpleweb`, `juce_cryptography`, and standalone ASIO.

## Build and check

Use an existing shared JUCE checkout, never a checkout inside Chataigne:

```powershell
cmake -S Tools/mcp -B Binaries/mcp-build -G "Visual Studio 18 2026" -A x64 -DJUCE_PATH=D:/Projects/Dev/JUCE
cmake --build Binaries/mcp-build --config Debug --parallel 4
ctest --test-dir Binaries/mcp-build -C Debug --output-on-failure
```

On other platforms choose an installed generator and set `JUCE_PATH` explicitly.
Python 3 is optional for the black-box HTTP tests. Core tests and the server are
native executables.

## Independent example

Run `organic_mcp_example --create-client` once and retain the bearer credential
in the client's secret store. Then run `organic_mcp_example`; it listens only at
`http://127.0.0.1:42003/mcp`. `--port` and `--store` select another port or grant
directory. The example exposes an inspect operation, identity, and an observed
clock resource. It does not load Chataigne or its project model.

The production default port is 42002. Listeners are opt-in. Host and Origin must
match the selected loopback endpoint. Credential files contain SHA-256
fingerprints, not bearer secrets. Profiles and folder grants are persistent;
rotation and revocation take effect on subsequent requests and existing streams.
Advanced grants permit executable behavior; they do not sandbox code.

## Extension contract

Providers register `Operation`, `Resource`, and `Prompt` entries with `Registry`.
Operations declare schemas, capabilities, reversibility, external effects, a
prevalidation callback, and a handler. Both local assistant calls and external
MCP calls use this registry. Provider code must check its execution context
during cancellable work, require each file path explicitly, and reject stale
object handles. Supported schema validation keywords are documented by
`checkSchema`; unsupported keywords fail registration instead of being ignored.
Templated managers expose factories and authoring through `ManagerAdapter`.

Network callbacks enqueue immutable request envelopes. Model handlers execute
on the JUCE message thread. The server bounds payloads to 1 MiB, nesting to 32,
queued requests to 64, legacy sessions to 128, and observation streams to 32.
Each stream has at most one outstanding write; stalled streams expire after
five seconds. Resource changes are sampled at 5 Hz and heartbeat comments detect
disconnected clients. `invalidateDocument()` rejects stale queued work and
subscriptions. Stop the server before destroying providers or the engine.

Modern MCP 2026-07-28 uses per-request metadata, mirrored HTTP headers,
`server/discover`, and POST `subscriptions/listen` streams. The compatibility
paths for 2025-11-25 and 2025-06-18 use credential-bound initialize sessions,
initialized notifications, GET streams, and DELETE session teardown. This
separation follows the [MCP transport specification](https://modelcontextprotocol.io/specification/2026-07-28/basic/transports/streamable-http).

## Delivery status

- Foundation: registry, grants, dedicated transport, resource observation,
  manager interface, independent example, and native/HTTP checks implemented.
- Chataigne and timeline providers: scoped object and factory discovery, revision
  checks, typed edits, ranges/enums, create/duplicate/move/reorder/remove, batched
  edits, named undo/redo, project files, commands, scripts, diagnostics, Detective,
  per-client observations, playback, cues, and keys implemented. Generic manager
  operations cover states, processing, routing, custom variables, dashboards,
  presets, Parrot, timeline layers, and clips through their existing model APIs.
  Arbitrary serialized configuration and selection import require Full Access;
  restricted profiles use typed operations with their capability and path checks.
- Dockable connection UI, native stdio bridge, MCPB packaging, and client setup
  guide implemented. The UI regression creates, tests, rotates, and revokes a
  client. The native bridge suite covers all three protocol versions,
  subscriptions, cancellation, and EOF teardown. Installed Claude Code passes an
  authenticated health check; installed Codex passes discovery and a direct
  read-only MCP call through app-server, with no model turn. See [client setup](CLIENTS.md).
- Optional assistant, ChatGPT account integration and final validation: pending.

The Windows Debug application builds with MCP enabled. The native authoring
regression covers 48 checks, including module/state/variable/sequence creation,
typed edits and conflicts, duplicate/move, grouped undo, folder restrictions on
undo, cue/key authoring, frame-aware seeking, save/reload, generation changes,
and teardown. Run `Tools/tests/run_mcp_authoring_test.ps1` after building the
application. The core and HTTP suites also pass.

The stage-2 CI build passed Windows (including Win7), Linux, and both Raspberry
architectures. Mac compilation failed; filesystem compatibility was revised for
the existing macOS deployment target and raw Xcode diagnostics are enabled for
the next run. CI now also builds and bundles the native bridge and Desktop
extension without changing its dependency checkout strategy.

Every subtype's authoring coverage, actual Claude Desktop and hosted ChatGPT
connections, account-backed inference, and final platform/package checks remain
final acceptance work. The optional assistant is not yet implemented.
