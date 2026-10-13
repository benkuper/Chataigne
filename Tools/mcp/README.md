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
- Optional ChatGPT-plan assistant implemented with browser OAuth/PKCE, verified
  identity, protected per-account storage, serialized token renewal, live model
  discovery, streamed responses and local registry function calls. The
  conversation uses OrganicUI parameters/triggers and native controls; account
  and access management live in Global Settings. See [assistant setup](ASSISTANT.md).
  A real subscription-backed native provider smoke test has completed a local
  project-inspection tool call. Offline account failures and streaming are tested.

The Windows Debug application builds with MCP enabled. The native authoring
regression covers module/state/variable/sequence creation,
typed edits and conflicts, duplicate/move, grouped undo, folder restrictions on
undo, cue/key authoring, frame-aware seeking, save/reload, generation changes,
and teardown, plus offline assistant accounts and native UI bindings. Coverage
now includes transitions, processing chains, multiplexes, routing, dashboards,
presets, Parrot records, timeline clips and live playback. Run
`Tools/tests/run_mcp_authoring_test.ps1` after building the application. The
registry, assistant security/streaming, HTTP and bridge suites pass on Windows
and Linux; they are also run by platform CI.

The stage-3 CI application builds passed Windows (including Win7), both Macs,
and both Raspberry architectures. Native test tooling failed on Mac and Linux:
the core/events-only Mac example needed direct run-loop pumping, and the Linux
bridge needed the existing libcurl dependency instead of JUCE's raw HTTP
fallback. Fixes are included for the next CI run. CI builds and bundles the native
bridge and Desktop extension without changing its dependency checkout strategy.

Actual Claude Desktop and hosted ChatGPT connection checks, expanded stress and
diagnostics coverage, and final platform/package checks remain acceptance work.
See [acceptance evidence](ACCEPTANCE.md) for the current scope and outstanding checks.
