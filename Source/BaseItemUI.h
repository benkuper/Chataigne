/*
  ==============================================================================

    BaseItemUI.h
    Created: 28 Oct 2016 8:04:09pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef BASEITEMUI_H_INCLUDED
#define BASEITEMUI_H_INCLUDED


#include "BaseItemMinimalUI.h"
#include "AssetManager.h"
#include "BoolImageToggleUI.h"
#include "StringParameterUI.h"

template<class T>
class BaseItemUI : 
	public BaseItemMinimalUI<T>,
	public ButtonListener
{
public:
	BaseItemUI<T>(T * _item);
	virtual ~BaseItemUI<T>();

	//layout
	int headerHeight;
	int headerGap;


	ScopedPointer<StringParameterUI> nameUI;
	ScopedPointer<BoolImageToggleUI> enabledBT;
	ScopedPointer<ImageButton> removeBT;

	virtual void resized() override;

	void buttonClicked(Button *b) override;
	virtual void buttonClickedInternal(Button *){} //override this in child classes


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseItemUI<T>)


};



template<class T>
BaseItemUI<T>::BaseItemUI(T * _item) :
	BaseItemMinimalUI<T>(_item),
	headerHeight(16), headerGap(5)
{
	
	nameUI = this->getBaseItem()->nameParam->createStringParameterUI();
	this->addAndMakeVisible(nameUI);

	if (this->getBaseItem()->canBeDisabled)
	{
		enabledBT = this->getBaseItem()->enabled->createImageToggle(AssetManager::getInstance()->getPowerBT());
		this->addAndMakeVisible(enabledBT);
	}
	
	removeBT = AssetManager::getInstance()->getRemoveBT();
	this->addAndMakeVisible(removeBT);
	removeBT->addListener(this);

	setHighlightOnMouseOver(true);
	this->setSize(100, headerHeight+4);
}

template<class T>
BaseItemUI<T>::~BaseItemUI()
{
	removeBT->removeListener(this);
}

template<class T>
void BaseItemUI<T>::resized()
{
	//Header
	Rectangle<int> r = this->getLocalBounds().reduced(2);
	Rectangle<int> h = r.withHeight(headerHeight);
	if(enabledBT != nullptr)
	{
		enabledBT->setBounds(h.removeFromLeft(h.getHeight()));
		h.removeFromLeft(2);
	}

	removeBT->setBounds(h.removeFromRight(h.getHeight()));
	h.removeFromRight(2);
	
	nameUI->setBounds(h);
}

template<class T>
void BaseItemUI<T>::buttonClicked(Button * b)
{
	if (b == removeBT)
	{
		this->getBaseItem()->remove();
		return;
	}

	buttonClickedInternal(b);
}





#endif  // BASEITEMUI_H_INCLUDED
