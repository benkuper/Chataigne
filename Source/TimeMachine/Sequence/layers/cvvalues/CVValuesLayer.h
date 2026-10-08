#pragma once

#include "CustomVariables/CustomVariablesIncludes.h"
#include "CVValuesEvaluator.h"
#include <memory>

class CVValuesLayer;
class CVValuesBlock;

class CVValuesTarget : public BaseItem
{
public:
    CVValuesTarget();
    TargetParameter* group = nullptr;
    TargetParameter* basePreset = nullptr;
    InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = {}) override;
    DECLARE_TYPE("CV Values Target");
};

class CVValuesValue : public BaseItem
{
public:
    CVValuesValue(Parameter* source = nullptr, const String& type = "Float");
    TargetParameter* source = nullptr;
    Parameter* value = nullptr;
    BoolParameter* overrideValue = nullptr;
    EnumParameter* unsetBehavior = nullptr;
    EnumParameter* interpolation = nullptr;
    BoolParameter* animated = nullptr;
    std::unique_ptr<Automation> automation;
    std::unique_ptr<GradientColorManager> gradient;
    void syncMetadata(Parameter* p);
    void setLength(float length, bool stretch = false, bool stickToEnd = false);
    var getJSONData(bool includeNonOverriden = false) override;
    void onContainerParameterChangedInternal(Parameter* p) override;
    DECLARE_TYPE("CV Values Override");
};

class CVValuesValueManager : public BaseManager<CVValuesValue>
{
public:
    CVValuesValueManager();
    CVValuesValue* createItemFromData(var data) override;
};

class CVValuesBlockGroup : public BaseItem
{
public:
    CVValuesBlockGroup(CVValuesLayer* layer = nullptr);
    TargetParameter* target = nullptr;
    TargetParameter* preset = nullptr;
    CVValuesValueManager values;
    void sync(CVValuesTarget* targetRow, float length, bool presetBlock);
    DECLARE_TYPE("CV Values Block Group");
};

class CVValuesBlock : public LayerBlock
{
public:
    CVValuesBlock(CVValuesLayer* layer);
    CVValuesLayer* layer;
    EnumParameter* blockType = nullptr;
    BaseManager<CVValuesBlockGroup> groups;
    bool valid = true;
    bool validating = false;
    bool acceptedEnabled = true;
    double acceptedStart = 0, acceptedLength = 10;
    bool setTiming(double start, double length, bool stretch = false);
    void syncTargets();
    Component* createValuesEditor();
    void setCoreLength(float length, bool stretch, bool stickToEnd = false) override;
    void setStartTime(float start, bool keepCoreEnd = false, bool stickToEnd = false) override;
    void onContainerParameterChangedInternal(Parameter* p) override;
    void afterLoadJSONDataInternal() override;
    DECLARE_TYPE("CV Values Block");
};

class CVValuesBlockManager : public LayerBlockManager
{
public:
    CVValuesBlockManager(CVValuesLayer* layer);
    CVValuesLayer* cvLayer;
    LayerBlock* createItem() override;
    LayerBlock* addItem(LayerBlock* item = nullptr, var data = var(), bool addToUndo = true, bool notify = true);
    LayerBlock* addItemFromData(var data, bool addToUndo = true) override;
    bool accepts(const CVValuesBlock* block, double start, double length) const;
    struct Timing { CVValuesBlock* block; double start, length; };
    bool applyTimings(const std::vector<Timing>& timings, bool stretch = false);
    void askForPlaceBlockTime(LayerBlock* block, float time) override;
    void askForDuplicateItem(BaseItem* item) override;
    Array<LayerBlock*> addItemsFromClipboard(bool showWarning = true) override;
    // One action preserves timing and all animation keys for drag/span undo.
    UndoableAction* stateAction(var before, var after, bool alreadyApplied = false);
};

class CVValuesLayer : public SequenceLayer,
    public BaseManager<CVValuesTarget>::ManagerListener,
    public LayerBlockManager::ManagerListener,
    public EnumParameter::EnumParameterListener,
    private AsyncUpdater
{
public:
    CVValuesLayer(Sequence* sequence, var params);
    ~CVValuesLayer() override;
    BaseManager<CVValuesTarget> targets;
    CVValuesBlockManager blocks;
    EnumParameter* interpolationMode = nullptr;
    FloatParameter* fadeIn = nullptr;
    FloatParameter* fadeOut = nullptr;
    Automation interpolationCurve;

    struct Output { WeakReference<Parameter> target; WeakReference<ControllableContainer> group; var value; };
    struct Snapshot
    {
        CVValuesEvaluation::Timeline<var> timeline;
        std::vector<WeakReference<Parameter>> parameters;
        std::vector<WeakReference<ControllableContainer>> groups;
        std::vector<WeakReference<ControllableContainer>> targets, bases;
    };
    std::shared_ptr<const Snapshot> snapshot;
    std::atomic<bool> rebuilding{ false };
    std::atomic<bool> snapshotDirty{ true };
    Array<WeakReference<Parameter>> metadataParameters;
    void refresh();
    void rebuildSnapshot();
    Array<Output> evaluate(double time) const;
    void updateActiveBlocks(double time);
    void handleAsyncUpdate() override;
    void onContainerParameterChangedInternal(Parameter* p) override;
    void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
    void onControllableFeedbackUpdate(ControllableContainer* cc, Controllable* c) override;
    void childStructureChanged(ControllableContainer* cc) override;
    void childAddressChanged(ControllableContainer* cc) override;
    void onExternalParameterRangeChanged(Parameter*) override { refresh(); }
    void enumOptionAdded(EnumParameter*, const String&) override { refresh(); }
    void enumOptionUpdated(EnumParameter*, int, const String&, const String&) override { refresh(); }
    void enumOptionRemoved(EnumParameter*, const String&) override { refresh(); }
    void releaseMetadataListeners();
    void afterLoadJSONDataInternal() override;
    void clearItem() override;
    void itemAdded(CVValuesTarget*) override { refresh(); }
    void itemsAdded(Array<CVValuesTarget*>) override { refresh(); }
    void itemRemoved(CVValuesTarget*) override { refresh(); }
    void itemsRemoved(Array<CVValuesTarget*>) override { refresh(); }
    void itemAdded(LayerBlock*) override { refresh(); }
    void itemsAdded(Array<LayerBlock*>) override { refresh(); }
    void itemRemoved(LayerBlock*) override { refresh(); }
    void itemsRemoved(Array<LayerBlock*>) override { refresh(); }
    void itemsReordered() override { refresh(); }
    void selectAll(bool addToSelection = false) override;
    bool paste() override;
    void getSnapTimes(Array<float>* times) override;
    Array<Inspectable*> selectAllItemsBetweenInternal(float start, float end) override;
    Array<UndoableAction*> getRemoveAllItemsBetweenInternal(float start, float end) override;
    Array<UndoableAction*> getInsertTimespanInternal(float start, float length) override;
    Array<UndoableAction*> getRemoveTimespanInternal(float start, float end) override;
    SequenceLayerTimeline* getTimelineUI() override;
    static CVValuesLayer* create(Sequence* sequence, var params) { return new CVValuesLayer(sequence, params); }
    DECLARE_TYPE("CV Values");
};
