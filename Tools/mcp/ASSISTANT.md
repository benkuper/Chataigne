# ChatGPT assistant (preview)

Open the **ChatGPT Assistant** docked panel. It uses Chataigne's OrganicUI
parameters and triggers, colors, fonts and inspector controls. The compact toolbar
has a history icon which switches to saved conversations, new conversation and
delete actions. Model, reasoning effort and remembered access share one row beside
the composer, including in narrow panels.
Effort uses `reasoning.effort` on every inference round. Auto omits that override;
choices are restricted to documented levels for the selected model family, and
unknown or non-reasoning variants stay on Auto. See the official
[reasoning guide](https://developers.openai.com/api/docs/guides/reasoning).
Consecutive tool calls collapse into one activity row with readable summaries;
click to expand. Transaction IDs and raw tool payloads stay out of the rendered
conversation. Messages support headings, lists, emphasis and code. Ctrl+Enter
sends a prompt; Enter inserts a line break. Message bodies use styled text;
hover a bubble to reveal its copy icon. The Copy conversation toolbar button
copies all messages, readable activity and errors,
plus model/effort/access/status context, without raw tool payloads or credentials.

**Global Settings > ChatGPT Assistant** holds account selection, browser sign-in,
model selection, project-context preference, access, approved folders, sign-out
and usage settings. The settings icon in the conversation selects that settings
container in the inspector. The conversation itself is an OrganicUI container
under the engine: Prompt, Send, Cancel, New conversation, Delete conversation, Status, Busy, and
Conversation. It is excluded from project saves. Prompt suggestions are ordinary
controllable triggers, and the editor binds directly to the Prompt parameter.

The integration uses your ChatGPT plan. Project context and conversation are
sent to OpenAI. Account eligibility and app usage limits are managed in
[ChatGPT Settings > Usage](https://chatgpt.com/settings/usage). There is no API-key
or API-billing fallback. MCP remains usable without an account or assistant.
The panel's **Usage** link opens that page. This direct sign-in integration has
no documented endpoint for a plan-usage percentage, so the panel does not invent
a percentage from response token counts or API rate limits.

Click **Continue with ChatGPT** to complete browser sign-in. For another account
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
operations; completed tool effects remain in activity. Status distinguishes
Thinking, Responding, Working, Completed, Stopped and Failed. Failures are also
saved in the transcript so they remain visible after another message or restart.
Unresolved tool errors mark the turn as needing attention, even when the model
has finished its response. Successful retries of those operations clear that state.
Failures include recovery instructions for access, approved folders, revoked local
credentials, connection problems, usage limits, project changes and unsupported
effort. Unresolved tool failures expose their specific recovery in the transcript.
**Sign out** stops requests, revokes the renewable session and clears local
tokens while retaining its registration. A revocation network failure is reported
explicitly so the app can also be disconnected from ChatGPT Settings.

For embedders, `ORGANICUI_USE_ASSISTANT` is off by default and requires MCP.
`assistant::Provider` is the native provider interface; `Controller` exposes the
OrganicUI model and a separate settings container; `Panel` binds its controls.
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
and bindings. Completed outcomes require successful read-only inspection call IDs from
the current turn; later edits invalidate that verification. A premature closing response
with pending work automatically resumes the same request, with a bounded guard against
repeated responses that make no progress. Transient inference failures retry up to twice
before any calls from that response execute; plan limits and access failures remain explicit.
Queues, response sizes and history remain bounded.

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
in one batch with grouped undo.
`assistant_subscription_smoke` opens
browser sign-in and performs a real plan-backed local tool call; `--saved-session`
uses an already validated session. This manual smoke test is excluded from CTest.
