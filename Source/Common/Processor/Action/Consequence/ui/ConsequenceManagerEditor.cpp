/*
  ==============================================================================

	ConsequenceManagerEditor.cpp
	Created: 6 Oct 2021 6:43:49pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"
#include "ConsequenceManagerEditor.h"

ConsequenceManagerEditor::ConsequenceManagerEditor(ConsequenceManager* csm, CommandContext context, bool isRoot, bool multiplexMode) :
	BaseCommandHandlerManagerEditor(csm, context, isRoot, multiplexMode),
	csm(csm)
{
	triggerUI.reset(csm->triggerPreview->createButtonUI());
	if (csm->isMultiplexed()) triggerUI->customLabel = "Trigger (" + String(csm->getPreviewIndex() + 1) + ")";
	addAndMakeVisible(triggerUI.get());

	for (auto& f : csm->factory.defs) acceptedDropTypes.add(f->type);

	csm->addAsyncManagerListener(this);
}

ConsequenceManagerEditor::~ConsequenceManagerEditor()
{
	if (!inspectable.wasObjectDeleted()) csm->removeAsyncManagerListener(this);
}

void ConsequenceManagerEditor::resizedInternalHeader(Rectangle<int>& r)
{
	BaseCommandHandlerManagerEditor::resizedInternalHeader(r);
	r.removeFromRight(2);
	triggerUI->setBounds(r.removeFromRight(100).reduced(2));
}

void ConsequenceManagerEditor::showMenuAndAddItem(bool)
{
	PopupMenu p = CommandFactory::getCommandMenu(context, nullptr, multiplexMode);
	p.addSeparator();
	p.addItem(-100000, "Group");

	p.showMenuAsync(PopupMenu::Options(), [this](int result)
		{

			if (result == -100000)
			{
				ConsequenceGroup* g = new ConsequenceGroup(var(), csm->multiplex);
				csm->addItem(g);

			}
			else if (CommandDefinition* def = CommandFactory::getCommandFromResult(result, nullptr))
			{
				Consequence* c = new Consequence(var(), csm->multiplex);
				c->setCommand(def);
				csm->addItem(c);
				setCollapsed(false, true);
			}
		}
	);
}

void ConsequenceManagerEditor::newMessage(const ConsequenceManager::ConsequenceManagerEvent& e)
{
	switch (e.type)
	{
	case ConsequenceManager::ConsequenceManagerEvent::MULTIPLEX_PREVIEW_CHANGED:
		if (csm->isMultiplexed()) triggerUI->customLabel = "Trigger (" + String(csm->getPreviewIndex() + 1) + ")";
		break;
	}
}
