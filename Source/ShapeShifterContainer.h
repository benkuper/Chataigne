/*
  ==============================================================================

    ShapeShifterContainer.h
    Created: 2 May 2016 3:11:25pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SHAPESHIFTERCONTAINER_H_INCLUDED
#define SHAPESHIFTERCONTAINER_H_INCLUDED

#include "ShapeShifterPanel.h"
#include "GapGrabber.h"


class ShapeShifterContainer;

class ShapeShifterContainerListener
{
public:
	virtual ~ShapeShifterContainerListener() {}
	virtual void panelAdded(ShapeShifterContainer *) {}
	virtual void panelRemoved(ShapeShifterContainer *) {}

	virtual void containerAdded(ShapeShifterContainer *) {}
	virtual void containerRemoved(ShapeShifterContainer *) {}

	virtual void containerEmptied(ShapeShifterContainer *) {}
	virtual void oneShifterRemaining(ShapeShifterContainer *, ShapeShifter *) {}
};


//Listener
class ShapeShifterContainer : public ShapeShifter,
	public GapGrabber::Listener,
	public ShapeShifterPanel::Listener,
	public ShapeShifterContainerListener
{
public:
	enum Direction { NONE, HORIZONTAL, VERTICAL};
	enum Position { TOP, BOTTOM, LEFT ,RIGHT };

	ShapeShifterContainer(Direction _direction);
	virtual ~ShapeShifterContainer();

	Direction direction;
	OwnedArray<GapGrabber> grabbers;

	Array<ShapeShifter *> shifters;

	const int gap = 6;

	void resized() override;


	//Generic handling
	void insertShifterAt(ShapeShifter * shifter, int index, bool resizeAfter = true);
	void removeShifter(ShapeShifter * shifter, bool deleteShifter, bool silent = false, bool resizeAfter = true);

	ShapeShifterPanel * insertPanelAt(ShapeShifterPanel * panel, int index = -1, bool resizeAfter = true);
	ShapeShifterPanel * insertPanelRelative(ShapeShifterPanel * panel, ShapeShifterPanel * relativeTo, ShapeShifterPanel::AttachZone zone, bool resizeAfter = true);

	ShapeShifterContainer * insertContainerAt(ShapeShifterContainer * container, int index = -1, bool resizeAfter = true);

	void movePanelsInContainer(ShapeShifterPanel * containedPanel, ShapeShifterPanel * newPanel, Direction _newDir, bool secondBeforeFirst);

	bool isFlexible() override;

	void clear();

	virtual var getCurrentLayout() override;
	virtual void loadLayoutInternal(var layout) override;

	virtual void grabberGrabUpdate(GapGrabber * gg, int dist) override;
	virtual void panelDetach(ShapeShifterPanel *) override;
	virtual void panelEmptied(ShapeShifterPanel * panel) override;
	virtual void panelDestroyed(ShapeShifterPanel *) override;

	virtual void containerEmptied(ShapeShifterContainer *) override;
	virtual void oneShifterRemaining(ShapeShifterContainer * container, ShapeShifter * lastShifter) override;

	ListenerList<ShapeShifterContainerListener> containerListeners;
	void addShapeShifterContainerListener(ShapeShifterContainerListener* newListener) { containerListeners.add(newListener); }
	void removeShapeShifterContainerListener(ShapeShifterContainerListener* listener) { containerListeners.remove(listener); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ShapeShifterContainer)
};



#endif  // SHAPESHIFTERCONTAINER_H_INCLUDED
