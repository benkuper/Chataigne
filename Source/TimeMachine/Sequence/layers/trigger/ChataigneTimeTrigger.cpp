#include "Common/Processor/ProcessorIncludes.h"

ChataigneTimeTrigger::ChataigneTimeTrigger(StringRef name) : TimeTrigger(name)
{
    onRewind = addEnumParameter("On rewind", "Consequences when seeking backward across this cue or out of this duration");
    onRewind->addOption("Run FALSE", action_triggerFalse)->addOption("Nothing", action_nothing);
    cdm.reset(new ConditionManager(nullptr));
    addChildControllableContainer(cdm.get());
    cdm->addConditionManagerListener(this);
    csm.reset(new ConsequenceManager("Consequences : TRUE"));
    untcsm.reset(new ConsequenceManager("Consequences : FALSE"));
    addChildControllableContainer(csm.get());
    addChildControllableContainer(untcsm.get());
}

ChataigneTimeTrigger::~ChataigneTimeTrigger()
{
    cdm->removeConditionManagerListener(this);
}

var ChataigneTimeTrigger::getDurationSettings() const
{
    if (!duration) return savedDurationSettings;
    var settings(new DynamicObject());
    for (auto* p : { duration->evaluation, duration->neverValidExit, duration->everValidExit,
                    duration->invalidHandling, duration->forwardSeek })
        if (p->getValueKey() != p->defaultValue.toString())
            settings.getDynamicObject()->setProperty(p->shortName, p->getValueData());
    return settings.getDynamicObject()->getProperties().size() == 0 ? var() : settings;
}

void ChataigneTimeTrigger::applyDurationSettings(var settings)
{
    if (!duration || !settings.isObject()) return;
    for (auto* p : { duration->evaluation, duration->neverValidExit, duration->everValidExit,
                    duration->invalidHandling, duration->forwardSeek })
        if (settings.hasProperty(p->shortName)) p->setValueWithData(settings.getProperty(p->shortName, var()));
}

void ChataigneTimeTrigger::updateTriggerParams()
{
    if (configuring) return;
    const ScopedValueSetter<bool> setup(configuring, true);
    if (length->floatValue() > 0 && !duration)
    {
        duration.reset(new DurationState());
        auto& d = *duration;
        d.evaluation = addEnumParameter("Evaluate conditions", "When to execute consequences while inside the duration");
        d.evaluation->addOption("Only once", onlyOnce)->addOption("Only on enter and exit", onlyEnterExit)->addOption("Always", always);
        d.everValidExit = addEnumParameter("On exit if ever valid", "Action if the condition was valid at least once during this visit");
        d.everValidExit->addOption("Nothing", action_nothing)->addOption("Run FALSE", action_triggerFalse);
        d.neverValidExit = addEnumParameter("On exit if never valid", "Action if the condition was never valid during this visit; TRUE is allowed even if invalid");
        d.neverValidExit->addOption("Run FALSE", action_triggerFalse)->addOption("Run TRUE", action_triggerTrue)
            ->addOption("Nothing", action_nothing)->addOption("Run both", action_triggerBoth);
        d.invalidHandling = addEnumParameter("Invalid handling", "Run FALSE inside the duration immediately, or defer to the exit rules");
        d.invalidHandling->addOption("At exit", atExit)->addOption("Immediately", immediately);
        d.forwardSeek = addEnumParameter("Duration forward seek", "Synchronize at the destination, or replay entry and exit for skipped blocks");
        d.forwardSeek->addOption("At destination", 0)->addOption("Replay boundaries", 1);
        auto feedback = [this](const String& name, const String& description)
        {
            auto* p = addBoolParameter(name, description, false);
            p->isSavable = false;
            p->setControllableFeedbackOnly(true);
            return p;
        };
        d.activeFeedback = feedback("Active", "The playhead is inside this duration");
        d.validFeedback = feedback("Valid", "The current condition result");
        d.everValidFeedback = feedback("Was ever valid", "The condition was valid at least once during this visit");
        applyDurationSettings(savedDurationSettings);
        savedDurationSettings = var();
        // Editing a length must not execute consequences.
        if (auto* seq = ControllableUtil::findParentAs<Sequence>(this))
            d.active = seq->currentTime->floatValue() >= time->floatValue()
                && seq->currentTime->floatValue() < time->floatValue() + length->floatValue();
        collisionState = d.active;
        updateValidity();
        updateFeedback();
    }
    else if (length->floatValue() <= 0 && duration)
    {
        savedDurationSettings = getDurationSettings();
        auto old = std::move(duration);
        Parameter* parameters[] = { old->evaluation, old->neverValidExit, old->everValidExit,
            old->invalidHandling, old->forwardSeek, old->activeFeedback, old->validFeedback, old->everValidFeedback };
        for (auto* p : parameters) removeControllable(p);
        collisionState = false;
    }
    triggerAtAnyTime = duration != nullptr;
}

