/*
  ==============================================================================

	ConsequenceManagerEditor.cpp
	Created: 6 Oct 2021 6:43:49pm
	Author:  bkupe

  ==============================================================================
*/

ConsequenceManagerEditor::ConsequenceManagerEditor(ConsequenceManager* csm, CommandContext context, bool isRoot, bool multiplexMode) :
	BaseCommandHandlerManagerEditor(csm, context, isRoot, multiplexMode),
	csm(csm)
{
}

ConsequenceManagerEditor::~ConsequenceManagerEditor()
{
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
