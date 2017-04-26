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

template<class T>
class BaseItemUI :
	public BaseItemMinimalUI<T>,
	public ButtonListener
{
public:
	enum ResizeMode { NONE, VERTICAL, HORIZONTAL, ALL };

	BaseItemUI<T>(T * _item, ResizeMode resizeMode = VERTICAL, bool canBeDragged = false);
	virtual ~BaseItemUI<T>();

	
	//LAYOUT
    const int margin = 2;
    int minContentHeight;
    
    


	//grabber
	int grabberHeight;
	Point<float> posAtMouseDown;
    bool canBeDragged;

	//header
	int headerHeight;
	int headerGap;
    
    //Resize
    ResizeMode resizeMode;
    int resizerWidth;
    int resizerHeight;
    
    ScopedPointer<ResizableCornerComponent> cornerResizer;
    ScopedPointer<ResizableEdgeComponent> edgeResizer;
    Component * resizer;
    
    

	ScopedPointer<StringParameterUI> nameUI;
	ScopedPointer<BoolImageToggleUI> enabledBT;
	ScopedPointer<ImageButton> removeBT;

	void setContentSize(int contentWidth, int contentHeight);

	//minimode
	int getHeightWithoutContent();

	virtual void updateMiniModeUI();

	virtual void resized() override;
	virtual void resizedInternalHeader(Rectangle<int> &) {}
	virtual void resizedInternalContent(Rectangle<int> &) {}
	void buttonClicked(Button *b) override;


	void mouseDown(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;
	void mouseDoubleClick(const MouseEvent &e) override;

	void controllableFeedbackUpdateInternal(Controllable *) override;

	class Grabber : public Component
	{
	public:
		Grabber() {}
		~Grabber() {}
		void paint(Graphics &g) override;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Grabber)
	};

	ScopedPointer<Grabber> grabber;

	class ItemUIListener
	{
	public:
		virtual ~ItemUIListener() {}
		virtual void itemUIGrabbed(BaseItemUI<T> *) {}
		virtual void itemUIMiniModeChanged(BaseItemUI<T> *) {}
	};

	ListenerList<ItemUIListener> itemUIListeners;
	void addItemUIListener(ItemUIListener* newListener) { itemUIListeners.add(newListener); }
	void removeItemUIListener(ItemUIListener* listener) { itemUIListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseItemUI<T>)

    friend class BaseItemMinimalUI<T>;

};



#include "BoolImageToggleUI.h"
#include "StringParameterUI.h"


template<class T>
BaseItemUI<T>::BaseItemUI(T * _item, ResizeMode _resizeMode, bool _canBeDragged) :
	BaseItemMinimalUI<T>(_item),
grabberHeight(0),
canBeDragged(_canBeDragged),
headerHeight(16),
headerGap(2),
resizeMode(_resizeMode),
resizerWidth(0),
resizerHeight(0),
resizer(nullptr)
{
    
	nameUI = this->baseItem->nameParam->createStringParameterUI();
	this->addAndMakeVisible(nameUI);

	if (this->baseItem->canBeDisabled)
	{
		enabledBT = this->baseItem->enabled->createImageToggle(AssetManager::getInstance()->getPowerBT());
		this->addAndMakeVisible(enabledBT);
	}

	removeBT = AssetManager::getInstance()->getRemoveBT();
	this->addAndMakeVisible(removeBT);
	removeBT->addListener(this);

	this->setHighlightOnMouseOver(true);


	if (canBeDragged)
	{
		grabber = new Grabber();
		grabber->setAlwaysOnTop(true);
		this->addAndMakeVisible(grabber);
		grabberHeight = 10;
	}

	switch (resizeMode)
	{
	case NONE:
		break;

	case VERTICAL:
		edgeResizer = new ResizableEdgeComponent(this, nullptr, ResizableEdgeComponent::bottomEdge);
		edgeResizer->setAlwaysOnTop(true);
		this->addAndMakeVisible(edgeResizer);
		resizerHeight = 4;
		//resizer = edgeResizer;
		//setContentSize(getWidth(),(int)item->listUISize->floatValue());
		break;

	case HORIZONTAL:
		edgeResizer = new ResizableEdgeComponent(this, nullptr, ResizableEdgeComponent::rightEdge);
		edgeResizer->setAlwaysOnTop(true);
		this->addAndMakeVisible(edgeResizer);
		resizerWidth = 4;
		//resizer = edgeResizer;
		//setContentSize((int)item->listUISize->floatValue(),getHeight());
		break;

	case ALL:
		cornerResizer = new ResizableCornerComponent(this, nullptr);
		cornerResizer->setAlwaysOnTop(true);
		this->addAndMakeVisible(cornerResizer);
		resizerHeight = 10;
		//resizer = cornerResizer;
		//setContentSize((int)item->viewUISize->getPoint().x, (int)item->viewUISize->getPoint().y);
		break;
	}

	updateMiniModeUI();
}

template<class T>
BaseItemUI<T>::~BaseItemUI()
{
	removeBT->removeListener(this);
}

template<class T>
void BaseItemUI<T>::setContentSize(int contentWidth, int contentHeight)
{
	int targetHeight = getHeightWithoutContent() + contentHeight;

	int targetWidth = contentWidth + margin * 2 + resizerWidth;

	this->setSize(targetWidth, targetHeight);
}

