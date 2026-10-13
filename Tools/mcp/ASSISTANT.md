# ChatGPT assistant (preview)

Open the **ChatGPT Assistant** docked panel. It uses Chataigne's OrganicUI
parameters and triggers, colors, fonts and inspector controls. A narrow side
panel stacks its controls; a wide main panel gives the conversation more room.
Tool activity is collapsed into result summaries; click an activity card for
details. Ctrl+Enter sends a prompt; Enter inserts a line break.

**Global Settings > ChatGPT Assistant** holds account selection, browser sign-in,
model selection, project-context preference, access, approved folders, sign-out
and usage settings. **Account & access** in the conversation selects that settings
container in the inspector. The conversation itself is an OrganicUI container
under the engine: Prompt, Send, Cancel, Clear conversation, Status, Busy, and
Conversation. It is excluded from project saves. Prompt suggestions are ordinary
controllable triggers, and the editor binds directly to the Prompt parameter.

The integration uses your ChatGPT plan. Project context and conversation are
sent to OpenAI. Account eligibility and app usage limits are managed in
[ChatGPT Settings > Usage](https://chatgpt.com/settings/usage). There is no API-key
or API-billing fallback. MCP remains usable without an account or assistant.

Click **Continue with ChatGPT** to complete browser sign-in. For another account
or workspace, use **Add account**; saved registrations remain separate, even
when they share an email address. Model choices come from the selected account.
Reauthorization validates the selected registration before replacing credentials.

Each verified account has its own local MCP credential. It starts with Inspect.
Change Access, Advanced execution and Approved folders, then trigger **Remember
access**. These are the same grants shown in **Global Settings > MCP > Clients**,
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

History is local and account-specific. **Clear conversation** deletes that
account's stored conversation. **Cancel** stops network work and cancels queued
operations; completed tool effects and transaction IDs remain in activity.
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
reported distinctly. Tool rounds, queues, response sizes and history are bounded.

`assistant_core_test` checks PKCE, signed identity failures, renewal, protected
storage and fragmented SSE. The native application regression adds offline OAuth
callback/account/permission/UI scenarios. `assistant_subscription_smoke` opens
browser sign-in and performs a real plan-backed local tool call; `--saved-session`
uses an already validated session. This manual smoke test is excluded from CTest.
