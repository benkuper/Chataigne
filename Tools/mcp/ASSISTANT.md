# AI Assistant (preview)

Open the **AI Assistant** docked panel and choose **ChatGPT** or **Claude** in
the provider selector. It uses Chataigne's OrganicUI
parameters and triggers, colors, fonts and inspector controls. The compact toolbar
has a history icon which switches to saved conversations, new conversation and
delete actions. Model, reasoning effort, remembered access and message actions
share one compact row, wrapping when the dock is narrow.
Effort uses `reasoning.effort` on every inference round. Auto omits that override;
choices are restricted to documented levels for the selected model family, and
unknown or non-reasoning variants stay on Auto. See the official
[reasoning guide](https://developers.openai.com/api/docs/guides/reasoning).
Consecutive tool calls collapse into one activity row with readable summaries;
click to expand. Transaction IDs and raw tool payloads stay out of the rendered
conversation. Messages support headings, lists, emphasis and code. Ctrl+Enter
sends a prompt; during active work it queues a follow-up. **Steer** or
Ctrl+Shift+Enter applies the composer text to the current request before its next
project action. An action already executing finishes; unexecuted calls from the
old response are skipped and the new instructions are included in the same turn.
The primary message button combines **Send**, **Queue**, and **Stop**: during active
work it shows Stop with an empty message box and Queue when a draft is entered.
**Steer** appears beside it only while a request is active and the composer has
text. Selectors and message actions share one row when space permits, wrapping
in narrow docks. The adjacent actions menu also offers Stop while a
draft is present. Stopping preserves the draft and pauses follow-ups.
Enter inserts a line break. Message bodies use styled text;
hover a bubble to reveal its copy icon. The Copy conversation toolbar button
copies all messages, readable activity and errors,
plus model/effort/access/status context, without raw tool payloads or credentials.

While the assistant is busy, a compact animated activity card stays above the composer
(and remains visible in History). It shows a plain-language phase such as
**Creating sequence presets...**, **Setting up actions...**, or **Configuring
custom variables...**, with a short detail and the number of verified outcomes.
A subtle rotating indicator shows that work is active; it stops when work stops.
The assistant updates phases before their project operations using the local
`assistant_progress` tool. Automatic operation/batch summaries provide a fallback
when no named phase has been supplied. Phases stay visible while the assistant
thinks through the next step, without requiring the user to expand tool calls.
The card does not invent a completion percentage, and resets for each new request
or steering instruction. Copy conversation includes the current phase while busy.

**Global Settings > AI Assistant** holds the provider selector, with **ChatGPT**
and **Claude** child containers for each provider's connection, models,
project-context preference, access, approved folders, disconnect and usage settings.
The settings icon in the conversation selects the active provider's settings
container in the inspector. Switching providers stops active work and preserves
each provider's own draft, transcript and history. Existing ChatGPT panel layouts
and top-level preferences migrate automatically. The conversation is an OrganicUI container
under the engine: Prompt, Send, Steer, Resume unfinished, Run queue, Cancel, New conversation, Delete conversation, Status, Busy, and
Conversation. It is excluded from project saves. Prompt suggestions are ordinary
controllable triggers, and the editor binds directly to the Prompt parameter.

The ChatGPT provider uses your ChatGPT plan. Its project context and conversation
are sent to OpenAI. Account eligibility and app usage limits are managed in
[ChatGPT Settings > Usage](https://chatgpt.com/settings/usage). There is no API-key
or API-billing fallback. MCP remains usable without an account or assistant.
The provider's **Usage** action in Account and settings opens that page. This direct sign-in integration has
no documented endpoint for a plan-usage percentage, so the panel does not invent
a percentage from response token counts or API rate limits.

The Claude provider hosts your installed, unmodified **Claude Code** executable.
Choose **Connect Claude Code** to use its existing account, or complete Claude
Code's own browser sign-in if needed. Chataigne never collects or stores Claude
credentials. The CLI keeps its own authentication and billing configuration.
**Disconnect** revokes Chataigne's local tool grant and keeps the CLI account signed in.
Claude-specific conversation history and local MCP credentials are protected under
`Chataigne/Assistant/Claude`; existing ChatGPT storage stays in `Chataigne/Assistant`.

Claude runs with built-in tools disabled, an empty strict MCP configuration, hooks
disabled, no inherited settings sources and no CLI session persistence. Conversation
context travels over stdin, without shell interpolation or command-line size limits.
Its structured responses request local registered tools through the same application
thread, remembered access, batching, undo and completion checks as ChatGPT. CLI model
aliases include its configured default, Sonnet, Opus and Haiku; availability follows
the account configured in Claude Code. See Anthropic's official
[CLI integration documentation](https://code.claude.com/docs/en/headless).

For ChatGPT, click **Continue with ChatGPT** to complete browser sign-in. For another account
or workspace, use **Add account**; saved registrations remain separate, even
when they share an email address. Model choices come from the selected account.
Reauthorization validates the selected registration before replacing credentials.
On startup the provider restores the protected account and loads its model catalog
in the background, renewing tokens as needed. Browser authorization is only
initiated by an explicit sign-in action. Network failures preserve the saved
session and offer **Reconnect** in the panel; expired or revoked refresh tokens
require signing in again.

Each verified account has its own local MCP credential. It starts with Inspect.
Selecting Access remembers it immediately. Advanced execution and Approved folders
can be edited in settings and applied with **Remember access**. These are the same grants shown in **Global Settings > MCP > Clients**,
with immediate rotation/revocation enforcement. Account login does not bypass a
revoked MCP credential. Full Access enables all supported local operations;
Advanced permits executable behavior and is not a filesystem sandbox.

Secrets are stored in the provider's protected Global Settings backing directory
(`Chataigne/Assistant` in the current user's application-data directory), never
as public controllable values, project data, logs or copied client configuration.
Windows uses current-user DPAPI encryption. Unix files are atomically replaced
with owner-only permissions, inside an owned directory. The host identifier is
persistent. A registration is keyed by the issued client ID and verified issuer
and account subject. Refresh uses an interprocess lock and replaces the access
token, expiry, scopes and rotating refresh token together.

History is local and account-specific. The Back arrow or Escape returns to chat.
**New conversation** preserves previous
conversations and their complete tool context. History selection restores both
the visible transcript and model context. Selecting a conversation or starting a
new one stops and saves an active turn before switching; Stop remains available
in history. New conversation also clears the previous draft. Existing single-conversation histories
are retained as **Previous conversation**. **Delete conversation** removes only
the selected conversation. **Cancel** stops network work and cancels queued
operations; completed tool effects remain in activity. Pending messages are shown
above the composer, with **Steer**, **Edit** (move back to the composer), and **Remove** actions.
Steer promotes an existing queued message to the active request without duplicating
it or changing the composer draft. It is available while a request is active.
In narrow panels, each prompt gets a separate row of actions and a full-text tooltip.
Follow-ups run in order after successful completion, each retaining its selected
model and effort. Stop or a problem pauses the queue. **Run queue** explicitly
starts the follow-ups; **Resume unfinished** continues the stopped request first,
then releases the queue only if that request succeeds. Resume also includes any
missing details entered in the composer and uses the currently selected model/effort.
The queue is bounded to 16 pending messages and is saved with its conversation in
protected storage. Switching conversations or providers saves and pauses the old
queue; restoring history never automatically runs project actions.
Status distinguishes
Thinking, Responding, Working, Completed, Stopped and Failed. Failures are also
saved in the transcript so they remain visible after another message or restart.
Unresolved tool errors mark the turn as needing attention, even when the model
has finished its response. Successful retries of those operations clear that state.
Stopped and failed requests save a progress summary listing verified outcomes,
remaining work, specific obstacles and the next user action. Blocked checklist
entries must supply `nextStep` with the exact setting to change or information to
provide. The checklist and resume controls survive reopening a conversation or
restarting the app. A full conversation instead explains how to copy the summary
and continue in a new conversation. Failures include recovery instructions for access, approved folders, revoked local
credentials, connection problems, usage limits, project changes and unsupported
effort. Unresolved tool failures expose their specific recovery in the transcript.
**Sign out** stops requests, revokes the renewable session and clears local
tokens while retaining its registration. A revocation network failure is reported
explicitly so the app can also be disconnected from ChatGPT Settings.

For embedders, `ORGANICUI_USE_ASSISTANT` is off by default and requires MCP.
`assistant::Provider` is the provider interface, including display metadata and
supported effort choices. `AgentProvider` shares the protected conversation and
tool execution loop; `ChatGptProvider` and `ClaudeProvider` implement authentication
and inference. `Controller` exposes each provider's OrganicUI controls and settings;
`Hub` owns the common provider selector/settings hierarchy, and `HubPanel` presents
the selected provider through the shared conversation UI.
Attach the settings container to Global Settings and exclude conversation state
from project serialization. Local tool calls go through the same Registry,
Permissions, schemas, revisions and undo handlers as external clients, on the
application thread. Network threads only hold immutable envelopes.

The provider follows the official [open-source sign-in flow](https://developers.openai.com/siwc/token-sharing-open-source/sign-in),
[account and renewal rules](https://developers.openai.com/siwc/token-sharing-open-source/profiles-and-sessions),
and [models and inference route](https://developers.openai.com/siwc/token-sharing-open-source/models-and-inference).
It verifies RS256 signatures against OpenAI JWKS, issuer, issued-client audience,
expiry, nonce and returning identity; uses loopback `/auth/callback` with PKCE;
and uses client-managed context, streamed Responses with `store:false`, namespaced
functions and local execution. Streamed output items are retained when the final
envelope omits them. Interrupted/incomplete streams and subscription limits are
reported distinctly. Missing streamed calls are retained even if the final
envelope also contains a message. Authoring runs allow up to 256 tool rounds.
The assistant tracks every requested outcome with a checklist, including configuration
and bindings. Successful read-only inspections return a model-visible `verification.id`.
Completed outcomes require fresh inspections from the current turn; omit `verifiedBy`
to attach them automatically, or use those returned IDs explicitly. Later edits
invalidate that verification. Checklist IDs remain stable while descriptions may be
clarified. Checklist validation and stale-object errors include internal repair
instructions rather than asking the user to repair integration bookkeeping.
A premature closing response
with pending work automatically resumes the same request, with a bounded guard against
repeated responses that make no progress. Transient inference failures retry up to twice
before any calls from that response execute, including unclassified terminal failures;
plan limits and access failures remain explicit. Standard top-level and nested streamed
error messages are retained. UI activity uses bounded, valid JSON summaries even when
the model receives a large tool result, so completed actions do not stay on “Working”.
Attention headings and warning fragments are yellow, actual error fragments red, and
progress and recovery instructions use the normal text colour. Single-line
`<warning>...</warning>` and `<error>...</error>` fragments also work inside paragraphs;
readable transcript exports strip this presentation markup.
Queues, response sizes and history remain bounded. `assistant_turn_test` provides
offline regression coverage for FIFO follow-ups, per-message effort, removal,
steering during inference and between actions, skipped-call context, paused
recovery, resume, restart restoration and isolation between conversations.

For an explicit live one-prompt regression, build the native authoring test and run
`mcp_authoring_test.exe --assistant-workflow-smoke` from `Builds/VisualStudio2022`.
This uses the saved ChatGPT subscription and remembered grant in a disposable empty
engine, with GPT-6.1-Sol. It does not upgrade permissions or load the user's project.
It checks varied variables and ranges, populated presets, twenty distinct trigger
times and resolved preset targets, and simulated Ctrl+P / one-second Ctrl+M chords
(including cancellation when a key is released). Project/transcript artifacts go
to ignored `Binaries`; the test conversation is removed and the prior selection
restored. It consumes subscription usage and is deliberately excluded from automated
offline test runs.

Repeated creation and configuration should use `organic_batch`, which supports up to
64 operations per call and one undo group. Explicit revisions are validated before any
edits and advanced for changes made within the batch. An operation can declare `id`;
later operations use `target` with that ID and an optional discovered relative child
`path` to configure or inspect the result without another model round. For example:

```json
{"operations":[
  {"id":"step01","name":"organic_create","arguments":{"handle":"<trigger-manager>","revision":1,"type":"<discovered-type>","name":"Step 01"}},
  {"name":"organic_set_value","target":"step01","path":"time","arguments":{"value":1}},
  {"name":"organic_inspect","target":"step01","path":"time","arguments":{}}
]}
```

Child paths are relative control addresses discovered from an existing exemplar, including
nested managers or command parameters. This works for any registered operation accepting
a handle, with its existing permissions and typed validation. Dependent targets are
validated immediately before execution. Preview never creates anything and reports
`deferredValidation`; `valid` is false while dependent validation remains. On failure,
the batch stops and returns its completed operations, error, and undo/recovery information.

`assistant_core_test` checks PKCE, signed identity failures, renewal, protected
storage and fragmented SSE. The native application regression adds offline OAuth
callback/account/permission/UI scenarios, automatic startup restoration, saved
conversation switching (including pointer hit testing during active work), effort
request payloads, hover copying, narrow selector layout, readable conversation export, longer
authoring, automatic completion/retry recovery, rejected invented verification, Sound Card
volume > 0.5 configuration, and creation/configuration/inspection of 20 sequence triggers
in one batch with grouped undo. Provider regression coverage includes Claude's local
tool execution and permissions, cancellation, grant revocation, protected history,
provider switching, legacy settings/layout migration and narrow shared-panel layout.
`assistant_subscription_smoke` opens
browser sign-in and performs a real plan-backed local tool call; `--saved-session`
uses an already validated ChatGPT session. `--claude` performs the equivalent manual
Claude check in an isolated temporary project; `--claude-check` only checks the native
CLI process and sign-in detection, without opening a browser or requesting inference.
These manual smoke checks are excluded from CTest.
