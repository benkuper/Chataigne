#include "TimeMachine/TimeMachineIncludes.h"
#include <cstring>

namespace CVValuesHelpers
{
using namespace CVValuesEvaluation;

static void presetSelector(TargetParameter* p, CVGroup* group = nullptr)
{
    p->targetType = TargetParameter::CONTAINER;
    WeakReference<ControllableContainer> groupRef(group);
    p->customGetTargetContainerFunc = [groupRef](ControllableContainer*, std::function<void(ControllableContainer*)> callback)
    { CVGroupManager::showMenuAndGetPreset(groupRef.get(), callback); };
    p->defaultContainerTypeCheckFunc = [groupRef](ControllableContainer* cc)
    { auto* preset = dynamic_cast<CVPreset*>(cc); return preset && (!groupRef || preset->group == groupRef.get()); };
}

static Mode mode(ParameterPreset::InterpolationMode m)
{
    return m == ParameterPreset::CHANGE_AT_START ? Mode::Start
        : m == ParameterPreset::CHANGE_AT_END ? Mode::End : Mode::Interpolate;
}

static var interpolate(Controllable::Type type, const var& a, const var& b, double w)
{
    if (type == Controllable::FLOAT) return (double)a + ((double)b - (double)a) * w;
    if (type == Controllable::INT) return static_cast<int>((double)a + ((double)b - (double)a) * w);
    if (type == Controllable::COLOR || type == Controllable::POINT2D || type == Controllable::POINT3D)
    {
        if (!a.isArray() || !b.isArray() || a.size() != b.size()) return w < 1 ? a : b;
        var v;
        for (int i = 0; i < a.size(); ++i) v.append((double)a[i] + ((double)b[i] - (double)a[i]) * w);
        return v;
    }
    return w > 0 ? b : a;
}

static var colorValue(Colour color)
{
    var v;
    v.append(color.getFloatRed()); v.append(color.getFloatGreen());
    v.append(color.getFloatBlue()); v.append(color.getFloatAlpha());
    return v;
}

static std::shared_ptr<Automation> copyAutomation(Automation& source)
{
    auto copy = std::make_shared<Automation>("Evaluation Curve", nullptr, true);
    copy->selectItemWhenCreated = false;
    copy->loadJSONData(source.getJSONData());
    return copy;
}

static double sampleAutomation(Automation& automation, double time)
{
    auto* key = automation.getKeyForPosition(time);
    auto* noise = key ? dynamic_cast<NoiseEasing*>(key->easing.get()) : nullptr;
    if (!noise || !key->nextKey || time <= key->position->doubleValue()
        || time >= key->nextKey->position->doubleValue()) return automation.getValueAtPosition(time);
    // Noise normally uses a stateful RNG. Derive its sample from authored time,
    // without modifying the curve or depending on the previously visited time.
    uint64_t seed = 0;
    std::memcpy(&seed, &time, sizeof(time));
    seed ^= seed >> 30; seed *= 0xbf58476d1ce4e5b9ULL;
    seed ^= seed >> 27; seed *= 0x94d049bb133111ebULL; seed ^= seed >> 31;
    const double random = static_cast<double>(seed >> 11) / 9007199254740992.0;
    const double duration = key->nextKey->position->doubleValue() - key->position->doubleValue();
    const double w = (time - key->position->doubleValue()) / duration;
    const double t1 = noise->taper1->x / duration, t2 = -noise->taper2->x / duration;
    double amplitude = noise->taper1->y;
    if (t1 > 0 && w < t1) amplitude *= (1 - std::cos(MathConstants<double>::pi * w / t1)) * .5;
    if (t2 > 0 && w > 1 - t2) amplitude *= (1 + std::cos(MathConstants<double>::pi * (w - (1 - t2)) / t2)) * .5;
    return key->value->doubleValue() + (key->nextKey->value->doubleValue() - key->value->doubleValue()) * w
        + (random - .5) * 2 * amplitude;
}

static double storedNumber(var data, const String& name, double fallback)
{
    if (auto* parameters = data.getProperty("parameters", var()).getArray())
        for (auto p : *parameters)
            if (p.getProperty("controlAddress", "").toString() == "/" + name)
                return (double)p.getProperty("value", fallback);
    return fallback;
}

static void storeNumber(var data, const String& name, double value)
{
    var parameters = data.getProperty("parameters", var());
    if (auto* list = parameters.getArray())
        for (auto p : *list)
            if (p.getProperty("controlAddress", "").toString() == "/" + name)
            { p.getDynamicObject()->setProperty("value", value); return; }
    var p(new DynamicObject());
    p.getDynamicObject()->setProperty("controlAddress", "/" + name);
    p.getDynamicObject()->setProperty("value", value);
    parameters.append(p);
    data.getDynamicObject()->setProperty("parameters", parameters);
}
}