void ChataigneTimeTrigger::onContainerParameterChangedInternal(Parameter* p)
{
    TimeTrigger::onContainerParameterChangedInternal(p);
    if (p == enabled && csm && untcsm)
    {
        csm->setForceDisabled(!enabled->boolValue());
        untcsm->setForceDisabled(!enabled->boolValue());
    }
    if (p == length) updateTriggerParams();
}

void ChataigneTimeTrigger::updateValidity()
{
    if (!duration) return;
    auto& d = *duration;
    d.valid = cdm->getIsValid(0, true);
    if (d.active && d.valid) d.everValid = true;
    updateFeedback();
}

void ChataigneTimeTrigger::updateFeedback()
{
    auto& d = *duration;
    const bool changed = d.activeFeedback->boolValue() != d.active || d.validFeedback->boolValue() != d.valid
        || d.everValidFeedback->boolValue() != d.everValid;
    d.activeFeedback->setValue(d.active, true);
    d.validFeedback->setValue(d.valid, true);
    d.everValidFeedback->setValue(d.everValid, true);
    if (!changed) return;
    std::vector<WeakReference<Parameter>> feedback { d.activeFeedback, d.validFeedback, d.everValidFeedback };
    MessageManager::callAsync([feedback]()
    {
        for (const auto& p : feedback) if (p != nullptr) p->notifyValueChanged();
    });
}

void ChataigneTimeTrigger::conditionManagerValidationChanged(ConditionManager*, int, bool)
{
    if (!duration) return;
    const bool wasValid = duration->valid;
    updateValidity();
    const auto mode = duration->evaluation->getValueDataAsEnum<evaluateSetting>();
    if (duration->evaluating && !configuring && !duration->evaluationPending)
    {
        duration->evaluationPending = true;
        WeakReference<ControllableContainer> safeThis(this);
        WeakReference<Parameter> visitFeedback(duration->activeFeedback);
        const auto visit = duration->visitRevision;
        MessageManager::callAsync([safeThis, visitFeedback, visit]()
        {
            if (auto* trigger = dynamic_cast<ChataigneTimeTrigger*>(safeThis.get()))
            {
                if (visitFeedback != nullptr && trigger->duration && trigger->duration->activeFeedback == visitFeedback.get())
                {
                    trigger->duration->evaluationPending = false;
                    if (trigger->duration->visitRevision == visit
                        && trigger->duration->evaluation->getValueDataAsEnum<evaluateSetting>() != onlyEnterExit)
                        trigger->evaluateCurrentVisit();
                }
            }
        });
    }
    if (!configuring && duration->active && mode != onlyEnterExit
        && (duration->valid != wasValid || mode == onlyOnce)) evaluateCurrentVisit();
}

void ChataigneTimeTrigger::trigger()
{
    if (duration) evaluateCurrentVisit();
    else dispatchConsequences(cdm->getIsValid(0, true));
}

void ChataigneTimeTrigger::unTrigger() { dispatchConsequences(false); }
void ChataigneTimeTrigger::triggerInternal() { csm->triggerAll(); }
void ChataigneTimeTrigger::unTriggerInternal() { untcsm->triggerAll(); }

void ChataigneTimeTrigger::dispatchAction(possibleActions action)
{
    WeakReference<ControllableContainer> safeThis(this);
    auto* sequence = ControllableUtil::findParentAs<Sequence>(this);
    WeakReference<ControllableContainer> safeSequence(sequence);
    const auto revision = sequence ? sequence->transportRevision.load() : 0;
    if (action == action_triggerTrue || action == action_triggerBoth) dispatchConsequences(true);
    if (sequence && (safeSequence == nullptr || sequence->transportRevision.load() != revision)) return;
    if (safeThis != nullptr && (action == action_triggerFalse || action == action_triggerBoth)) dispatchConsequences(false);
}

