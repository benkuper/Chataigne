/*
  ==============================================================================

    CommandChooserUI.h
    Created: 4 Nov 2016 11:23:37am
    Author:  bkupe

  ==============================================================================
*/

#ifndef COMMANDTARGETUI_H_INCLUDED
#define COMMANDTARGETUI_H_INCLUDED

#include "Module/ModuleManager.h"

class CommandChooserUI :
	public Component,
	public Button::Listener
{
public:
	CommandChooserUI(CommandContext context);
	virtual ~CommandChooserUI();

	CommandContext context;

	Module * lockedModule;

	bool commandIsSet;
	String noTargetText;
	Label label;
	ScopedPointer<ImageButton> targetBT;

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



#endif  // COMMANDTARGETUI_H_INCLUDED
