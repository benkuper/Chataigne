# Reusable OrganicUI MCP server and Chataigne assistant

## Summary and architecture

Deliver full project authoring, live control, and diagnostics through MCP, plus an optional assistant inside Chataigne. Implement in stages, with all stages included in the agreed deliverable.

- Add an optional C++ MCP foundation to `juce_organicui`, enabled in Chataigne and disabled by default for other apps.
- Put reusable sequence and timeline capabilities in `juce_timeline`; keep Chataigne-specific capabilities in Chataigne.
- Use one operation registry for external MCP clients and the embedded assistant. Permissions, validation, execution, undo, and diagnostics follow the same path.
- Reuse the existing SimpleWeb HTTP infrastructure with a dedicated listener. Chataigne’s MCP server requires no Node.js, Python, or model account.

Expose extension interfaces for providers to register tools, resources, prompts, and diagnostics. Operation descriptors include names, descriptions, JSON input/output schemas, required capabilities, side-effect annotations, and handlers. A non-template manager adapter exposes existing OrganicUI factories and item operations.

Support MCP `2026-07-28`, plus compatibility adapters for `2025-11-25` and `2025-06-18`. Keep their lifecycle and notification behavior distinct, following the [MCP specification](https://modelcontextprotocol.io/specification/2026-07-28/basic/transports/streamable-http).

## Capability coverage

Keep tool names stable and organize them under `organic_*`, `timeline_*`, and `chataigne_*`. Agents discover available objects, types, commands, and parameter schemas through scoped, paginated queries.

| Area | Included capabilities |
|---|---|
| OrganicUI objects | Browse, search, inspect, create, duplicate, rename, move, reorder, remove, and configure supported objects and parameters. |
| Projects | New, open, save, save-as, import, export, inspect changes, and validate references and assets. |
| Modules and commands | Discover built-in and installed community modules; configure connections; inspect devices and values; discover, configure, and execute commands and templates. |
| States and processing | Author states, transitions, actions, conditions, consequences, mappings, inputs, filters, outputs, ranges, and multiplex configurations. |
| Time Machine | Author sequences, layers, clips, cues, triggers, automation keys, interpolation, media assignments, and synchronization; control playback and seeking. |
| Other workflows | Author routing, custom variables, groups, presets, and dashboards; expose Detective and Parrot inspection and operation. |
| Scripts | Inspect source and errors; create, edit, reload, and invoke scripts with the appropriate grant. |
| Diagnostics | Inspect warnings, recent logs, connection state, unresolved targets, missing assets, script failures, and processing dependencies; observe selected values over time. |

Provide resources for application identity, project summaries, object details, capabilities, diagnostics, and usage documentation. Include prompts for building projects, explaining signal flow, troubleshooting connections, and reviewing show readiness.

Read-only diagnostics use observed state and report their evidence. Executing a test command or emitting output is an explicit control operation.

## Access, reliability, and client setup

**Persistent access profiles**

- Offer **Inspect**, **Control**, **Edit**, and **Full Access**.
- Control can operate existing project behavior, including authored actions that invoke scripts or system commands.
- Edit adds structural authoring. Introducing or changing executable behavior, or directly invoking scripts/system commands, requires a separately remembered Advanced grant.
- Restricted profiles enforce approved folders for project, asset, and script file operations. Advanced execution grants code execution; it is not a filesystem sandbox.
- Full Access grants all supported actions and accessible file paths once, including scripts, system commands, and live editing.
- Bind remembered grants to authenticated credentials, rather than client-supplied names. Persist them across requests, reconnects, project changes, and restarts.
- Provide review, rotation, and revocation in Chataigne. Authorized operations require no further Chataigne approval. Preview is available without becoming an approval gate.
- External assistants retain their own approval rules.

**Execution guarantees**

- Validate arguments, types, ranges, target compatibility, and permissions before execution.
- Dispatch model access through the appropriate application thread and subsystem locks. Network callbacks never traverse mutable application objects directly.
- Use object handles backed by weak references, document generations, and edit revisions to detect deleted objects, project replacement, and conflicting edits.
- Group reversible edits into named undo transactions. Undo requests identify the intended transaction and reject intervening-history conflicts.
- Prevalidate batches; report completed operations and recovery details if execution fails. External output and script effects are reported accurately as irreversible.
- Support cancellation, deadlines, bounded queues, payload limits, pagination, and throttled observation streams.
- Stop accepting work before engine teardown; reject stale queued requests during project loading or replacement.

**Connections**

Default to an opt-in, authenticated loopback endpoint at configurable port `42002`. LAN access uses HTTPS or an encrypted tunnel. Validate Host and Origin headers and protect credentials in logs and diagnostic exports.

Provide a connection panel with client creation, access selection, endpoint information, copyable configuration, connection tests, activity history, and actionable errors.

Support Codex and Claude Code through HTTP configuration. Bundle a reusable native stdio bridge and Claude Desktop extension packaging for desktop clients needing stdio. The bridge connects to an explicitly selected running instance.

For hosted ChatGPT, provide guided [Secure MCP Tunnel setup](https://developers.openai.com/api/docs/guides/secure-mcp-tunnels), including its separate Platform credentials and workspace requirements. Public hosting and public plugin-directory publication are outside this release.

## Optional assistant inside Chataigne

Add a reusable dockable OrganicUI assistant panel with a provider interface. Ship a ChatGPT-plan provider using the documented [Sign in with ChatGPT flow for open-source local apps](https://developers.openai.com/siwc/token-sharing-open-source).

- Use browser sign-in with PKCE, validated account identity, a persistent host identifier, protected credential storage, and automatic token renewal.
- Retrieve account-specific model choices; stream responses and execute function calls locally through the shared operation registry.
- Include conversation history, project context, tool activity, edit summaries, cancellation, account switching, logout, and history deletion.
- Follow the route’s [preview requirements](https://developers.openai.com/siwc/token-sharing-open-source/preview-limitations), including client-managed conversation context and local tool execution.
- Display account eligibility and usage-limit failures clearly. Do not silently switch to API billing.
- Keep the assistant optional and identify the account integration as a preview. MCP remains independently usable.
- Claude support in this release means external Claude clients; an embedded Claude account integration is outside scope.

## Delivery and acceptance

Implement four reviewable stages:

1. OrganicUI registry, MCP transport, permissions, observation, and a minimal independent OrganicUI example.
2. Complete Chataigne and timeline providers, authoring workflows, undo, and diagnostics.
3. Connection UI, native bridge, client packaging, documentation, and interoperability checks.
4. Optional ChatGPT assistant, account integration, and final integrated validation.

Acceptance includes:

- Protocol tests for both modern and legacy versions, schemas, errors, notifications, cancellation, disconnects, and malformed traffic.
- Permission tests proving remembered grants survive restart, revocation takes effect, Full Access avoids repeated app prompts, and generic operations cannot bypass restricted capabilities.
- Authoring scenarios covering every capability family, save/reload, undo/redo, broken references, concurrent UI edits, and live playback.
- Diagnostic scenarios for connection failures, missing assets, script errors, and mappings that produce unexpected output.
- Stress tests with large control trees, concurrent clients, slow subscribers, repeated project loads, and shutdown during pending work.
- Actual Codex, Claude Code, Claude Desktop, and ChatGPT connection checks; mocked account failure tests and a real subscription-backed assistant smoke test.
- Builds and regression checks across supported Windows, macOS, Linux, and Raspberry targets, including MCP-disabled framework builds.

Use the shared JUCE checkout at `D:\Projects\Dev\JUCE` for local builds. Keep framework changes in their respective submodules, preserve existing project formats, and leave the local-only `site/` tree untouched.
