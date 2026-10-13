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
Resources include application/project summaries, capabilities, diagnostics,
observation history and `organic://usage`. Inspect/browse results include
`resourceUri` for current object details. `resources/templates/list` advertises
`organic://object/{handle}`; reads retain the tool's redaction and generation
checks. The reusable template interface supports RFC 6570 simple expansion with
one final path-segment variable, rather than arbitrary template expressions.

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
  User-added native controls (including Parrot targets) use the same factory,
  revision checks and undo history. Project summaries expose built-in app command
  modules. Native selection export produces importable `.lilnut` files.
  Arbitrary serialized configuration and selection import require Full Access;
  restricted profiles use typed operations with their capability and path checks.
- Dockable connection UI, native stdio bridge, MCPB packaging, and client setup
  guide implemented. The UI regression creates, tests, rotates, and revokes a
  client. The native bridge suite covers all three protocol versions,
  subscriptions, cancellation, and EOF teardown. Installed Claude Code passes an
  authenticated health check; installed Codex passes discovery and a direct
  read-only MCP call through app-server, with no model turn. Both also pass
  against the real Chataigne engine using its protected native bridge credential.
  See [client setup](CLIENTS.md).
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
now includes transitions, processing chains, multiplexes, routing, dashboard
bindings, presets, Parrot recording/playback, media loading, interpolation,
synchronization, command templates, native selection round trips, broken assets
and targets, refused connections, 5,000 controls and repeated loads. Run
`Tools/tests/run_mcp_authoring_test.ps1` after building the application. The
registry, assistant security/streaming, HTTP and bridge suites pass on Windows
and Linux; they are also run by platform CI.

The stage-4 CI passed all seven platforms: Windows (including Win7), both Macs,
Linux and both Raspberry architectures, including native bridge/Desktop bundle
packaging. Local Windows/Linux lifecycle tests additionally exercise queue
overflow, stale queued work, an unread large SSE stream and pending shutdown.
Streaming teardown cancels outstanding socket writes and releases queued response
owners before destroying their listener. CI preserves its existing dependency
checkout strategy.

Claude Desktop also passes real-engine discovery: its enabled connector exposes
native prompts and application resources through the protected native bridge.
The final seven-platform build/package run passed. See
[acceptance evidence](ACCEPTANCE.md) for the checked scope and source revision.
Hosted ChatGPT and tunnel setup are excluded by the user's scope clarification;
the embedded assistant uses regular ChatGPT sign-in and needs no Platform key.
