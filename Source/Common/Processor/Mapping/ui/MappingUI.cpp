/*
  ==============================================================================

	MappingUI.cpp
	Created: 28 Oct 2016 8:06:18pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"
#include "StateMachine/StateMachineIncludes.h"
#include "CustomVariables/CustomVariablesIncludes.h"

MappingUI::MappingUI(Mapping* mapping) :
	ProcessorUI(mapping),
	mapping(mapping),
	outputParamUI(nullptr)
{
	acceptedDropTypes.add("Module");
	acceptedDropTypes.add("CommandTemplate");

	mapping->addAsyncMappingListener(this);
	updateBGColor();

	updateOutputParamUI();
}

MappingUI::~MappingUI()
{
	if (!inspectable.wasObjectDeleted()) mapping->removeAsyncMappingListener(this);
}

void MappingUI::paint(Graphics& g)
{
	BaseItemUI::paint(g);

	if (isDraggingOver)
	{
		g.setColour(BLUE_COLOR.darker());
		g.fillRoundedRectangle(getLocalBounds().toFloat(), 2);
	}
}

void MappingUI::updateOutputParamUI()
{
	if (inspectable == nullptr || inspectable.wasObjectDeleted()) return;

	if (outputParamUI != nullptr)
	{
		removeChildComponent(outputParamUI.get());
	}

	if (mapping->om.outParams.size() > 0)
	{

		//if (outputParamUI != nullptr && mapping->om.outParams.size() > 0 && outputParamUI->controllable == mapping->om.outParams[0][0]) return;

		int pi = mapping->getPreviewIndex();
		if (mapping->om.outParams.size() > pi)
		{
			Array<WeakReference<Parameter>> outs = mapping->om.outParams[pi];
			if (outs.size() > 0 && outs[0] != nullptr)
			{
				outputParamUI.reset(outs[0]->createDefaultUI());
				if (outputParamUI != nullptr)
				{
					outputParamUI->showLabel = false;
					addAndMakeVisible(outputParamUI.get());
				}
			}
		}
	}

	resized();
}

void MappingUI::resizedInternalHeader(Rectangle<int>& r)
{
	ProcessorUI::resizedInternalHeader(r);
	if (outputParamUI != nullptr)
	{
		outputParamUI->setBounds(r.removeFromRight(120).reduced(1));
	}
}

void MappingUI::itemDropped(const SourceDetails& details)
{
	BaseItemUI::itemDropped(details);

	String dataType = details.description.getProperty("dataType", "");
	CommandDefinition* def = nullptr;
	bool isInput = false;

	if (dataType == "Module")
	{
		ModuleUI* mui = dynamic_cast<ModuleUI*>(details.sourceComponent.get());

		PopupMenu pm;
		ControllableChooserPopupMenu mappingInputMenu(&mui->item->valuesCC, 10000, -1, StringArray(), StringArray(Trigger::getTypeStringStatic()));
		PopupMenu mappingCommandMenu = mui->item->getCommandMenu(20000, CommandContext::MAPPING);

		pm.addSubMenu("Input", mappingInputMenu);
		pm.addSubMenu("Output", mappingCommandMenu);

		pm.showMenuAsync(PopupMenu::Options(), [this, &mappingInputMenu, mui](int result)
			{
				if (result == 0) return;
				bool isInput = result < 20000;

				if (isInput)
				{
					Controllable* target = mappingInputMenu.getControllableForResult(result);
					StandardMappingInput* mi = new StandardMappingInput();
					mapping->im.addItem(mi);
					mi->inputTarget->setValueFromTarget(target);
				}
				else //command
				{
					if (CommandDefinition* def = mui->item->getCommandDefinitionForItemID(result - 1 - 20000))
					{
						MappingOutput* o = mapping->om.addItem();
						o->setCommand(def);
					}
				}
			}
		);
	}
	else if (dataType == "CommandTemplate")
	{
		BaseItemUI<CommandTemplate>* tui = dynamic_cast<BaseItemUI<CommandTemplate> *>(details.sourceComponent.get());
		if (tui != nullptr)
		{
			CommandTemplateManager* ctm = dynamic_cast<CommandTemplateManager*>(tui->item->parentContainer.get());
			if (ctm != nullptr) def = ctm->defManager->getCommandDefinitionFor(ctm->menuName, tui->item->niceName);
		}
	}

	if (!isInput && def != nullptr)
	{
		MappingOutput* o = mapping->om.addItem();
		o->setCommand(def);
	}
}


void MappingUI::newMessage(const Mapping::MappingEvent& e)
{
	switch (e.type)
	{
	case Mapping::MappingEvent::OUTPUT_TYPE_CHANGED:
		updateOutputParamUI();
		break;
	}
}

void MappingUI::addContextMenuItems(PopupMenu& p)
{
	p.addItem(100, "Copy inputs");
	p.addItem(101, "Paste inputs");

	p.addItem(102, "Copy filters");
	p.addItem(103, "Paste filters");

	p.addItem(104, "Copy outputs");
	p.addItem(105, "Paste outputs");
}

void MappingUI::handleContextMenuResult(int result)
{
	switch (result)
	{
	case 100: SystemClipboard::copyTextToClipboard(JSON::toString(mapping->im.getJSONData())); break;
	case 101: mapping->im.loadJSONData(JSON::fromString(SystemClipboard::getTextFromClipboard())); break;
	case 102: SystemClipboard::copyTextToClipboard(JSON::toString(mapping->fm.getJSONData())); break;
	case 103: mapping->fm.loadJSONData(JSON::fromString(SystemClipboard::getTextFromClipboard())); break;
	case 104: SystemClipboard::copyTextToClipboard(JSON::toString(mapping->om.getJSONData())); break;
	case 105: mapping->om.loadJSONData(JSON::fromString(SystemClipboard::getTextFromClipboard())); break;
	}
}