CVValuesTarget::CVValuesTarget() : BaseItem("Group", true, false)
{
    saveAndLoadRecursiveData = true;
    group = addTargetParameter("CV Group", "Custom Variable Group to control", CVGroupManager::getInstance());
    group->targetType = TargetParameter::CONTAINER;
    group->customGetTargetContainerFunc = &CVGroupManager::showMenuAndGetGroup;
    group->defaultContainerTypeCheckFunc = [](ControllableContainer* cc) { return dynamic_cast<CVGroup*>(cc) != nullptr; };
    basePreset = addTargetParameter("Base Preset", "Background values before, between and after blocks", CVGroupManager::getInstance());
    CVValuesHelpers::presetSelector(basePreset);
}

CVValuesValue::CVValuesValue(Parameter* parameter, const String& type) : BaseItem(parameter ? parameter->niceName : "Value", false, false)
{
    itemDataType = "CVValuesOverride";
    saveAndLoadRecursiveData = true;
    userCanRemove = false;
    userCanDuplicate = false;
    nameCanBeChangedByUser = false;
    source = addTargetParameter("Source", "Variable represented by this override", CVGroupManager::getInstance());
    source->hideInEditor = true;
    value = dynamic_cast<Parameter*>(ControllableFactory::createControllable(parameter ? parameter->getTypeString() : type));
    if (!value) value = new FloatParameter("Value", "", 0);
    value->setNiceName("Value");
    value->forceSaveValue = true;
    value->saveValueOnly = false;
    value->userCanSetReadOnly = false;
    value->lockManualControlMode = true;
    addParameter(value);
    overrideValue = addBoolParameter("Override", "Enable this block's custom value", false);
    unsetBehavior = addEnumParameter("When Unset", "What to do when Override is disabled");
    unsetBehavior->addOption("Take last set value", 0)->addOption("Do not change", 1);
    interpolation = addEnumParameter("Transition", "How this value transitions from another block");
    if (value->type == Controllable::FLOAT || value->type == Controllable::INT || value->isComplex())
        interpolation->addOption("Interpolate", ParameterPreset::INTERPOLATE);
    interpolation->addOption("Change at start", ParameterPreset::CHANGE_AT_START)->addOption("Change at end", ParameterPreset::CHANGE_AT_END);
    animated = addBoolParameter("Animated", "Use block-local automation or a color gradient", false);
    animated->hideInEditor = value->type != Controllable::FLOAT && value->type != Controllable::INT && value->type != Controllable::COLOR;
    if (value->type == Controllable::FLOAT || value->type == Controllable::INT)
    {
        automation.reset(new Automation("Animation", nullptr, true));
        automation->selectItemWhenCreated = false;
        automation->allowKeysOutside = true;
        automation->length->setValue(10);
        if (parameter && parameter->hasRange()) automation->valueRange->setPoint(parameter->minimumValue, parameter->maximumValue);
        else automation->valueRange->setEnabled(false);
        automation->addKey(0, parameter ? parameter->floatValue() : 0, false);
        automation->addKey(10, parameter ? parameter->floatValue() : 0, false);
        addChildControllableContainer(automation.get());
    }
    else if (value->type == Controllable::COLOR)
    {
        gradient.reset(new GradientColorManager(10, false, false));
        gradient->setNiceName("Animation");
        gradient->selectItemWhenCreated = false;
        gradient->setAllowKeysOutside(true);
        const Colour c = parameter ? static_cast<ColorParameter*>(parameter)->getColor() : Colours::black;
        gradient->addColorAt(0, c); gradient->addColorAt(10, c);
        addChildControllableContainer(gradient.get());
    }
    if (parameter)
    {
        source->setValueFromTarget(parameter);
        syncMetadata(parameter);
        value->setValue(parameter->value.clone());
    }
    onContainerParameterChangedInternal(animated);
}

void CVValuesValue::syncMetadata(Parameter* p)
{
    if (!p || value->type != p->type) return;
    if (niceName != p->niceName) setNiceName(p->niceName);
    if (p->hasRange()) value->setRange(p->minimumValue, p->maximumValue);
    else value->clearRange();
    if (automation)
    {
        automation->valueRange->setEnabled(p->hasRange());
        if (p->hasRange()) automation->valueRange->setPoint(p->minimumValue, p->maximumValue);
    }
    if (auto* from = dynamic_cast<EnumParameter*>(p))
    {
        auto* to = static_cast<EnumParameter*>(value);
        Array<EnumParameter::EnumValue> options;
        bool changed = from->enumValues.size() != to->enumValues.size();
        for (int i = 0; i < from->enumValues.size(); ++i)
        {
            const auto& option = *from->enumValues[i];
            options.add(option);
            if (i >= to->enumValues.size() || option.key != to->enumValues[i]->key
                || option.value != to->enumValues[i]->value) changed = true;
        }
        if (changed) to->setOptions(options);
    }
}

void CVValuesValue::setLength(float length, bool stretch, bool stickToEnd)
{
    if (automation) automation->setLength(length, stretch, stickToEnd);
    if (gradient) gradient->setLength(length, stretch, stickToEnd);
}

var CVValuesValue::getJSONData(bool includeNonOverriden)
{
    var data = BaseItem::getJSONData(includeNonOverriden);
    data.getDynamicObject()->setProperty("valueType", value->getTypeString());
    return data;
}

