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
#include "BoolToggleUI.h"
#include "StringParameterUI.h"

template<class T>
class BaseItemUI : 
	public InspectableComponent,
	public ControllableContainerListener,
	public ButtonListener
{
public:
	BaseItemUI<T>(T * _item);
	virtual ~BaseItemUI<T>();

	T * item;

	//layout
	int headerHeight;
	int headerGap;

	ScopedPointer<StringParameterUI> nameUI;
	ScopedPointer<BoolToggleUI> enabledBT;
	ScopedPointer<ImageButton> removeBT;

	virtual void paint(Graphics &g) override;
	virtual void resized() override;

	void buttonClicked(Button *b) override;
	virtual void buttonClickedInternal(Button *){} //override this in child classes

	void controllableFeedbackUpdate(ControllableContainer *, Controllable *) override;
	virtual void controllableFeedbackUpdateInternal(Controllable *) {} //override this in child classes

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseItemUI<T>)

private:
	BaseItem * getBaseItem() { return static_cast<BaseItem *>(item); }
};



template<class T>
BaseItemUI<T>::BaseItemUI(T * _item) :
	InspectableComponent(_item),
	item(_item),
	headerHeight(16), headerGap(5)
{
	addMouseListener(this, true);

	getBaseItem()->addControllableContainerListener(this);
	
	nameUI = getBaseItem()->nameParam->createStringParameterUI();
	addAndMakeVisible(nameUI);

	enabledBT = getBaseItem()->enabled->createToggle();
	addAndMakeVisible(enabledBT);

	removeBT = AssetManager::getInstance()->getRemoveBT();
	addAndMakeVisible(removeBT);
	removeBT->addListener(this);

	setSize(100, headerHeight+4);
}

template<class T>
BaseItemUI<T>::~BaseItemUI()
{
	getBaseItem()->removeControllableContainerListener(this);
	removeBT->removeListener(this);
}


template<class T>
void BaseItemUI<T>::paint(Graphics &g)
{
	Rectangle<float> r = getLocalBounds().toFloat();
	g.setColour(getBaseItem()->enabled->boolValue()?BG_COLOR.brighter(.1f):BG_COLOR.darker(.1f));
	g.fillRoundedRectangle(r, 4);
}

template<class T>
void BaseItemUI<T>::resized()
{
	//Header
	Rectangle<int> r = getLocalBounds().reduced(2);
	Rectangle<int> h = r.withHeight(headerHeight);
	enabledBT->setBounds(h.removeFromLeft(h.getHeight()));
	h.removeFromLeft(2);
	removeBT->setBounds(h.removeFromRight(h.getHeight()));
	h.removeFromRight(2);
	nameUI->setBounds(h);
}

template<class T>
void BaseItemUI<T>::buttonClicked(Button * b)
{
	if (b == removeBT)
	{
		getBaseItem()->remove();
		return;
	}

	buttonClickedInternal(b);
	
}

template<class T>
inline void BaseItemUI<T>::controllableFeedbackUpdate(ControllableContainer *, Controllable * c)
{
	if (c == getBaseItem()->enabled) repaint();
	controllableFeedbackUpdateInternal(c);
}



#endif  // BASEITEMUI_H_INCLUDED
