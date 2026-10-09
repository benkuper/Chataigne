#pragma once

class ConsequenceManager;

class ChataigneTimeTrigger : public ConditionManagerListener, public TimeTrigger
{
public:
    ChataigneTimeTrigger(StringRef name = "Trigger");
    ~ChataigneTimeTrigger() override;

    enum evaluateSetting { onlyOnce = 0, onlyEnterExit = 1, always = 2 };
    // Keep the action values used by existing projects.
    enum possibleActions { action_nothing = 0, action_deactivate = 1, action_triggerTrue = 2,
                           action_triggerFalse = 3, action_triggerBoth = 4 };
    enum InvalidHandling { atExit = 0, immediately = 1 };

    struct DurationState
    {
        EnumParameter *evaluation = nullptr, *neverValidExit = nullptr, *everValidExit = nullptr;
        EnumParameter *invalidHandling = nullptr, *forwardSeek = nullptr;
        BoolParameter *activeFeedback = nullptr, *validFeedback = nullptr, *everValidFeedback = nullptr;
        bool active = false, valid = false, everValid = false;
        bool trueExecuted = false, initialFalseExecuted = false;
        bool evaluated = false, lastEvaluatedValidity = false;
        bool evaluating = false, evaluationPending = false;
        std::uint64_t visitRevision = 0;
    };

    std::unique_ptr<DurationState> duration;
    EnumParameter* onRewind;
    std::unique_ptr<ConsequenceManager> csm, untcsm;
    std::unique_ptr<ConditionManager> cdm;

    void updateTriggerParams();
    void onContainerParameterChangedInternal(Parameter* p) override;
    void conditionManagerValidationChanged(ConditionManager*, int, bool) override;
    void trigger() override;
    void unTrigger() override;
    void triggerInternal() override;
    void unTriggerInternal() override;
    void exitedInternal(bool rewind) override;
    void setTimelineActive(bool active, bool evaluate, bool rewind = false) override;
    void evaluateCurrentVisit() override;
    bool replayForwardSeek() const override;
    var getJSONData(bool includeNonOverriden = false) override;
    void loadJSONData(var data, bool createIfNotThere = false) override;
    void loadJSONDataInternal(var data) override;

private:
    var savedDurationSettings;
    bool configuring = false;
    var getDurationSettings() const;
    void applyDurationSettings(var settings);
    void updateValidity();
    void updateFeedback();
    void dispatchAction(possibleActions action);
};
