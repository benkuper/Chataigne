/*
  ==============================================================================

    BaseItemMinimalUI.h
    Created: 20 Nov 2016 2:48:09pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef BASEITEMMINIMALUI_H_INCLUDED
#define BASEITEMMINIMALUI_H_INCLUDED

#include "InspectableContentComponent.h"
#include "BaseItem.h"
#include "Style.h"

template<class T>
class BaseItemMinimalUI :
	public InspectableContentComponent,
	public ControllableContainer::ContainerAsyncListener
{
public:
	BaseItemMinimalUI<T>(T * _item);
	virtual ~BaseItemMinimalUI<T>();

	T * item;
	
	//ui
	Colour bgColor;
	bool highlightOnMouseOver;

	void setHighlightOnMouseOver(bool highlight);

	void paint(Graphics &g) override;

	void newMessage(const ContainerAsyncEvent &e) override;

	//void controllableFeedbackUpdate(ControllableContainer *, Controllable *) override;
	virtual void controllableFeedbackUpdateInternal(Controllable *) {} //override this in child classes
	

protected:
	BaseItem * getBaseItem() { return static_cast<BaseItem *>(item); }
};





template<class T>
BaseItemMinimalUI<T>::BaseItemMinimalUI(T * _item) :
	InspectableContentComponent(_item),
	item(_item),
	bgColor(BG_COLOR.brighter(.1f)),
	highlightOnMouseOver(false)
{
	addMouseListener(this, true);
	getBaseItem()->addAsyncContainerListener(this);

	setSize(100, 20);
}

template<class T>
BaseItemMinimalUI<T>::~BaseItemMinimalUI()
{
	getBaseItem()->removeAsyncContainerListener(this);
}


template<class T>
void BaseItemMinimalUI<T>::setHighlightOnMouseOver(bool highlight)
{
	setRepaintsOnMouseActivity(highlight);
	highlightOnMouseOver = highlight;
}

template<class T>
void BaseItemMinimalUI<T>::paint(Graphics &g)
{
	Rectangle<float> r = getLocalBounds().toFloat();
	Colour c = getBaseItem()->enabled->boolValue() ? bgColor : bgColor.darker(.3f);
	if (highlightOnMouseOver && isMouseOver(true)) c = c.brighter(.03f);
	g.setColour(c);
	g.fillRoundedRectangle(r, 4);
}

template<class T>
void BaseItemMinimalUI<T>::newMessage(const ContainerAsyncEvent & e)
{
	if (e.type == ContainerAsyncEvent::ControllableFeedbackUpdate)
	{
		if (e.targetControllable == getBaseItem()->enabled) repaint();
		controllableFeedbackUpdateInternal(e.targetControllable);
	}
}


#endif  // BASEITEMMINIMALUI_H_INCLUDED
