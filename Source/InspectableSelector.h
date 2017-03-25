/*
  ==============================================================================

    InspectableSelector.h
    Created: 25 Mar 2017 4:13:37pm
    Author:  Ben

  ==============================================================================
*/

#ifndef INSPECTABLESELECTOR_H_INCLUDED
#define INSPECTABLESELECTOR_H_INCLUDED

#include "Inspectable.h"

class InspectableSelector : 
	public Component
{
public:
	juce_DeclareSingleton(InspectableSelector, true);
	InspectableSelector();
	~InspectableSelector();

	Component * selectablesParent;
	Inspector * targetInspector;

	Array<Component *> selectables;
	Array<Inspectable * > inspectables;

	Rectangle<int> selectionBounds;

	void startSelection(Component * parent, Array<Component *> selectables, bool clearSelection = true);
	void startSelection(Component * parent, Array<Component *> selectables, Array<Inspectable *> relatedInspectables, bool clearSelection = true); //If custom UI needed, use that function to provide different set of ui and inspectables (must be the same size !!)
	void endSelection(bool confirmSelection = true);

	void paint(Graphics &g) override;

	void mouseDrag(const MouseEvent &e) override;
	void mouseUp(const MouseEvent &e) override;

	/*
	class  SelectorListener
	{
	public:
		virtual ~SelectorListener() {}
		virtual void selection() {};
	};

	ListenerList<SelectorListener> listeners;
	void addSelectorListener(SelectorListener* newListener) { listeners.add(newListener); }
	void removeSelectorListener(SelectorListener* listener) { listeners.remove(listener); }
	*/
};



#endif  // INSPECTABLESELECTOR_H_INCLUDED
