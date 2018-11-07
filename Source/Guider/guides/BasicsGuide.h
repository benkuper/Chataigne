/*
  ==============================================================================

    BasicsGuide.h
    Created: 7 Nov 2018 2:57:23pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../BaseGuide.h"
#include "Module/ModuleManager.h"
#include "StateMachine/ui/StateMachineView.h"
#include "Common/Processor/Action/Condition/conditions/StandardCondition/StandardCondition.h"
#include "Common/Processor/Action/Condition/conditions/StandardCondition/ui/StandardConditionEditor.h"
#include "Common/Command/ui/BaseCommandHandlerManagerEditor.h"

class AudioModule;
class ResolumeModule;
class ModuleManagerUI;

class BasicsGuide :
	public BaseGuide,
	public ModuleManager::AsyncListener,
	public StateMachineView::ManagerUIListener,
	public ConditionManager::AsyncListener,
	public StandardCondition::AsyncListener,
	public ConsequenceManager::AsyncListener,
	public BaseCommandHandler::CommandHandlerListener,
	public Inspector::InspectorListener,
	public GenericControllableContainerEditor::ContainerEditorListener
{
public:
	BasicsGuide();
	~BasicsGuide();

	enum Steps { NOTHING, AUDIO_MODULE, RESOLUME_MODULE, STATE, ACTION, CONDITION, CONDITION_SOURCE, CONSEQUENCE };

	AudioModule * audioModule;
	ResolumeModule * resolumeModule;
	State * state;
	Action * action;
	StandardCondition * condition;
	Consequence * consequence; 

	Inspector * inspector;
	StateMachineView * smui;
	ModuleManagerUI * mmui; 
	StateViewUI * svui;
	GenericManagerEditor<Condition> * cme;
	BaseCommandHandlerManagerEditor<Consequence> * csme;
	StandardConditionEditor * ce;
	BaseCommandHandler * command;
	

	void clear() override;
	void handleStep(int step) override;

	void newMessage(const ModuleManager::ManagerEvent &e) override;
	void newMessage(const ConditionManager::ManagerEvent &e) override;
	void newMessage(const ConsequenceManager::ManagerEvent &e) override;
	void newMessage(const StandardCondition::ConditionEvent &e) override;

	void commandChanged(BaseCommandHandler * bch) override;
	void currentInspectableChanged(Inspector *) override;

	void itemUIAdded(StateViewUI * svui) override;

	void containerRebuilt(GenericControllableContainerEditor * editor);

	static BasicsGuide * create(var) { return new BasicsGuide(); }
};