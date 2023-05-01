/*
  ==============================================================================

	ConsequenceManagerEditor.h
	Created: 6 Oct 2021 6:43:49pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConsequenceManagerEditor :
	public BaseCommandHandlerManagerEditor<BaseItem>,
	public ConsequenceManager::AsyncListener
{
public:
	ConsequenceManagerEditor(ConsequenceManager* csm, CommandContext context, bool isRoot, bool multiplexMode);
	~ConsequenceManagerEditor();

	ConsequenceManager* csm;

	std::unique_ptr<TriggerUI> triggerUI;

	virtual void resizedInternalHeader(Rectangle<int>& r) override;
	
	virtual void showMenuAndAddItem(bool /*isFromAddButton*/) override;

	virtual void newMessage(const ConsequenceManager::ConsequenceManagerEvent& e) override;
};