void CVValuesValue::onContainerParameterChangedInternal(Parameter*)
{
    if (!animated || !overrideValue) return;
    bool changed = false;
    auto visibility = [&changed](bool& hidden, bool next) { changed |= hidden != next; hidden = next; };
    if (automation) visibility(automation->hideInEditor, !animated->boolValue() || !overrideValue->boolValue());
    if (gradient) visibility(gradient->hideInEditor, !animated->boolValue() || !overrideValue->boolValue());
    visibility(value->hideInEditor, animated->boolValue() || !overrideValue->boolValue());
    visibility(unsetBehavior->hideInEditor, overrideValue->boolValue());
    if (changed) notifyStructureChanged();
}

CVValuesValueManager::CVValuesValueManager() : BaseManager("Custom Values")
{
    itemDataType = "CVValuesOverride";
    userCanAddItemsManually = false;
    selectItemWhenCreated = false;
}

CVValuesValue* CVValuesValueManager::createItemFromData(var data)
{
    return new CVValuesValue(nullptr, data.getProperty("valueType", "Float").toString());
}

CVValuesBlockGroup::CVValuesBlockGroup(CVValuesLayer* layer) : BaseItem("Group", false, false)
{
    saveAndLoadRecursiveData = true;
    userCanRemove = false; userCanDuplicate = false;
    target = addTargetParameter("Target", "Layer target represented by this block", layer ? &layer->targets : nullptr);
    target->targetType = TargetParameter::CONTAINER;
    target->hideInEditor = true;
    preset = addTargetParameter("Preset", "Preset for this target group; unset inherits", CVGroupManager::getInstance());
    CVValuesHelpers::presetSelector(preset);
    addChildControllableContainer(&values);
}

void CVValuesBlockGroup::sync(CVValuesTarget* row, float length, bool presetBlock)
{
    auto* group = row->group->getTargetContainerAs<CVGroup>();
    if (!group) return;
    if (niceName != group->niceName) setNiceName(group->niceName);
    CVValuesHelpers::presetSelector(preset, group);
    const bool changed = preset->hideInEditor != !presetBlock || values.hideInEditor != presetBlock;
    preset->hideInEditor = !presetBlock; values.hideInEditor = presetBlock;
    if (changed) notifyStructureChanged();
    Array<CVValuesValue*> visibleValues;
    for (auto* item : group->values.items)
    {
        auto* p = dynamic_cast<Parameter*>(item->controllable);
        if (!p) continue;
        CVValuesValue* override = nullptr;
        const String address = p->getControlAddress(CVGroupManager::getInstance());
        for (auto* v : values.items)
        {
            if (v->value->type != p->type) continue;
            if (v->source->getTargetParameter() == p || (!v->source->getTargetParameter() && v->source->ghostValue == address))
            {
                override = v;
                if (!v->source->getTargetParameter()) v->source->setValueFromTarget(p);
                break;
            }
        }
        if (!override)
        {
            override = new CVValuesValue(p);
            values.addItem(override, var(), false);
            override->setLength(length, true);
        }
        else override->syncMetadata(p);
        visibleValues.add(override);
        override->onContainerParameterChangedInternal(override->animated);
    }
    bool visibilityChanged = false;
    for (auto* v : values.items)
    {
        const bool hidden = !visibleValues.contains(v);
        if (v->hideInEditor != hidden) { v->hideInEditor = hidden; visibilityChanged = true; }
    }
    if (visibilityChanged) notifyStructureChanged();
}

CVValuesBlock::CVValuesBlock(CVValuesLayer* owner) : LayerBlock("Preset Block"), layer(owner), groups("Groups")
{
    itemDataType = "CVValuesBlock";
    saveAndLoadRecursiveData = true;
    blockType = addEnumParameter("Block Type", "Preset references or custom values");
    blockType->addOption("Preset Block", 0)->addOption("Custom Values", 1);
    loopLength->hideInEditor = true;
    isActive->isSavable = false;
    loopLength->setControllableFeedbackOnly(true);
    groups.userCanAddItemsManually = false;
    groups.selectItemWhenCreated = false;
    groups.customCreateItemFunc = [owner] { return new CVValuesBlockGroup(owner); };
    addChildControllableContainer(&groups);
}

void CVValuesBlock::syncTargets()
{
    bool changed = false;
    for (auto* g : groups.items)
    {
        const bool missing = !layer->targets.items.contains(dynamic_cast<CVValuesTarget*>(g->target->getTargetContainer()));
        if (g->hideInEditor != missing) { g->hideInEditor = missing; changed = true; }
    }
    for (auto* row : layer->targets.items)
    {
        if (!row->group->getTargetContainerAs<CVGroup>()) continue;
        CVValuesBlockGroup* entry = nullptr;
        for (auto* g : groups.items) if (g->target->getTargetContainer() == row) { entry = g; break; }
        if (!entry)
        {
            entry = new CVValuesBlockGroup(layer);
            groups.addItem(entry, var(), false);
            entry->target->setValueFromTarget(row);
        }
        entry->sync(row, coreLength->floatValue(), blockType->getValueDataAsEnum<int>() == 0);
    }
    if (changed) notifyStructureChanged();
}

