# MCP implementation acceptance

The agreed deliverable is all four stages in `MCP PLAN.md`. This file records
evidence and open checks; a successful narrow test does not substitute for a
missing platform, capability family or real-client check.

| Requirement | Current evidence | Remaining validation |
|---|---|---|
| Optional reusable registry, provider interfaces, independent example | OrganicUI core/HTTP test suites; independent native example | Final disabled/enabled rebuild after UI changes |
| Modern and both legacy protocol versions | Core, HTTP and bridge suites: lifecycle, schemas, errors, subscriptions, malformed traffic, cancel/disconnect/EOF | Latest platform CI |
| Remembered grants, rotation, revocation, folders, Advanced, Full | Core persistence/path tests; native authoring permissions/undo tests; offline assistant grant/revocation tests | Review all new settings surfaces |
| OrganicUI object authoring and revisions | Native typed edits, duplicate, move, reorder/remove handlers, conflict and grouped undo tests | Expanded large-tree and concurrent-edit scenarios |
| Projects, modules, commands and templates | Native create/save/new/reload, registry commands/configuration/import/export | Broader command/template and import/export round trips |
| States and processing | Native action, condition, consequence, mapping input/filter/output, multiplex and transition authoring with undo | Broken-processing diagnostic scenarios |
| Time Machine | Native sequence, layers, clips, cue/key authoring, frame-aware seek and live play/pause | Media assignment, interpolation and synchronization scenarios |
| Routing, variables, presets, dashboards, Detective, Parrot | Native authoring and Detective inspection; generic typed controls for operation | Target/record/playback and dashboard binding scenarios |
| Scripts and diagnostics | Script inspect/write/reload/invoke handlers; observed warning/assets/targets/script/log diagnostics | Latest expanded script regression and missing-asset/reference scenarios |
| Bounded work and clean lifecycle | Transport concurrency/disconnect checks, generation rejection, provider cancellation and engine teardown tests | Large trees, slow subscribers, repeated loads, pending-work shutdown |
| OrganicUI settings and responsive panels | Conversation parameters/triggers under engine; account settings under Global Settings; native connection settings and controls | Inspect fresh narrow/wide renders and teardown |
| Native bridge and Desktop bundle | Native bridge suite, verified MCPB archive; CI packaging on all targets | Actual Claude Desktop extension connection |
| Actual external clients | Installed Codex: discovery + read-only direct call; installed Claude Code: authenticated health check | Claude Desktop and hosted ChatGPT |
| ChatGPT subscription assistant | Real browser login, RS256 account validation, live account catalog, completed streamed inference + local project tool call | Final real Chataigne engine smoke after UI/model integration |
| Mocked account failures | PKCE, signature/claims, nonce, audience arrays, refresh, identity mismatch, revocation, limits, cancel, persistence and history deletion | Latest final regression |
| Cross-platform build/package regression | Stage 3 Windows/Win7/Mac/RPi application builds; local Windows+Linux native suites | Stage 4 CI and final packages |

The final merge targets `master`, the repository's default branch, as confirmed
by the user. Submodule and root feature branches are committed and pushed per
stage. Final merge and branch status remain to be verified after acceptance.
