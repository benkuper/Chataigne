/*
  ==============================================================================

    InspectableContent.h
    Created: 30 Oct 2016 9:08:27am
    Author:  bkupe

  ==============================================================================
*/

#ifndef INSPECTABLECONTENT_H_INCLUDED
#define INSPECTABLECONTENT_H_INCLUDED

#include "Inspectable.h"

class InspectableContent :
	public Inspectable::InspectableListener
{
public:
	InspectableContent(Inspectable * inspectable);
	virtual ~InspectableContent();

	WeakReference<Inspectable> inspectable;

	void inspectableSelectionChanged(Inspectable *) override;
	void inspectableDestroyed(Inspectable *) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InspectableContent)
};



#endif  // INSPECTABLECONTENT_H_INCLUDED
