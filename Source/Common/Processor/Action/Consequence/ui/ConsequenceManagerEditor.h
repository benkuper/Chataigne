/*
  ==============================================================================

	ConsequenceManagerEditor.h
	Created: 6 Oct 2021 6:43:49pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConsequenceManagerEditor :
	public BaseCommandHandlerManagerEditor<BaseItem>
{
public:
	ConsequenceManagerEditor(ConsequenceManager* csm, CommandContext context, bool isRoot, bool multiplexMode);
	~ConsequenceManagerEditor();

	ConsequenceManager* csm;
	virtual void showMenuAndAddItem(bool /*isFromAddButton*/) override;
};