bool CVValuesBlock::setTiming(double start, double length, bool stretch)
{
    if (!std::isfinite(start) || !std::isfinite(length) || start < 0 || length < .1
        || (enabled->boolValue() && !layer->blocks.accepts(this, start, length)))
    { setWarningMessage("Edit rejected: only ordinary two-block crossfades are supported.", "edit", false); return false; }
    {
        const ScopedValueSetter<bool> guard(validating, true);
        time->setValue(start); coreLength->setValue(length);
        for (auto* g : groups.items) for (auto* v : g->values.items) v->setLength(static_cast<float>(length), stretch, false);
        acceptedStart = time->doubleValue(); acceptedLength = coreLength->doubleValue();
        acceptedEnabled = enabled->boolValue();
    }
    clearWarning("edit"); layer->refresh(); return true;
}

void CVValuesBlock::setCoreLength(float length, bool stretch, bool)
{
    setTiming(time->doubleValue(), length, stretch);
}

void CVValuesBlock::setStartTime(float start, bool keepEnd, bool stickToEnd)
{
    const double length = keepEnd ? time->doubleValue() + coreLength->doubleValue() - start : coreLength->doubleValue();
    ignoreUnused(stickToEnd);
    setTiming(start, length);
}

void CVValuesBlock::onContainerParameterChangedInternal(Parameter* p)
{
    if (validating || isCurrentlyLoadingData || !layer || !blockType) return;
    if (p == isActive) return;
    if (p == loopLength && loopLength->doubleValue() != 0)
    { validating = true; loopLength->setValue(0); validating = false; }
    if ((p == time || p == coreLength || p == enabled) && layer->blocks.items.contains(this))
    {
        if (enabled->boolValue() && !layer->blocks.accepts(this, time->doubleValue(), coreLength->doubleValue()))
        {
            validating = true;
            time->setValue(acceptedStart); coreLength->setValue(acceptedLength);
            enabled->setValue(acceptedEnabled);
            validating = false;
            setWarningMessage("Edit rejected: only ordinary two-block crossfades are supported.", "edit", false);
            return;
        }
        acceptedStart = time->doubleValue(); acceptedLength = coreLength->doubleValue();
        acceptedEnabled = enabled->boolValue();
        if (p == coreLength)
            for (auto* g : groups.items) for (auto* v : g->values.items) v->setLength(coreLength->floatValue());
        clearWarning("edit");
    }
    layer->refresh();
}

void CVValuesBlock::afterLoadJSONDataInternal()
{
    acceptedStart = time->doubleValue(); acceptedLength = coreLength->doubleValue();
    acceptedEnabled = enabled->boolValue();
    loopLength->setValue(0);
    layer->refresh();
}

CVValuesBlockManager::CVValuesBlockManager(CVValuesLayer* layer) : LayerBlockManager(layer), cvLayer(layer)
{
    itemDataType = "CVValuesBlock";
}

LayerBlock* CVValuesBlockManager::createItem() { return new CVValuesBlock(cvLayer); }

LayerBlock* CVValuesBlockManager::addItem(LayerBlock* item, var data, bool addToUndo, bool notify)
{
    std::unique_ptr<LayerBlock> proposed(item ? item : createItem());
    if (!isCurrentlyLoadingData && !cvLayer->isCurrentlyLoadingData && !Engine::mainEngine->isLoadingFile
        && !UndoMaster::getInstance()->isPerforming)
    {
        const double start = data.isVoid() ? proposed->time->doubleValue() : CVValuesHelpers::storedNumber(data, "startTime", 0);
        const double length = data.isVoid() ? proposed->coreLength->doubleValue() : CVValuesHelpers::storedNumber(data, "length", 10);
        if (!accepts(nullptr, start, length))
        { cvLayer->setWarningMessage("Block creation rejected: only ordinary two-block crossfades are supported.", "add", false); return nullptr; }
    }
    cvLayer->clearWarning("add");
    return LayerBlockManager::addItem(proposed.release(), data, addToUndo, notify);
}

LayerBlock* CVValuesBlockManager::addItemFromData(var data, bool addToUndo)
{
    return addItem(createItemFromData(data), data, addToUndo);
}

UndoableAction* CVValuesBlockManager::stateAction(var before, var after, bool alreadyApplied)
{
    class StateAction : public UndoableAction
    {
    public:
        StateAction(CVValuesLayer* l, var a, var b, bool applied)
            : owner(l), before(a.clone()), after(b.clone()), skipFirst(applied)
        { for (auto* block : l->blocks.items) if (block->isSelected) selection.add(block->shortName); }
        WeakReference<ControllableContainer> owner;
        var before, after;
        bool skipFirst;
        StringArray selection;
        bool apply(const var& data)
        {
            auto* layer = dynamic_cast<CVValuesLayer*>(owner.get());
            if (!layer) return false;
            layer->blocks.loadJSONData(data.clone());
            for (const auto& name : selection) if (auto* block = layer->blocks.getItemWithName(name)) block->selectThis(true, false);
            layer->rebuildSnapshot();
            if (auto* sequence = dynamic_cast<ChataigneSequence*>(layer->sequence)) sequence->evaluateCVValues();
            return true;
        }
        bool perform() override { if (skipFirst) { skipFirst = false; return true; } return apply(after); }
        bool undo() override { return apply(before); }
    };
    return new StateAction(cvLayer, before, after, alreadyApplied);
}