template<class T>
int BaseItemUI<T>::getHeightWithoutContent()
{
	return grabberHeight + headerHeight + margin * 2 + (this->baseItem->miniMode->boolValue() ? 0 : headerGap + resizerHeight);
}

template<class T>
void BaseItemUI<T>::updateMiniModeUI()
{
	if (this->baseItem->miniMode->boolValue())
	{
		if (resizer != nullptr) this->removeChildComponent(resizer);

		int targetHeight = getHeightWithoutContent();
		this->setSize(this->getWidth(), targetHeight);
	} else
	{
		if (resizer != nullptr) this->addAndMakeVisible(resizer);

		int targetHeight = 0;
		int targetWidth = this->getWidth();

		switch (resizeMode)
		{
		case ALL:
			targetWidth = (int)this->baseItem->viewUISize->getPoint().x;
			targetHeight = (int)this->baseItem->viewUISize->getPoint().y;
			break;
		case VERTICAL:
			targetHeight = (int)this->baseItem->listUISize->floatValue();
			break;
		case HORIZONTAL:
			targetWidth = (int)this->baseItem->listUISize->floatValue();
			break;
		case NONE:
			break;
		}

		setContentSize(targetWidth, targetHeight);
	}

	itemUIListeners.call(&ItemUIListener::itemUIMiniModeChanged, this);
}

template<class T>
void BaseItemUI<T>::resized()
{
	if (this->getHeight() < getHeightWithoutContent() + minContentHeight)
	{
		this->setSize(this->getWidth(), getHeightWithoutContent() + minContentHeight);
		return;
	}

	//Header
	Rectangle<int> r = this->getLocalBounds().reduced(margin);

	//Grabber
	if (canBeDragged)
	{
		//Grabber
		grabber->setBounds(r.removeFromTop(grabberHeight));
		grabber->repaint();
	}

	Rectangle<int> h = r.removeFromTop(headerHeight);

	if (enabledBT != nullptr)
	{
		enabledBT->setBounds(h.removeFromLeft(h.getHeight()));
		h.removeFromLeft(2);
	}

	removeBT->setBounds(h.removeFromRight(h.getHeight()));
	h.removeFromRight(2);

	resizedInternalHeader(h);
	nameUI->setBounds(h);

	r.removeFromTop(headerGap);

	if (!this->baseItem->miniMode->boolValue())
	{
		switch (resizeMode)
		{
		case NONE:
			break;

		case VERTICAL:
			edgeResizer->setBounds(r.removeFromBottom(resizerHeight));
			this->baseItem->listUISize->setValue((float)r.getHeight());
			break;

		case HORIZONTAL:
			edgeResizer->setBounds(r.removeFromRight(resizerWidth));
			this->baseItem->listUISize->setValue((float)r.getWidth());
			break;

		case ALL:
			cornerResizer->setBounds(r.removeFromBottom(resizerHeight).withLeft(r.getWidth() - resizerHeight));
			this->baseItem->viewUISize->setPoint((float)r.getWidth(), (float)r.getHeight());
			break;

		}

		resizedInternalContent(r);
	}
}

template<class T>
void BaseItemUI<T>::buttonClicked(Button * b)
{
	if (b == removeBT)
	{
		this->baseItem->remove();
		return;
	}
}


template<class T>
void BaseItemUI<T>::mouseDown(const MouseEvent & e)
{
	BaseItemMinimalUI<T>::mouseDown(e);

	if (e.mods.isLeftButtonDown())
	{
		if (canBeDragged && e.eventComponent == grabber) posAtMouseDown = this->baseItem->viewUIPosition->getPoint();
	}
}


template<class T>
void BaseItemUI<T>::mouseDrag(const MouseEvent & e)
{
	BaseItemMinimalUI<T>::mouseDrag(e);

	if (canBeDragged && e.mods.isLeftButtonDown() && e.eventComponent == grabber)
	{
		Point<float> targetPos = posAtMouseDown + e.getOffsetFromDragStart().toFloat() / Point<float>((float)this->getParentComponent()->getWidth(), (float)this->getParentComponent()->getHeight());
		this->baseItem->viewUIPosition->setPoint(targetPos);
	}

}

template<class T>
void BaseItemUI<T>::mouseDoubleClick(const MouseEvent & e)
{
	if (e.eventComponent == grabber) this->baseItem->miniMode->setValue(!this->baseItem->miniMode->boolValue());

}

template<class T>
void BaseItemUI<T>::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == this->baseItem->miniMode) updateMiniModeUI();
	else if (canBeDragged && c == this->baseItem->viewUIPosition) itemUIListeners.call(&ItemUIListener::itemUIGrabbed, this);
}


template<class T>
void BaseItemUI<T>::Grabber::paint(Graphics & g)
{
	Rectangle<int> r = getLocalBounds();
	g.setColour(BG_COLOR.brighter(.3f));
	const int numLines = 3;
	for (int i = 0; i < numLines; i++)
	{
		float th = (i + 1)*(float)getHeight() / ((float)numLines + 1);
		g.drawLine(0, th, (float)getWidth(), th, 1);
	}
}




#endif  // BASEITEMUI_H_INCLUDED