void ChataigneTimeTrigger::evaluateCurrentVisit()
{
    if (!duration || !duration->active || configuring || duration->evaluating || !enabled->boolValue()) return;
    auto* layer = ControllableUtil::findParentAs<SequenceLayer>(this);
    if (layer && (!layer->enabled->boolValue() || !layer->sequence->enabled->boolValue())) return;
    updateValidity();
    auto& d = *duration;
    auto mode = d.evaluation->getValueDataAsEnum<evaluateSetting>();
    if (d.evaluated && (mode == onlyEnterExit || (mode == always && d.valid == d.lastEvaluatedValidity))) return;
    d.evaluated = true;
    d.lastEvaluatedValidity = d.valid;
    possibleActions action = action_nothing;
    if (d.valid && (mode != onlyOnce || !d.trueExecuted))
    {
        d.trueExecuted = true;
        action = action_triggerTrue;
    }
    else if (!d.valid && d.invalidHandling->getValueDataAsEnum<InvalidHandling>() == immediately
        && (mode != onlyOnce || (!d.trueExecuted && !d.initialFalseExecuted)))
    {
        d.initialFalseExecuted = true;
        action = action_triggerFalse;
    }
    // A consequence can delete this trigger; do not retain a reference or RAII setter into it.
    d.evaluating = true;
    WeakReference<ControllableContainer> safeThis(this);
    WeakReference<Parameter> visitFeedback(d.activeFeedback);
    dispatchAction(action);
    if (safeThis != nullptr && duration && visitFeedback != nullptr && duration->activeFeedback == visitFeedback.get())
        duration->evaluating = false;
}

void ChataigneTimeTrigger::setTimelineActive(bool active, bool evaluate, bool rewind)
{
    if (!duration)
    {
        if (rewind && (!evaluate || onRewind->getValueDataAsEnum<possibleActions>() != action_triggerFalse)
            && isTriggered->boolValue())
        {
            isTriggered->setValue(false, true);
            WeakReference<Parameter> feedback(isTriggered);
            MessageManager::callAsync([feedback]() { if (feedback != nullptr) feedback->notifyValueChanged(); });
        }
        if (evaluate) { if (active) trigger(); else if (rewind) exitedInternal(true); }
        return;
    }
    if (duration->active == active) return;
    auto& d = *duration;
    possibleActions exitAction = action_nothing;
    if (!active)
        exitAction = rewind ? onRewind->getValueDataAsEnum<possibleActions>()
            : (d.everValid ? d.everValidExit : d.neverValidExit)->getValueDataAsEnum<possibleActions>();
    d.active = active;
    ++d.visitRevision;
    collisionState = active;
    if (active)
    {
        d.everValid = false;
        d.trueExecuted = d.initialFalseExecuted = d.evaluated = false;
        updateValidity();
    }
    updateFeedback();
    if (evaluate)
    {
        if (active) evaluateCurrentVisit();
        else dispatchAction(exitAction);
    }
}

void ChataigneTimeTrigger::exitedInternal(bool rewind)
{
    if (duration) setTimelineActive(false, true, rewind);
    else if (rewind) dispatchAction(onRewind->getValueDataAsEnum<possibleActions>());
}

bool ChataigneTimeTrigger::replayForwardSeek() const
{
    return duration && (int)duration->forwardSeek->getValueData() == 1;
}

var ChataigneTimeTrigger::getJSONData(bool includeNonOverriden)
{
    var data = TimeTrigger::getJSONData(includeNonOverriden);
    data.getDynamicObject()->setProperty("consequences", csm->getJSONData());
    data.getDynamicObject()->setProperty("untriggerConsequences", untcsm->getJSONData());
    data.getDynamicObject()->setProperty("conditions", cdm->getJSONData());
    if (!duration && !savedDurationSettings.isVoid()) data.getDynamicObject()->setProperty("durationSettings", savedDurationSettings);
    return data;
}