bool CVValuesBlockManager::accepts(const CVValuesBlock* block, double start, double length) const
{
    std::vector<CVValuesEvaluation::Range> ranges;
    for (auto* item : items)
    {
        auto* b = static_cast<CVValuesBlock*>(item);
        if (b == block || !b->enabled->boolValue() || !b->valid) continue;
        ranges.push_back({ b->time->doubleValue(), b->time->doubleValue() + b->coreLength->doubleValue() });
    }
    ranges.push_back({ start, start + length });
    std::sort(ranges.begin(), ranges.end(), [](auto a, auto b) { return a.start < b.start; });
    return CVValuesEvaluation::validRanges(ranges);
}

bool CVValuesBlockManager::applyTimings(const std::vector<Timing>& timings, bool stretch)
{
    std::vector<CVValuesEvaluation::Range> ranges;
    for (auto* item : items)
    {
        auto* b = static_cast<CVValuesBlock*>(item);
        if (!b->enabled->boolValue()) continue;
        auto proposal = std::find_if(timings.begin(), timings.end(), [b](const Timing& t) { return t.block == b; });
        if (proposal != timings.end()) ranges.push_back({ proposal->start, proposal->start + proposal->length });
        else if (b->valid) ranges.push_back({ b->time->doubleValue(), b->time->doubleValue() + b->coreLength->doubleValue() });
    }
    std::sort(ranges.begin(), ranges.end(), [](auto a, auto b) { return a.start < b.start; });
    if (!CVValuesEvaluation::validRanges(ranges))
    { cvLayer->setWarningMessage("Edit rejected: only ordinary two-block crossfades are supported.", "edit", false); return false; }
    for (const auto& t : timings)
    {
        if (!std::isfinite(t.start) || !std::isfinite(t.length) || t.start < 0 || t.length < .1) return false;
    }
    for (const auto& t : timings)
    {
        const ScopedValueSetter<bool> guard(t.block->validating, true);
        t.block->time->setValue(t.start); t.block->coreLength->setValue(t.length);
        for (auto* g : t.block->groups.items) for (auto* v : g->values.items) v->setLength(static_cast<float>(t.length), stretch);
        t.block->acceptedStart = t.start; t.block->acceptedLength = t.length;
    }
    cvLayer->clearWarning("edit"); cvLayer->refresh(); return true;
}

void CVValuesBlockManager::askForPlaceBlockTime(LayerBlock* item, float time)
{
    auto* block = static_cast<CVValuesBlock*>(item);
    if (accepts(block, time, block->coreLength->doubleValue())) block->time->setValue(time);
}

void CVValuesBlockManager::askForDuplicateItem(BaseItem* item)
{
    auto* original = dynamic_cast<CVValuesBlock*>(item);
    if (!original) return;
    const double start = original->time->doubleValue() + original->coreLength->doubleValue();
    if (!accepts(nullptr, start, original->coreLength->doubleValue()))
    { cvLayer->setWarningMessage("Duplicate rejected: there is no room directly after this block.", "duplicate", false); return; }
    var data = original->getJSONData();
    data.getDynamicObject()->setProperty("index", items.indexOf(original) + 1);
    CVValuesHelpers::storeNumber(data, "startTime", start);
    cvLayer->clearWarning("duplicate");
    addItemFromData(data);
}

Array<LayerBlock*> CVValuesBlockManager::addItemsFromClipboard(bool showWarning)
{
    // Validate the complete proposed paste before constructing undo actions.
    var data = JSON::parse(SystemClipboard::getTextFromClipboard());
    var list = data.getProperty("items", var());
    if (data.getProperty("itemType", "").toString() != itemDataType) return {};
    if (!list.isArray()) list = var(Array<var>{ data });
    if (list.size() == 0) return {};
    std::vector<CVValuesEvaluation::Range> ranges;
    for (auto* b : items) if (b->enabled->boolValue() && static_cast<CVValuesBlock*>(b)->valid)
        ranges.push_back({ b->time->doubleValue(), b->time->doubleValue() + b->coreLength->doubleValue() });
    double earliest = std::numeric_limits<double>::max();
    for (auto d : *list.getArray()) earliest = std::min(earliest, CVValuesHelpers::storedNumber(d, "startTime", 0));
    const double offset = cvLayer->sequence->currentTime->doubleValue() - earliest;
    for (auto d : *list.getArray())
    {
        const double start = CVValuesHelpers::storedNumber(d, "startTime", 0) + offset;
        if (CVValuesHelpers::storedNumber(d, "enabled", 1) != 0)
            ranges.push_back({ start, start + CVValuesHelpers::storedNumber(d, "length", 10) });
        CVValuesHelpers::storeNumber(d, "startTime", start);
    }
    std::sort(ranges.begin(), ranges.end(), [](auto a, auto b) { return a.start < b.start; });
    if (!CVValuesEvaluation::validRanges(ranges))
    {
        if (showWarning) cvLayer->setWarningMessage("Paste rejected: overlapping blocks must form ordinary two-block crossfades.", "paste", false);
        return {};
    }
    cvLayer->clearWarning("paste");
    // Offset belongs in the serialized undo action, rather than a post-paste move.
    return addItemsFromData(list);
}

