/*
  ==============================================================================

    BaseManagerUI.h
    Created: 28 Oct 2016 8:03:45pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef BASEMANAGERUI_H_INCLUDED
#define BASEMANAGERUI_H_INCLUDED

#include "ShapeShifterContent.h"
#include "BaseManager.h"
#include "BaseItemUI.h"
#include "Style.h"

template<class M, class T, class U>
class BaseManagerUI :
	public ShapeShifterContent,
	public BaseManager<T>::Listener
{
public:
	BaseManagerUI<M, T, U>(const String &contentName, M * _manager);
	virtual ~BaseManagerUI();

	M * manager;
	OwnedArray<U> itemsUI;
	
	//ui
	String addItemText;
	int itemHeight = 20;
	int gap = 2;

	virtual void mouseDown(const MouseEvent &e) override;
	virtual void paint(Graphics &g) override;
	virtual void resized() override;
	
	virtual U * addItemUI(T * item);
	virtual void removeItemUI(T * item);
	U * getUIForItem(T * item);

	void itemAdded(BaseItem * item) override;
	void itemRemoved(BaseItem * item) override;

	virtual void itemAddedInternal(T * ) {}
	virtual void itemRemovedInternal(T * ) {}


};

template<class M, class T, class U>
BaseManagerUI<M, T, U>::BaseManagerUI(const String &contentName, M * _manager) :
	ShapeShifterContent(contentName),
	manager(_manager)
{
	relatedControllableContainer = static_cast<ControllableContainer *>(manager);
	static_cast<BaseManager<T>*>(manager)->addBaseManagerListener(this);
	addItemText = "Add Item";
}


template<class M, class T, class U>
BaseManagerUI<M, T, U>::~BaseManagerUI()
{
	static_cast<BaseManager<T>*>(manager)->removeBaseManagerListener(this);
}

template<class M, class T, class U>
void BaseManagerUI<M, T, U>::mouseDown(const MouseEvent & e)
{
	if (e.mods.isLeftButtonDown())
	{
		selectThis();
	} else if (e.mods.isRightButtonDown())
	{
		PopupMenu p;
		p.addItem(1, addItemText);
		int result = p.show();
		if (result)
		{
			switch (result)
			{
			case 1:
				manager->addItem();
				break;
			}
		}
	} 
}

template<class M, class T, class U>
void BaseManagerUI<M, T, U>::paint(Graphics & g)
{
	g.fillAll(BG_COLOR);
}

template<class M, class T, class U>
void BaseManagerUI<M, T, U>::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(2);
	for (auto &ui : itemsUI)
	{
		BaseItemUI<T> * bui = static_cast<BaseItemUI<T>*>(ui);
		bui->setBounds(r.removeFromTop(itemHeight));
		r.removeFromTop(gap);
	}
}

template<class M, class T, class U>
U * BaseManagerUI<M, T, U>::addItemUI(T * item)
{
	U * tui = new U(item);
	itemsUI.add(tui);
	addAndMakeVisible(static_cast<BaseItemUI<T>*>(tui));
	resized();

	return tui;
}

template<class M, class T, class U>
void BaseManagerUI<M, T, U>::removeItemUI(T * item)
{
	U * tui = getUIForItem(item);
	if (tui == nullptr) return;

	removeChildComponent(static_cast<BaseItemUI<T>*>(tui));
	itemsUI.removeObject(tui);
	resized();
}

template<class M, class T, class U>
U * BaseManagerUI<M, T, U>::getUIForItem(T * item)
{
	for (auto &ui : itemsUI) if (static_cast<BaseItemUI<T>*>(ui)->item == item) return ui;
	return nullptr;
}

template<class M, class T, class U>
void BaseManagerUI<M, T, U>::itemAdded(BaseItem * item)
{
	addItemUI(static_cast<T*>(item));
	itemAddedInternal(static_cast<T*>(item));
}

template<class M, class T, class U>
void BaseManagerUI<M, T, U>::itemRemoved(BaseItem * item)
{
	removeItemUI(static_cast<T*>(item));
	itemRemovedInternal(static_cast<T*>(item));
}


#endif  // BASEMANAGERUI_H_INCLUDED
