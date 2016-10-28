/*
  ==============================================================================

    BaseItemUI.h
    Created: 28 Oct 2016 8:04:09pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef BASEITEMUI_H_INCLUDED
#define BASEITEMUI_H_INCLUDED

#include "InspectableComponent.h"
#include "BaseItem.h"
#include "AssetManager.h"
#include "Style.h"

template<class T>
class BaseItemUI : 
	public InspectableComponent,
	public ButtonListener
{
public:
	BaseItemUI<T>(T * _item);
	virtual ~BaseItemUI<T>();

	ScopedPointer<ImageButton> removeBT;

	void mouseDown(const MouseEvent &e);
	void paint(Graphics &g) override;
	void resized() override;

	T * item;

	void buttonClicked(Button *b) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseItemUI<T>)
};



template<class T>
BaseItemUI<T>::BaseItemUI(T * _item) :
	InspectableComponent(_item),
	item(_item)
{
	removeBT = AssetManager::getInstance()->getRemoveBT();
	addAndMakeVisible(removeBT);
	removeBT->addListener(this);
}

template<class T>
BaseItemUI<T>::~BaseItemUI()
{
	removeBT->removeListener(this);
}

template<class T>
inline void BaseItemUI<T>::mouseDown(const MouseEvent & e)
{
	if(e.mods.isLeftButtonDown()) selectThis();
}

template<class T>
void BaseItemUI<T>::paint(Graphics &g)
{
	Rectangle<float> r = getLocalBounds().toFloat();
	g.setColour(BG_COLOR.brighter(.1f));
	g.fillRoundedRectangle(r, 2);
	g.setColour(isSelected ? HIGHLIGHT_COLOR : BG_COLOR.brighter(.2f));
	g.drawRoundedRectangle(r, 2, 2);
}

template<class T>
void BaseItemUI<T>::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(2);
	removeBT->setBounds(r.removeFromRight(r.getHeight()));
}

template<class T>
void BaseItemUI<T>::buttonClicked(Button * b)
{
	if (b == removeBT) static_cast<BaseItem *>(item)->remove();
}



#endif  // BASEITEMUI_H_INCLUDED