CVValuesLayer::CVValuesLayer(Sequence* sequence, var) : SequenceLayer(sequence, getTypeString()),
    targets("Targets"), blocks(this), interpolationCurve("Interpolation Curve")
{
    saveAndLoadRecursiveData = true;
    targets.itemDataType = "CVValuesTarget";
    targets.customCreateItemFunc = [] { return new CVValuesTarget(); };
    targets.selectItemWhenCreated = false;
    targets.addBaseManagerListener(this);
    blocks.addBaseManagerListener(this);
    addChildControllableContainer(&targets);
    interpolationMode = addEnumParameter("Interpolation Mode", "Values in empty spaces between blocks");
    interpolationMode->addOption("Base Preset", 0)->addOption("Interpolate", 1)->addOption("Hold", 2);
    fadeIn = addFloatParameter("Fade In", "Transition duration at block beginnings", 1, 0);
    fadeOut = addFloatParameter("Fade Out", "Transition duration at block endings", 1, 0);
    fadeIn->defaultUI = fadeOut->defaultUI = FloatParameter::TIME;
    interpolationCurve.selectItemWhenCreated = false;
    interpolationCurve.length->setValue(1);
    interpolationCurve.length->setControllableFeedbackOnly(true);
    interpolationCurve.addKey(0, 0, false)->easingType->setValueWithData(Easing::LINEAR);
    interpolationCurve.addKey(1, 1, false)->easingType->setValueWithData(Easing::LINEAR);
    interpolationCurve.editorIsCollapsed = true;
    addChildControllableContainer(&interpolationCurve);
    addChildControllableContainer(&blocks);
    CVGroupManager::getInstance()->addControllableContainerListener(this);
    refresh();
}

CVValuesLayer::~CVValuesLayer()
{
    cancelPendingUpdate();
    releaseMetadataListeners();
    CVGroupManager::getInstance()->removeControllableContainerListener(this);
    targets.removeBaseManagerListener(this); blocks.removeBaseManagerListener(this);
}

void CVValuesLayer::refresh()
{
    if (isClearing || isBeingDestroyed) return;
    snapshotDirty.store(true);
    if (!rebuilding) triggerAsyncUpdate();
}

void CVValuesLayer::handleAsyncUpdate()
{
    if (isCurrentlyLoadingData || Engine::mainEngine->isLoadingFile || isClearing) return;
    rebuildSnapshot();
    if (auto* s = dynamic_cast<ChataigneSequence*>(sequence)) s->evaluateCVValues();
}

