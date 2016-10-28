/*
  ==============================================================================

    ShapeShifterPanelTab.h
    Created: 3 May 2016 12:43:01pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SHAPESHIFTERPANELTAB_H_INCLUDED
#define SHAPESHIFTERPANELTAB_H_INCLUDED

#include "ShapeShifterContent.h"//keep
class ShapeShifterPanelTab :
	public Component,
	public ButtonListener
{
public:
	ShapeShifterPanelTab(ShapeShifterContent * _content);
	~ShapeShifterPanelTab();

	ShapeShifterContent * content;
	Label panelLabel;
	ImageButton closePanelBT;

	bool selected;
	void setSelected(bool value);

	void paint(Graphics &g) override;
	void resized()override;

	int getLabelWidth();

	void buttonClicked(Button * b) override;

	//Listener
	class TabListener
	{
	public:
		virtual ~TabListener() {};
		virtual void askForRemoveTab(ShapeShifterPanelTab *) {};
	};

	ListenerList<TabListener> tabListeners;
	void addShapeShifterTabListener(TabListener* newListener) { tabListeners.add(newListener); }
	void removeShapeShifterTabListener(TabListener* listener) { tabListeners.remove(listener); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ShapeShifterPanelTab)
};


#endif  // SHAPESHIFTERPANELTAB_H_INCLUDED
