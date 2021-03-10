/*
  ==============================================================================

    CommandChooserUI.h
    Created: 4 Nov 2016 11:23:37am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../CommandContext.h"

class Module;
class CommandDefinition;

class CommandChooserUI :
	public Component,
	public Button::Listener
{
public:
	CommandChooserUI(CommandContext context, bool multiplexMode = false);
	virtual ~CommandChooserUI();

	CommandContext context;
	bool multiplexMode;

	Module * lockedModule;

	bool commandIsSet;
	String noTargetText;
	Label label;
	std::unique_ptr<ImageButton> targetBT;

	void paint(Graphics &g) override;
	void resized() override;

	void setLabel(const String &text);

	virtual void showPopupAndGetDefinition();//can be overriden to get specific PopupMenu
	void mouseDown(const MouseEvent &e) override;

	virtual void buttonClicked(Button * b) override;

	class Listener
	{
	public:
		virtual ~Listener() {}
		virtual void definitionChosen(CommandDefinition *) {}
	};

	ListenerList<Listener> chooserListeners;
	void addChooserListener(Listener* newListener) { chooserListeners.add(newListener); }
	void removeChooserListener(Listener* listener) { chooserListeners.remove(listener); }



protected:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CommandChooserUI)
};