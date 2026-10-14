#include "../../Modules/juce_organicui/assistant/AssistantProvider.h"
#include <iostream>

using namespace organic::assistant;
using namespace organic::mcp;
namespace
{
int checks = 0;
void require(bool condition, const char* message)
{ ++checks; if (!condition) throw std::runtime_error(message); }
void until(const std::function<bool()>& done)
{
    auto deadline = juce::Time::getMillisecondCounterHiRes() + 10000;
    while (!done() && juce::Time::getMillisecondCounterHiRes() < deadline)
        juce::MessageManager::getInstance()->runDispatchLoopUntil(5);
    require(done(), "Fixture completed before deadline");
}
var call(const String& name, const String& id, const var& args = object())
{ return object({{"type", "function_call"}, {"name", name}, {"call_id", id}, {"arguments", juce::JSON::toString(args, true)}}); }
var message() { return array({object({{"type", "message"}, {"role", "assistant"}, {"content", array({object({{"type", "output_text"}, {"text", "Done"}})})}})}); }
String lastUser(const var& payload)
{
    String text;
    for (auto item : *payload["input"].getArray()) if (item["role"].toString() == "user") text = item["content"].toString();
    return text;
}
class Fixture final : public AgentProvider
{
public:
    std::function<var(const var&)> respond;
    Fixture(Registry& registry, Permissions& permissions, juce::File storage)
        : AgentProvider(registry, permissions, storage, "Fixture", [] { return (juce::int64) 1; }, [] { return object(); })
    {
        activeRecord = object({{"key", "fixture"}, {"mcp_token", permissions.create("Fixture", Access::full).second}});
        state.active = "fixture"; state.signedIn = true;
        state.models = array({object({{"slug", "fixture-model"}, {"name", "Fixture model"}})});
        std::lock_guard<std::mutex> guard(mutex); loadConversations("fixture");
    }
    ~Fixture() override { cancel(); }
    ProviderInfo info() const override { return {"fixture", "Fixture", {}, {}, {}, false}; }
    juce::StringArray efforts(const String&) const override { return {"low", "high"}; }
    void signIn(bool) override {}
    void refreshModels() override {}
    void signOut() override { cancel(); }
    void wait() { until([&] { auto v = view(); return !v.busy && (v.queuePaused || v.pendingMessages.size() == 0); }); }
    void hold(std::atomic<bool>& entered, std::atomic<bool>& release)
    { entered = true; while (!release && !cancelled->load()) juce::Thread::sleep(1); check(); }
private:
    var accessToken(var record) override { return record; }
    void infer(const var& payload, const var&, ResponseStream& stream) override
    {
        auto output = respond(payload);
        auto event = "data: " + juce::JSON::toString(object({{"type", "response.completed"}, {"response", object({{"output", output}})}}), true) + "\n\n";
        stream.push(event.toRawUTF8(), event.getNumBytesAsUTF8(), [this](const String& delta) { append(delta); }); stream.finish();
    }
};
var task(const String& id, const String& description, const String& status, const String& verification = {})
{
    return object({{"id", id}, {"description", description}, {"status", status}, {"evidence", status == "blocked" ? "Device is disconnected" : "Inspected current configuration"},
        {"nextStep", status == "blocked" ? "Connect the audio device and select it in Sound Card settings." : ""},
        {"verifiedBy", verification.isEmpty() ? array({}) : array({verification})}});
}
}
int main()
{
    juce::ScopedJuceInitialiser_GUI runtime;
    auto directory = juce::File::getSpecialLocation(juce::File::tempDirectory).getNonexistentChildFile("assistant-turn-test", {}, false);
    struct Cleanup { juce::File directory; ~Cleanup() { directory.deleteRecursively(); } } cleanup {directory};
    try
    {
        Registry registry; Permissions permissions(directory.getChildFile("clients.json")); int edits = 0;
        std::function<void()> afterEdit;
        Operation edit; edit.name = "fixture_edit"; edit.capabilities = organic::mcp::edit;
        edit.execute = [&](const var&, Context&) { ++edits; if (afterEdit) afterEdit(); return object(); }; registry.add(edit);
        Operation read; read.name = "fixture_read"; read.output = openObjectSchema();
        read.execute = [&](const var&, Context&) { return object({{"edits", edits}}); }; registry.add(read);
        auto hasTone = [](const std::vector<MessageSpan>& spans, const String& text, MessageTone tone)
        { for (auto span : spans) if (span.tone == tone && span.text.contains(text)) return true; return false; };
        auto styled = messageSpans("Created the variables. <warning>Attention: one step remains.</warning> Presets are saved. <error>The device is disconnected.</error> Connect it and resume.");
        require(hasTone(styled, "Created the variables", MessageTone::normal) && hasTone(styled, "Presets are saved", MessageTone::normal), "Successful work within a warning paragraph keeps normal text colour");
        require(hasTone(styled, "Attention:", MessageTone::warning) && hasTone(styled, "device is disconnected", MessageTone::error), "Only warning and error fragments receive their semantic colours");
        require(hasTone(styled, "Connect it and resume", MessageTone::normal), "Recovery instructions after an inline error return to normal colour");
        require(hasTone(messageSpans("Attention: Progress saved"), "Progress saved", MessageTone::warning), "Existing attention headings use warning colour");
        require(hasTone(messageSpans("Created the sequence. Error: Connection failed. Your presets are saved."), "Your presets are saved", MessageTone::normal), "An untagged inline error does not colour the following information");
        require(hasTone(messageSpans("`<error>literal</error>`"), "<error>literal</error>", MessageTone::normal), "Semantic tags inside code remain literal");
        auto recoveryText = messageSpans(errorFragment("Connection failed.\n\nWhat to try: Reconnect, then resume."));
        require(hasTone(recoveryText, "Connection failed", MessageTone::error) && hasTone(recoveryText, "Reconnect, then resume", MessageTone::normal), "Error emphasis excludes the recovery paragraph");
        require(activityForTool("organic_create", object({{"type", "Sequence"}})) == "Creating sequences...", "Sequence creation has a readable activity fallback");
        require(activityForTool("organic_create", object({{"type", "Custom Variables Group"}})) == "Configuring custom variables...", "Custom variables have a readable activity fallback");
        require(activityForTool("organic_batch", object({{"operations", array({object({{"name", "organic_inspect"}}), object({{"name", "organic_create"}, {"arguments", object({{"type", "Preset"}})}})})}})) == "Creating presets...", "Batches expose their main authoring category instead of a raw tool name");
        {
            Fixture provider(registry, permissions, directory.getChildFile("activity")); std::atomic<bool> entered {false}, release {false}; int rounds = 0;
            provider.respond = [&](const var& payload)
            {
                if (++rounds == 1)
                {
                    require(payload["instructions"].toString().contains("assistant_progress BEFORE each meaningful phase"), "Both provider transports receive instructions for visible phase updates");
                    return array({call("assistant_progress", "phase", object({{"title", "Creating sequence presets..."}, {"detail", "Preparing Low and High presets"}})), call("fixture_read", "inspect-phase")});
                }
                provider.hold(entered, release); return message();
            };
            provider.send("Create presets", "fixture-model"); until([&] { return entered.load(); });
            auto view = provider.view();
            require(view.workTitle == "Creating sequence presets..." && view.workDetail == "Preparing Low and High presets", "Named phase survives tool execution and thinking between calls");
            require(view.activity == Activity::thinking && edits == 0, "Activity feedback neither edits the project nor claims completion");
            release = true; provider.wait();
            require(provider.view().activity == Activity::completed, "Progress updates share the existing completion flow");
            entered = false; release = false;
            provider.send("A different request", "fixture-model"); until([&] { return entered.load(); });
            require(provider.view().workTitle == "Planning your request..." && provider.view().workDetail.isEmpty(), "Each new request resets the previous activity label");
            release = true; provider.wait();
        }
        {
            Fixture provider(registry, permissions, directory.getChildFile("fifo")); std::atomic<bool> entered {false}, release {false};
            juce::StringArray requests, efforts;
            provider.respond = [&](const var& payload)
            {
                requests.add(lastUser(payload)); efforts.add(payload["reasoning"]["effort"].toString());
                if (requests.size() == 1) provider.hold(entered, release); return message();
            };
            provider.send("First", "fixture-model", "low"); until([&] { return entered.load(); });
            provider.send("Second", "fixture-model", "high"); provider.send("Remove me", "fixture-model");
            auto snapshot = provider.view(); provider.removeQueued(snapshot.pendingMessages[1]["id"].toString()); provider.send("Third", "fixture-model");
            snapshot.pendingMessages.getArray()->clear();
            require(provider.view().pendingMessages.size() == 2, "Queue snapshots are independent and individual messages can be removed");
            require(requests.size() == 1, "Queued requests do not run during the active request");
            release = true; provider.wait();
            require(requests == juce::StringArray{"First", "Second", "Third"}, "Follow-ups run in FIFO order without the removed message");
            require(efforts == juce::StringArray{"low", "high", ""}, "Each queued request retains its own effort");
        }
        {
            Fixture provider(registry, permissions, directory.getChildFile("steering")); std::atomic<bool> entered {false}, release {false}; int rounds = 0;
            provider.respond = [&](const var& payload)
            {
                if (++rounds == 1) { provider.hold(entered, release); return array({call("fixture_edit", "old1"), call("fixture_edit", "old2")}); }
                require(lastUser(payload) == "Use the new direction", "Steering becomes user context in the same request");
                int skipped = 0;
                for (auto item : *payload["input"].getArray()) if (item["output"].toString().startsWith("Not executed:")) ++skipped;
                require(skipped == 2, "Skipped calls retain matched outputs for valid conversation context"); return message();
            };
            provider.send("Old direction", "fixture-model"); until([&] { return entered.load(); });
            provider.send("Use the new direction", "fixture-model", "high");
            auto queuedId = provider.view().pendingMessages[0]["id"].toString(); provider.steer("Earlier correction");
            provider.steerQueued(queuedId);
            require(provider.view().pendingMessages.size() == 2 && provider.view().pendingMessages[1]["kind"].toString() == "steer"
                && provider.view().pendingMessages[1]["text"].toString() == "Use the new direction", "A promoted queued prompt becomes the newest steering instruction without duplication");
            release = true; provider.wait(); require(edits == 0 && rounds == 2, "Steering skips obsolete actions returned by an in-flight response");
            require(provider.view().transcript.contains("You: Use the new direction"), "Steering remains in protected conversation history");
        }
        {
            Fixture provider(registry, permissions, directory.getChildFile("between-actions")); int rounds = 0;
            afterEdit = [&] { provider.steer("Stop changing the project"); };
            provider.respond = [&](const var& payload)
            {
                if (++rounds == 1) return array({call("fixture_edit", "first"), call("fixture_edit", "second")});
                require(lastUser(payload) == "Stop changing the project", "Steering between project actions is consumed"); return message();
            };
            provider.send("Two changes", "fixture-model"); provider.wait(); afterEdit = {};
            require(edits == 1, "An action already executing finishes, but steering prevents the next action");
        }
        {
            Fixture provider(registry, permissions, directory.getChildFile("superseded-error")); std::atomic<bool> entered {false}, release {false}; int rounds = 0;
            provider.setAccess(Access::inspect, false, {});
            provider.respond = [&](const var& payload)
            {
                if (++rounds == 1) return array({call("fixture_edit", "denied-edit")});
                if (rounds == 2) { provider.hold(entered, release); return message(); }
                require(lastUser(payload) == "Only inspect instead", "Steering supersedes the denied action");
                if (rounds == 3) return array({call("fixture_read", "read-instead")});
                return message();
            };
            provider.send("Change the project", "fixture-model"); until([&] { return entered.load(); });
            provider.steer("Only inspect instead"); release = true; provider.wait();
            require(provider.view().activity == Activity::completed && !provider.view().canResume, "A superseded action's error does not keep the revised request blocked");
        }
        auto recoveryDirectory = directory.getChildFile("recovery"); String savedConversation;
        {
            Fixture provider(registry, permissions, recoveryDirectory); std::atomic<bool> entered {false}, release {false}; int rounds = 0;
            provider.respond = [&](const var&)
            {
                if (++rounds == 1)
                {
                    provider.hold(entered, release);
                    return array({call("fixture_read", "verify"), call("assistant_task", "progress", object({{"tasks", array({
                        task("created", "Create the workflow", "completed", "verify"), task("device", "Connect the audio input", "blocked")})}}))});
                }
                return message();
            };
            provider.send("Create workflow and connect audio", "fixture-model", "high"); until([&] { return entered.load(); });
            provider.send("Follow-up after recovery", "fixture-model"); release = true; provider.wait();
            auto view = provider.view(); savedConversation = view.conversation;
            require(view.activity == Activity::failed && view.canResume && view.queuePaused && view.pendingMessages.size() == 1, "An obstacle pauses the queue and offers resume");
            require(view.recovery.contains("Verified:") && view.recovery.contains("Create the workflow") && view.recovery.contains("Remaining:")
                && view.recovery.contains("Connect the audio device") && view.recovery.contains("Resume unfinished"), "Recovery explains verified work, remaining work, and the exact user action");
        }
        {
            Fixture provider(registry, permissions, recoveryDirectory); auto restored = provider.view(); int rounds = 0;
            require(restored.conversation == savedConversation && restored.tasks.size() == 2 && restored.canResume && restored.pendingMessages.size() == 1 && restored.queuePaused,
                "Restart restores checklist and pending messages without executing them");
            provider.respond = [&](const var& payload)
            {
                if (lastUser(payload) == "Follow-up after recovery") return message();
                if (++rounds == 1)
                {
                    require(payload["instructions"].toString().contains("Connect the audio input"), "Resume sends the saved checklist to the model");
                    require(lastUser(payload).contains("Device is now connected") && payload["reasoning"]["effort"].toString() == "low", "Resume includes missing details and the current effort selection");
                    return array({call("fixture_read", "reverify"), call("assistant_task", "done", object({{"tasks", array({
                        task("created", "Create the workflow", "completed", "reverify"), task("device", "Connect the audio input", "completed", "reverify")})}}))});
                }
                return message();
            };
            provider.resume("fixture-model", "low", "Device is now connected"); provider.wait();
            require(!provider.view().canResume && provider.view().pendingMessages.size() == 0 && provider.view().transcript.contains("You: Follow-up after recovery"), "Successful resume finishes the saved checklist then releases follow-ups");
        }
        {
            Fixture provider(registry, permissions, directory.getChildFile("cancel")); std::atomic<bool> entered {false}, release {false};
            provider.respond = [&](const var&) { provider.hold(entered, release); return message(); };
            provider.send("Long request", "fixture-model"); until([&] { return entered.load(); }); provider.send("Keep for later", "fixture-model"); provider.cancel();
            auto view = provider.view(); require(view.activity == Activity::stopped && view.canResume && view.queuePaused && view.pendingMessages.size() == 1, "Stop preserves follow-ups and resumable progress");
            provider.newConversation(); require(provider.view().pendingMessages.size() == 0 && !provider.view().canResume, "New conversations do not inherit the old queue");
            provider.selectConversation(view.conversation); require(provider.view().pendingMessages.size() == 1 && provider.view().canResume && provider.view().queuePaused, "Returning to stopped history restores its paused follow-ups");
            provider.respond = [&](const var& payload) { require(lastUser(payload) == "Keep for later", "Run queue starts the requested follow-up"); return message(); };
            provider.runQueue(); provider.wait(); require(provider.view().pendingMessages.size() == 0, "Paused follow-ups can explicitly run without resuming the stopped request");
        }
        {
            Fixture provider(registry, permissions, directory.getChildFile("automatic-verification")); int rounds = 0, editsBefore = edits; String receipt;
            provider.respond = [&](const var& payload)
            {
                if (++rounds == 1) return array({call("assistant_task", "plan", object({{"tasks", array({task("setup", "Create and configure the workflow", "pending")})}})), call("fixture_edit", "edit-once"), call("fixture_read", "opaque-transport-id")});
                if (rounds == 2)
                {
                    for (auto item : *payload["input"].getArray()) if (item["call_id"].toString() == "opaque-transport-id" && item["type"].toString() == "function_call_output") receipt = juce::JSON::parse(item["output"].toString())["verification"]["id"].toString();
                    require(receipt.startsWith("inspection-"), "Read-only results expose model-visible verification receipts");
                    return array({call("assistant_task", "finish", object({{"tasks", array({task("setup", "Configure the workflow and verify the final values", "completed")})}}))});
                }
                return message();
            };
            provider.send("Create and verify a workflow", "fixture-model"); provider.wait();
            auto view = provider.view();
            require(view.activity == Activity::completed && view.tasks[0]["verifiedBy"][0].toString() == receipt, "Completion automatically attaches real inspections without transport call IDs");
            require(edits == editsBefore + 1 && rounds == 3, "Rewording a checklist description does not block completion or replay edits");
        }
        {
            Fixture provider(registry, permissions, directory.getChildFile("verification-repair")); int rounds = 0;
            provider.respond = [&](const var& payload)
            {
                if (++rounds == 1) return array({call("fixture_read", "read"), call("assistant_task", "invented", object({{"tasks", array({task("setup", "Verify the workflow", "completed", "invented-id")})}}))});
                if (rounds == 2)
                {
                    auto error = juce::JSON::parse(payload["input"][payload["input"].size() - 1]["output"].toString());
                    require(error["availableVerifications"].size() == 1 && error["nextAction"].toString().contains("omit verifiedBy"), "Checklist errors tell the model exactly how to repair completion");
                    require(!error["message"].toString().contains("Resume unfinished"), "Internal bookkeeping does not ask the user to resume or report a false blocker");
                    return array({call("assistant_task", "fixed", object({{"tasks", array({task("setup", "Verify the workflow", "completed", error["availableVerifications"][0].toString())})}}))});
                }
                return message();
            };
            provider.send("Verify the workflow", "fixture-model"); provider.wait();
            require(provider.view().activity == Activity::completed && !provider.view().canResume, "An explicit returned receipt repairs invented references within one prompt");
        }
        {
            Fixture provider(registry, permissions, directory.getChildFile("stale-verification")); int rounds = 0;
            provider.respond = [&](const var& payload)
            {
                if (++rounds == 1) return array({call("fixture_read", "old-read"), call("fixture_edit", "later-edit"), call("assistant_task", "too-early", object({{"tasks", array({task("setup", "Configure and verify", "completed")})}}))});
                if (rounds == 2)
                {
                    auto error = juce::JSON::parse(payload["input"][payload["input"].size() - 1]["output"].toString());
                    require(error.hasProperty("error") && error["availableVerifications"].size() == 0, "Automatic attachment cannot use evidence invalidated by a later edit");
                    return array({call("fixture_read", "final-read"), call("assistant_task", "verified", object({{"tasks", array({task("setup", "Configure and verify", "completed")})}}))});
                }
                return message();
            };
            provider.send("Configure and verify", "fixture-model"); provider.wait();
            require(provider.view().activity == Activity::completed, "Fresh final inspection repairs stale evidence in the same request");
        }
        {
            Fixture provider(registry, permissions, directory.getChildFile("unclassified-failure")); int rounds = 0, failures = 0, editsBefore = edits;
            provider.respond = [&](const var&)
            {
                if (++rounds == 1) return array({call("assistant_task", "plan", object({{"tasks", array({task("setup", "Finish the configuration", "pending")})}})), call("fixture_edit", "edit-once")});
                if (failures++ < 2)
                {
                    ResponseStream failed; String event = "data: {\"type\":\"response.failed\",\"response\":{\"error\":{\"message\":\"Temporary generation failure\"}}}\n\n";
                    failed.push(event.toRawUTF8(), event.getNumBytesAsUTF8(), [](const String&) {});
                }
                if (rounds == 4) return array({call("fixture_read", "inspect-recovered"), call("assistant_task", "done", object({{"tasks", array({task("setup", "Finish the configuration", "completed")})}}))});
                return message();
            };
            provider.send("Finish despite an unclassified upstream failure", "fixture-model"); provider.wait();
            require(provider.view().activity == Activity::completed && edits == editsBefore + 1 && rounds == 5, "Unclassified stream failures recover automatically without replaying completed edits");
        }
        {
            Operation large; large.name = "fixture_large_read"; large.output = openObjectSchema(); large.execute = [](const var&, Context&) { return object({{"name", "Large inspected project"}, {"data", String::repeatedString("x", 18000)}}); }; registry.add(large);
            Fixture provider(registry, permissions, directory.getChildFile("large-activity")); int rounds = 0;
            provider.respond = [&](const var& payload)
            {
                if (++rounds == 1) return array({call("fixture_large_read", "large")});
                auto result = juce::JSON::parse(payload["input"][payload["input"].size() - 1]["output"].toString());
                require(result["data"].toString().length() == 18000 && result["verification"].isObject(), "The model receives full large inspection results and receipts");
                return message();
            };
            provider.send("Inspect a large project", "fixture-model"); provider.wait();
            auto line = provider.view().transcript.fromFirstOccurrenceOf("[Tool: fixture_large_read] ", false, false).upToFirstOccurrenceOf("\n", false, false);
            require(juce::JSON::parse(line)["name"].toString() == "Large inspected project" && line.length() < 4096, "Completed large actions keep valid bounded summaries instead of permanent Working labels");
        }
        {
            int attempts = 0;
            Operation repair; repair.name = "fixture_revision_read"; repair.output = openObjectSchema();
            repair.execute = [&](const var&, Context&) { if (++attempts == 1) throw Error(-32006, "Object changed since it was inspected"); return object({{"verified", true}}); }; registry.add(repair);
            Fixture provider(registry, permissions, directory.getChildFile("internal-tool-repair")); int rounds = 0;
            provider.respond = [&](const var& payload)
            {
                if (++rounds == 1) return array({call("fixture_revision_read", "stale")});
                if (rounds == 2)
                {
                    auto error = juce::JSON::parse(payload["input"][payload["input"].size() - 1]["output"].toString());
                    require((bool) error["recoverable"] && error["nextAction"].toString().contains("retry only unfinished"), "Recoverable tool errors provide internal repair instructions");
                    require(!error["message"].toString().contains("Resume unfinished"), "Recoverable validation does not tell the model to hand work back to the user");
                    return array({call("fixture_revision_read", "fresh")});
                }
                return message();
            };
            provider.send("Inspect and repair a stale revision internally", "fixture-model"); provider.wait();
            require(provider.view().activity == Activity::completed && attempts == 2, "An internally repaired tool error allows one-prompt completion");
        }
        std::cout << "Passed " << checks << " assistant steering, queue and recovery checks\n"; return 0;
    }
    catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 1; }
}