void CVValuesLayer::rebuildSnapshot()
{
    if (isClearing || isCurrentlyLoadingData || rebuilding.exchange(true)) return;
    struct Guard { std::atomic<bool>& flag; ~Guard() { flag.store(false); } } guard{ rebuilding };
    snapshotDirty.store(false);
    releaseMetadataListeners();
    auto result = std::make_shared<Snapshot>();
    auto& timeline = result->timeline;
    timeline.gapMode = static_cast<CVValuesEvaluation::GapMode>(interpolationMode->getValueDataAsEnum<int>());
    timeline.fadeIn = fadeIn->doubleValue(); timeline.fadeOut = fadeOut->doubleValue();
    auto curve = CVValuesHelpers::copyAutomation(interpolationCurve);
    timeline.curve = [curve](double t) { return CVValuesHelpers::sampleAutomation(*curve, t); };
    std::vector<CVValuesTarget*> rows;
    std::vector<Controllable::Type> types;
    for (auto* row : targets.items)
    {
        auto* group = row->group->getTargetContainerAs<CVGroup>();
        auto* base = row->basePreset->getTargetContainerAs<CVPreset>();
        CVValuesHelpers::presetSelector(row->basePreset, group);
        const bool compatible = group && base && base->group == group && group->controlMode->getValueDataAsEnum<CVGroup::ControlMode>() == CVGroup::FREE;
        row->setWarningMessage(compatible ? String() : "Select a group in Free mode and a Base Preset from that group.", "target", false);
        if (!compatible || !row->enabled->boolValue() || !group->enabled->boolValue()) continue;
        for (auto* item : group->values.items)
        {
            auto* p = dynamic_cast<Parameter*>(item->controllable);
            if (!p) continue;
            if (!metadataParameters.contains(p))
            {
                metadataParameters.add(p); p->addParameterListener(this);
                if (auto* enumeration = dynamic_cast<EnumParameter*>(p)) enumeration->addEnumParameterListener(this);
            }
            auto* pp = base->values.getParameterPresetForSource(p);
            timeline.base.push_back((pp && pp->hasTimelineValue ? pp->parameter->value : p->defaultValue).clone());
            result->parameters.emplace_back(p); result->groups.emplace_back(group);
            result->targets.emplace_back(row); result->bases.emplace_back(base);
            rows.push_back(row); types.push_back(p->type);
        }
    }
    timeline.interpolate = [types](size_t ch, const var& a, const var& b, double w)
    { return CVValuesHelpers::interpolate(types[ch], a, b, w); };
    Array<CVValuesBlock*> sorted;
    for (auto* item : blocks.items) sorted.add(static_cast<CVValuesBlock*>(item));
    std::stable_sort(sorted.begin(), sorted.end(), [](auto* a, auto* b) { return a->time->doubleValue() < b->time->doubleValue(); });
    std::vector<CVValuesEvaluation::Range> accepted;
    for (auto* block : sorted)
    {
        block->syncTargets();
        if (!block->enabled->boolValue()) continue;
        auto proposed = accepted;
        proposed.push_back({ block->time->doubleValue(), block->time->doubleValue() + block->coreLength->doubleValue() });
        block->valid = CVValuesEvaluation::validRanges(proposed);
        block->setWarningMessage(block->valid ? String() : "Invalid overlap: this block is excluded from evaluation. Move or resize it to form a two-block crossfade.", "overlap", false);
        if (!block->valid) continue;
        accepted = proposed;
        block->acceptedStart = block->time->doubleValue(); block->acceptedLength = block->coreLength->doubleValue();
        block->acceptedEnabled = block->enabled->boolValue();
        CVValuesEvaluation::Block<var> b;
        b.start = block->acceptedStart; b.length = block->acceptedLength;
        for (size_t ch = 0; ch < rows.size(); ++ch)
        {
            CVValuesEvaluation::Channel<var> c;
            CVValuesBlockGroup* entry = nullptr;
            for (auto* g : block->groups.items) if (g->target->getTargetContainer() == rows[ch]) { entry = g; break; }
            auto* parameter = result->parameters[ch].get();
            if (entry && block->blockType->getValueDataAsEnum<int>() == 0)
            {
                auto* p = entry->preset->getTargetContainerAs<CVPreset>();
                if (p && p->group == result->groups[ch].get())
                {
                    if (auto* pp = p->values.getParameterPresetForSource(parameter))
                    {
                        auto m = pp->interpolationMode->getValueDataAsEnum<ParameterPreset::InterpolationMode>();
                        c.source = m == ParameterPreset::NONE ? CVValuesEvaluation::Source::NoWrite : CVValuesEvaluation::Source::Value;
                        c.constant = { (pp->hasTimelineValue ? pp->parameter->value : timeline.base[ch]).clone(), true, CVValuesHelpers::mode(m) };
                    }
                    else { c.source = CVValuesEvaluation::Source::Value; c.constant.value = timeline.base[ch]; }
                    entry->clearWarning("preset");
                }
                else if (entry->preset->stringValue().isNotEmpty() || entry->preset->ghostValue.isNotEmpty())
                {
                    c.source = CVValuesEvaluation::Source::NoWrite;
                    entry->setWarningMessage("Preset is missing or belongs to another CV Group.", "preset", false);
                }
                else entry->clearWarning("preset");
            }
            else if (entry)
            {
                for (auto* v : entry->values.items)
                {
                    if (v->source->getTargetParameter() != parameter || v->value->type != parameter->type) continue;
                    if (!v->overrideValue->boolValue())
                        c.source = v->unsetBehavior->getValueDataAsEnum<int>() == 0 ? CVValuesEvaluation::Source::Inherit : CVValuesEvaluation::Source::NoWrite;
                    else
                    {
                        c.source = CVValuesEvaluation::Source::Value;
                        c.constant = { v->value->value.clone(), true, CVValuesHelpers::mode(v->interpolation->getValueDataAsEnum<ParameterPreset::InterpolationMode>()) };
                        if (v->animated->boolValue() && v->automation)
                        {
                            auto a = CVValuesHelpers::copyAutomation(*v->automation);
                            const bool integer = parameter->type == Controllable::INT;
                            c.animation = [a, integer](double t) -> var { const double n = CVValuesHelpers::sampleAutomation(*a, t); return integer ? var(roundToInt(n)) : var(n); };
                        }
                        else if (v->animated->boolValue() && v->gradient)
                        {
                            auto g = std::make_shared<GradientColorManager>(static_cast<float>(b.length), false, false);
                            g->selectItemWhenCreated = false; g->setAllowKeysOutside(true);
                            g->loadJSONData(v->gradient->getJSONData());
                            c.animation = [g](double t) { return CVValuesHelpers::colorValue(g->getColorForPosition(static_cast<float>(t))); };
                        }
                    }
                    break;
                }
            }
            b.channels.push_back(std::move(c));
        }
        timeline.blocks.push_back(std::move(b));
    }
    std::atomic_store(&snapshot, std::shared_ptr<const Snapshot>(result));
    if (snapshotDirty.load()) triggerAsyncUpdate();
}

