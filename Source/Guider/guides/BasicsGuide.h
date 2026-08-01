/*
  ==============================================================================

    BasicsGuide.h
    Created: 7 Nov 2018 2:57:23pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Module/ModuleIncludes.h"
#include "StateMachine/StateMachineIncludes.h"
#include "Common/Processor/ProcessorIncludes.h"

#include "Common/Command/ui/BaseCommandHandlerManagerEditor.h"
#include "Common/Command/ui/BaseCommandHandlerEditor.h"

class BasicsGuide :
	public BaseGuide,
	public ModuleManager::AsyncListener,
	public StateMachineView::ManagerUIListener,
	public StandardCondition::AsyncListener,
	public Inspector::InspectorListener,
	public GenericControllableContainerEditor::ContainerEditorListener,
	public Parameter::AsyncListener
{
public:
	BasicsGuide();
	~BasicsGuide();

	enum Steps { 
		INTRO, 
		OBSERVE_MODULES, 
		ADD_KBD_MOD, 
		OBSERVE_INSPECTOR,
		ADD_AUDIO_MOD,
		OBSERVE_INSPECTOR_2,
		ADD_STATE,
		ADD_ACTION,
		ADD_CONDITION,
		SELECT_CONDITION_SOURCE,
		EDIT_CONDITION,
		OBSERVE_CONDITION,
		ADD_CONSEQUENCE,
		EDIT_CONSEQUENCE,
		END,
		STEPS_MAX };

	KeyboardModule * keyboardModule;
	AudioModule * audioModule;
	State * state;
	StandardCondition * condition;
	WeakReference<Inspectable> conditionRef;
	WeakReference<Parameter> conditionReference;
	WeakReference<Parameter> audioFile;

	Component::SafePointer<Inspector> inspector;
	Component::SafePointer<StateMachineView> smui;
	Component::SafePointer<ModuleManagerUI> mmui;
	Component::SafePointer<StateViewUI> svui;
	Component::SafePointer<GenericManagerEditor<Condition>> cme;
	Component::SafePointer<ConsequenceManagerEditor> csme;
	Component::SafePointer<StandardConditionEditor> ce;
	Component::SafePointer<BaseCommandHandlerEditor> cse;

	void initInternal() override;
	void clear() override;
	void handleStep(int step) override;

	void newMessage(const ModuleManager::ManagerEvent &e) override;
	void newMessage(const StandardCondition::ConditionEvent &e) override;
	void newMessage(const Parameter::ParameterEvent &e) override;

	void currentInspectableChanged(Inspector *) override;

	void itemUIAdded(StateViewUI * svui) override;

	void containerRebuilt(GenericControllableContainerEditor * editor) override;

	static BasicsGuide * create(var) { return new BasicsGuide(); }
};