void ChataigneTimeTrigger::loadJSONData(var data, bool createIfNotThere)
{
    if (!data.isObject()) return;
    const ScopedValueSetter<bool> loading(configuring, true);
    length->setValue(0, true);
    configuring = false;
    updateTriggerParams();
    configuring = true;
    savedDurationSettings = data.getProperty("durationSettings", var()).clone();
    // Pre-read length: duration settings may precede it in the parameter array.
    if (auto* parameters = data.getProperty("parameters", var()).getArray())
        for (const auto& p : *parameters)
            if (p.getProperty("controlAddress", "").toString() == "/" + length->shortName)
                length->setValue(p.getProperty("value", 0), true);
    configuring = false;
    updateTriggerParams();
    configuring = true;
    var normalized = data.clone();
    if (auto* parameters = normalized.getProperty("parameters", var()).getArray())
        for (auto& p : *parameters)
        {
            String address = p.getProperty("controlAddress", "");
            String value = p.getProperty("value", var()).toString();
            if (address == "/" + StringUtil::toShortName("On exit (active)"))
                p.getDynamicObject()->setProperty("controlAddress", "/" + StringUtil::toShortName("On exit if ever valid"));
            if (address == "/" + StringUtil::toShortName("On exit (inactive)"))
                p.getDynamicObject()->setProperty("controlAddress", "/" + StringUtil::toShortName("On exit if never valid"));
            if (value.startsWith("Deactivate - "))
            {
                String replacement = value.contains("both") ? "Run both" : value.contains("FALSE") ? "Run FALSE"
                    : value.contains("TRUE") ? "Run TRUE" : "Nothing";
                p.getDynamicObject()->setProperty("value", replacement);
            }
            if (!duration)
            {
                // Old point cues may contain hidden duration choices. Keep only compact values.
                String key = p.getProperty("controlAddress", "").toString().trimCharactersAtStart("/");
                String label = p.getProperty("value", "").toString();
                int selected = 0, defaultSelection = 0;
                bool durationSetting = true;
                if (key == StringUtil::toShortName("Evaluate conditions"))
                    selected = label == "Always" ? always : label == "Only on enter and exit" ? onlyEnterExit : onlyOnce;
                else if (key == StringUtil::toShortName("On exit if ever valid") || key == StringUtil::toShortName("On exit if never valid"))
                {
                    selected = label == "Run TRUE" ? action_triggerTrue : label == "Run FALSE" ? action_triggerFalse
                        : label == "Run both" ? action_triggerBoth : action_nothing;
                    if (key == StringUtil::toShortName("On exit if never valid")) defaultSelection = action_triggerFalse;
                }
                else if (key == StringUtil::toShortName("Invalid handling")) selected = label == "Immediately" ? immediately : atExit;
                else if (key == StringUtil::toShortName("Duration forward seek")) selected = label == "Replay boundaries" ? 1 : 0;
                else durationSetting = false;
                if (durationSetting && selected != defaultSelection)
                {
                    if (!savedDurationSettings.isObject()) savedDurationSettings = var(new DynamicObject());
                    savedDurationSettings.getDynamicObject()->setProperty(key, selected);
                }
            }
        }
    if (!duration)
        if (auto* parameters = normalized.getProperty("parameters", var()).getArray())
            for (int i = parameters->size(); --i >= 0;)
            {
                const String key = (*parameters)[i].getProperty("controlAddress", "").toString().trimCharactersAtStart("/");
                for (const char* name : { "Evaluate conditions", "On exit if ever valid", "On exit if never valid",
                        "Invalid handling", "Duration forward seek", "Active", "Valid", "Was ever valid" })
                    if (key == StringUtil::toShortName(name)) { parameters->remove(i); break; }
            }
    TimeTrigger::loadJSONData(normalized, createIfNotThere);
    if (duration)
    {
        duration->everValid = false;
        duration->trueExecuted = duration->initialFalseExecuted = duration->evaluated = false;
    }
    updateValidity();
    // Refresh an attached manager even when pre-reading length made its generic load a no-op.
    length->notifyValueChanged();
}

void ChataigneTimeTrigger::loadJSONDataInternal(var data)
{
    TimeTrigger::loadJSONDataInternal(data);
    csm->loadJSONData(data.getProperty("consequences", var()));
    untcsm->loadJSONData(data.getProperty("untriggerConsequences", var()));
    cdm->loadJSONData(data.getProperty("conditions", var()));
}