Array<CVValuesLayer::Output> CVValuesLayer::evaluate(double time) const
{
    Array<Output> output;
    if (!enabled->boolValue() || !sequence->enabled->boolValue()) return output;
    auto data = std::atomic_load(&snapshot);
    if (!data) return output;
    auto samples = data->timeline.evaluate(time);
    for (size_t ch = 0; ch < samples.size(); ++ch)
    {
        auto* group = dynamic_cast<CVGroup*>(data->groups[ch].get());
        auto* row = dynamic_cast<CVValuesTarget*>(data->targets[ch].get());
        if (samples[ch].write && data->parameters[ch] && group && group->enabled->boolValue()
            && row && row->enabled->boolValue() && row->group->getTargetContainer() == group
            && data->bases[ch] && row->basePreset->getTargetContainer() == data->bases[ch].get()
            && group->controlMode->getValueDataAsEnum<CVGroup::ControlMode>() == CVGroup::FREE)
            output.add({ data->parameters[ch], data->groups[ch], samples[ch].value });
    }
    return output;
}

void CVValuesLayer::updateActiveBlocks(double time)
{
    const ScopedLock lock(blocks.items.getLock());
    for (auto* item : blocks.items)
    {
        auto* b = static_cast<CVValuesBlock*>(item);
        b->isActive->setValue(enabled->boolValue() && b->enabled->boolValue() && b->valid
            && b->time->doubleValue() <= time && time < b->time->doubleValue() + b->coreLength->doubleValue());
    }
}

void CVValuesLayer::onContainerParameterChangedInternal(Parameter*) { refresh(); }
void CVValuesLayer::onControllableFeedbackUpdateInternal(ControllableContainer*, Controllable* c)
{
    if (c->shortName != "isActive") refresh();
}

void CVValuesLayer::onControllableFeedbackUpdate(ControllableContainer* cc, Controllable* c)
{
    if (CVGroupManager::getInstance()->containsControllable(c))
    {
        // Live CV output feedback is deliberately not an authored-data edit.
        for (auto* group : CVGroupManager::getInstance()->items)
            if (group->pm->containsControllable(c) || c == group->controlMode || c == group->enabled) { refresh(); break; }
    }
    else SequenceLayer::onControllableFeedbackUpdate(cc, c);
}

void CVValuesLayer::childStructureChanged(ControllableContainer* cc)
{
    SequenceLayer::childStructureChanged(cc); refresh();
}
void CVValuesLayer::childAddressChanged(ControllableContainer* cc)
{
    SequenceLayer::childAddressChanged(cc); refresh();
}
void CVValuesLayer::afterLoadJSONDataInternal() { refresh(); }
void CVValuesLayer::clearItem()
{
    cancelPendingUpdate();
    releaseMetadataListeners();
    std::atomic_store(&snapshot, std::shared_ptr<const Snapshot>());
    blocks.clear(); targets.clear(); SequenceLayer::clearItem();
}
void CVValuesLayer::releaseMetadataListeners()
{
    for (auto p : metadataParameters) if (p)
    {
        p->removeParameterListener(this);
        if (auto* enumeration = dynamic_cast<EnumParameter*>(p.get())) enumeration->removeEnumParameterListener(this);
    }
    metadataParameters.clear();
}
void CVValuesLayer::selectAll(bool add) { blocks.askForSelectAllItems(add); setSelected(false); }
bool CVValuesLayer::paste()
{
    const auto data = JSON::parse(SystemClipboard::getTextFromClipboard());
    if (data.getProperty("itemType", "").toString() == blocks.itemDataType) { blocks.askForPaste(); return true; }
    return SequenceLayer::paste();
}
void CVValuesLayer::getSnapTimes(Array<float>* times) { blocks.getSnapTimes(times); }
Array<Inspectable*> CVValuesLayer::selectAllItemsBetweenInternal(float start, float end)
{
    Array<Inspectable*> result;
    for (auto* b : blocks.getBlocksInRange(start, end)) result.add(b);
    return result;
}
Array<UndoableAction*> CVValuesLayer::getRemoveAllItemsBetweenInternal(float start, float end)
{ return blocks.getRemoveItemsUndoableAction(blocks.getBlocksInRange(start, end)); }
Array<UndoableAction*> CVValuesLayer::getInsertTimespanInternal(float start, float length)
{
    var before = blocks.getJSONData(), after = before.clone();
    if (auto* items = after.getProperty("items", var()).getArray()) for (auto data : *items)
    {
        const double time = CVValuesHelpers::storedNumber(data, "startTime", 0);
        if (time >= start) CVValuesHelpers::storeNumber(data, "startTime", time + length);
    }
    return { blocks.stateAction(before, after) };
}
Array<UndoableAction*> CVValuesLayer::getRemoveTimespanInternal(float start, float end)
{
    var before = blocks.getJSONData(), after = before.clone();
    var list;
    if (auto* items = after.getProperty("items", var()).getArray()) for (auto data : *items)
    {
        const double time = CVValuesHelpers::storedNumber(data, "startTime", 0);
        const double length = CVValuesHelpers::storedNumber(data, "length", 10);
        if (time < end && time + length > start) continue;
        if (time >= end) CVValuesHelpers::storeNumber(data, "startTime", time - (end - start));
        list.append(data);
    }
    after.getDynamicObject()->setProperty("items", list);
    return { blocks.stateAction(before, after) };
}
