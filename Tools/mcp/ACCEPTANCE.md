# MCP implementation acceptance

The agreed deliverable is all four stages in `MCP PLAN.md`. This file records
evidence and open checks; a successful narrow test does not substitute for a
missing platform, capability family or real-client check.

| Requirement | Current evidence | Remaining validation |
|---|---|---|
| Optional reusable registry, provider interfaces, independent example | OrganicUI core/HTTP test suites; independent native example; final framework compilation with both optional features disabled and enabled | Complete |
| Modern and both legacy protocol versions | Core, HTTP and bridge suites: lifecycle, schemas, errors, subscriptions, malformed traffic, cancel/disconnect/EOF | Latest platform CI |
| Remembered grants, rotation, revocation, folders, Advanced, Full | Core persistence/path tests; native authoring permissions/undo tests; offline assistant grant/revocation tests; native settings controls contain no bearer/token values | Complete (163 native app checks) |
| OrganicUI object authoring and revisions | Native typed edits, duplicate, move, user control creation/removal, grouped undo; 5,000-control paginated tree and intervening native UI edits | Complete (163 native app checks) |
| Projects, modules, commands and templates | Native create/save/new/reload; built-in/template command discovery and assignment; native .lilnut export/import round trip and undo | Complete (163 native app checks) |
| States and processing | Native action, condition, consequence, mapping input/filter/output, multiplex and transition authoring with undo; observed remap input/output/dependency diagnostics without emitting output | Complete (163 native app checks) |
| Time Machine | Native sequence/layer/clip/cue/key authoring; live playback, frame-aware seek, real WAV media loading, interpolation and synchronization parameters; unassigned audio diagnostics without a modal prerequisite | Complete (163 native app checks) |
| Routing, variables, presets, dashboards, Detective, Parrot | Native family authoring and Detective inspection; dashboard target binding; Parrot target creation, recording and native playback through registry controls | Complete (163 native app checks) |
| Scripts and diagnostics | Script inspect/write/reload/invoke; broken script, missing asset, unresolved reference and real refused WebSocket connection evidence | Complete (163 native app checks) |
| Bounded work and clean lifecycle | Native lifecycle suite proves 64-job bound, queued-generation rejection, real unread large SSE backpressure, concurrent service and pending-work shutdown; native app proves 5,000 controls and eight project reloads | Complete on Windows and Linux; final platform CI |
| OrganicUI settings and responsive panels | Conversation parameters/triggers under engine; account settings under Global Settings; native connection settings and controls; fresh assistant 320/660px and connection 320/720/1100px renders | Complete: fresh narrow/wide renders inspected |
| Native bridge and Desktop bundle | Native bridge suite, verified MCPB archive; CI packaging on all targets | Actual Claude Desktop extension connection |
| Actual external clients | Installed Codex: discovery + direct `chataigne_project` call against the real engine; installed Claude Code: authenticated real-engine health check; protected bridge credential, no model turn | Claude Desktop; hosted ChatGPT/tunnels excluded by the user |
| ChatGPT subscription assistant | Real browser login, RS256 account validation, live account catalog, completed streamed inference + local project tool call; real Chataigne engine smoke passed | Complete |
| Mocked account failures | PKCE, signature/claims, nonce, audience arrays, refresh, identity mismatch, revocation, limits, cancel, persistence and history deletion | Complete (163 native app checks) |
| Cross-platform build/package regression | Stage 4 CI `38410197116` passed Windows x64/Win7, macOS Intel/ARM, Linux and Raspberry 32/64 packages; final local Windows+Linux native suites pass | Follow-up CI and final packages |

The final merge targets `master`, the repository's default branch, as confirmed
by the user. Submodule and root feature branches are committed and pushed per
stage. Final merge and branch status remain to be verified after acceptance.
