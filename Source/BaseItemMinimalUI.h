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
    BaseItem * baseItem;
    
	//ui
	Colour bgColor;
	bool highlightOnMouseOver;

	bool removeOnCtrlDown;
	bool removeOnDelKey;

	void mouseDown(const MouseEvent &e) override;
	void mouseExit(const MouseEvent &e) override;
	bool keyPressed(const KeyPress &e) override;

	void setHighlightOnMouseOver(bool highlight);

	void paint(Graphics &g) override;
	void newMessage(const ContainerAsyncEvent &e) override;

	//void controllableFeedbackUpdate(ControllableContainer *, Controllable *) override;
	virtual void controllableFeedbackUpdateInternal(Controllable *) {} //override this in child classes
	

};





template<class T>
BaseItemMinimalUI<T>::BaseItemMinimalUI(T * _item) :
	InspectableContentComponent(_item),
	item(_item),
	bgColor(BG_COLOR.brighter(.1f)),
	highlightOnMouseOver(false),
	removeOnCtrlDown(false),
	removeOnDelKey(true)
{

    baseItem = static_cast<BaseItem *>(item);
    
	setWantsKeyboardFocus(true);

	addMouseListener(this, true);
	baseItem->addAsyncContainerListener(this);

	setSize(100, 20);
}

template<class T>
BaseItemMinimalUI<T>::~BaseItemMinimalUI()
{
	baseItem->removeAsyncContainerListener(this);
}


template<class T>
void BaseItemMinimalUI<T>::mouseDown(const MouseEvent & e)
{
	InspectableContentComponent::mouseDown(e);
	if (removeOnCtrlDown && e.mods.isLeftButtonDown() && e.mods.isCtrlDown()) baseItem->remove();
}

template<class T>
void BaseItemMinimalUI<T>::mouseExit(const MouseEvent &e)
{
	InspectableContentComponent::mouseExit(e);
	repaint();
}

template<class T>
bool BaseItemMinimalUI<T>::keyPressed(const KeyPress & e)
{
	DBG("Key pressed here ! " << baseItem->niceName);

	if (removeOnDelKey && (e.getKeyCode() == e.deleteKey || e.getKeyCode() == e.backspaceKey) && inspectable->isSelected)
	{
		baseItem->remove();
		return true;
	}
	
	return false;
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
	bool isItemEnabled = baseItem->canBeDisabled ? baseItem->enabled->boolValue() : true;

	Colour c = isItemEnabled ? bgColor : bgColor.darker(.3f);
	if (highlightOnMouseOver && isMouseOver(true)) c = c.brighter(.03f);
	g.setColour(c);
	g.fillRoundedRectangle(r, 4);
}

template<class T>
void BaseItemMinimalUI<T>::newMessage(const ContainerAsyncEvent & e)
{
	if (e.type == ContainerAsyncEvent::ControllableFeedbackUpdate)
	{
		if (e.targetControllable == baseItem->enabled) repaint();
		controllableFeedbackUpdateInternal(e.targetControllable);
	}
}


#endif  // BASEITEMMINIMALUI_H_INCLUDED
