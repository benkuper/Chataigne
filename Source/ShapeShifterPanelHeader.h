/*
  ==============================================================================

    ShapeShifterPanelHeader.h
    Created: 3 May 2016 2:25:55pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SHAPESHIFTERPANELHEADER_H_INCLUDED
#define SHAPESHIFTERPANELHEADER_H_INCLUDED

#include "ShapeShifterPanelTab.h"


class ShapeShifterPanelHeader :
	public Component,
	public ShapeShifterPanelTab::TabListener
{
public:
	ShapeShifterPanelHeader();
	virtual ~ShapeShifterPanelHeader();

	OwnedArray<ShapeShifterPanelTab> tabs;

	void addTab(ShapeShifterContent * content);
	void removeTab(ShapeShifterPanelTab * tab, bool doRemove = true);
	void attachTab(ShapeShifterPanelTab * tab);

	ShapeShifterPanelTab * getTabForContent(ShapeShifterContent * content);


	void mouseDown(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;

	void paint(Graphics &g) override;
	void resized()override;

	void askForRemoveTab(ShapeShifterPanelTab *) override;

	class Listener
	{
	public:
        virtual ~Listener(){}
		virtual void tabDrag(ShapeShifterPanelTab *) = 0;
		virtual void tabSelect(ShapeShifterPanelTab *) = 0;

		virtual void askForRemoveTab(ShapeShifterPanelTab *) = 0;
		virtual void headerDrag() = 0;


	};

	ListenerList<Listener> listeners;
	void addHeaderListener(Listener* newListener) { listeners.add(newListener); }
	void removeHeaderListener(Listener* listener) { listeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ShapeShifterPanelHeader)
};


#endif  // SHAPESHIFTERPANELHEADER_H_INCLUDED
