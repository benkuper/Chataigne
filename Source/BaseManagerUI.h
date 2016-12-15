/*
  ==============================================================================

    BaseManagerUI.h
    Created: 28 Oct 2016 8:03:45pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef BASEMANAGERUI_H_INCLUDED
#define BASEMANAGERUI_H_INCLUDED

#pragma warning(disable:4244)

#include "InspectableContentComponent.h"
#include "BaseManager.h"
#include "BaseItemMinimalUI.h"
#include "Style.h"

template<class T>
class BaseManagerItemComparator
{
public:
	template<class T>
	BaseManagerItemComparator(BaseManager<T> * _manager) :manager(_manager) {}
	
	BaseManager<T> * manager;

	template<class T>
	int compareElements(BaseItemMinimalUI<T> * u1, BaseItemMinimalUI<T> * u2)
	{
		return (manager->items.indexOf(u1->item) < manager->items.indexOf(u2->item)) ? -1 : 1;
	}
};


template<class M, class T, class U>
class BaseManagerUI;

template<class M, class T, class U>
class ManagerUIItemContainer :
	public Component
{
public:
	ManagerUIItemContainer<M,T,U>(BaseManagerUI<M,T,U> * _mui) : mui(_mui){};
	~ManagerUIItemContainer() {}

	BaseManagerUI<M,T,U> * mui;

	void childBoundsChanged(Component * c) { mui->childBoundsChanged(c); }
};

template<class M, class T, class U>
class BaseManagerUI :
	public InspectableContentComponent,
	public BaseManager<T>::Listener
{
public:
	BaseManagerUI<M, T, U>(const String &contentName, M * _manager, bool _useViewport = true);
	virtual ~BaseManagerUI();

	M * manager;
	OwnedArray<U> itemsUI;
	BaseManagerItemComparator<T> managerComparator;

	//ui
	bool useViewport; //TODO, create a BaseManagerViewportUI
	
	ManagerUIItemContainer<M, T, U> container;
	Viewport viewport;
	
	Colour bgColor;
	int labelHeight = 10;
	String managerUIName;
	bool drawContour;
	bool drawHighlightWhenSelected;
	bool transparentBG;

	//menu
	bool useDefaultMenu;
	String addItemText;

	//layout
	bool fixedItemHeight;
	int gap = 2;

	virtual void mouseDown(const MouseEvent &e) override;
	virtual void paint(Graphics &g) override;

	virtual void resized() override;
	virtual void childBoundsChanged(Component *) override;

	virtual void addItemFromMenu();
	virtual U * addItemUI(T * item);
	virtual U * createUIForItem(T * item);
	virtual void addItemUIInternal(U *) {}


	virtual void removeItemUI(T * item);
	virtual void removeItemUIInternal(U *) {}

	U * getUIForItem(T * item);

	int getContentHeight();

	void itemAdded(T * item) override;
	void itemRemoved(T * item) override;
	void itemsReordered() override;

	class  ManagerUIListener
	{
	public:
		/** Destructor. */
		virtual ~ManagerUIListener() {}
		virtual void itemUIAdded(U *) {}
		virtual void itemUIRemoved(U *) {}
	};

	ListenerList<ManagerUIListener> managerUIListeners;
	void addManagerUIListener(ManagerUIListener* newListener) { managerUIListeners.add(newListener); }
	void removeManagerUIListener(ManagerUIListener* listener) { managerUIListeners.remove(listener); }

};

template<class M, class T, class U>
BaseManagerUI<M, T, U>::BaseManagerUI(const String & contentName, M * _manager, bool _useViewport) :
	InspectableContentComponent(_manager),
	manager(_manager),
	container(this),
	drawContour(false),
	bgColor(BG_COLOR),
	transparentBG(false),
	managerUIName(contentName),
	fixedItemHeight(true),
	useViewport(_useViewport),
	useDefaultMenu(true),
	managerComparator(_manager)
{
	highlightColor = LIGHTCONTOUR_COLOR;
	addItemText = "Add Item";

	if (useViewport)
	{
		viewport.setViewedComponent(&container, false);
		viewport.setScrollBarsShown(true, false);
		viewport.setScrollOnDragEnabled(false);
		viewport.setScrollBarThickness(10);
		this->addAndMakeVisible(viewport);
	} 

	BaseManager<T>* baseM = static_cast<BaseManager<T>*>(manager);
	baseM->addBaseManagerListener(this);
	//add existing items
	for (auto &t : baseM->items) addItemUI(t);
}


template<class M, class T, class U>
BaseManagerUI<M, T, U>::~BaseManagerUI()
{
	static_cast<BaseManager<T>*>(manager)->removeBaseManagerListener(this);
}

template<class M, class T, class U>
void BaseManagerUI<M, T, U>::mouseDown(const MouseEvent & e)
{
	InspectableContentComponent::mouseDown(e);

	if (e.mods.isLeftButtonDown())
	{
	} else if (e.mods.isRightButtonDown())
	{
		if (useDefaultMenu)
		{
			PopupMenu p;
			p.addItem(1, addItemText);
			int result = p.show();
			if (result)
			{
				switch (result)
				{
				case 1:
					addItemFromMenu();
					break;
				}
			}
		}
	} 
}

template<class M, class T, class U>
void BaseManagerUI<M, T, U>::paint(Graphics & g)
{
	if (drawContour)
	{
		Rectangle<int> r = getLocalBounds();
		if (!transparentBG)
		{
			g.setColour(bgColor);
			g.fillRoundedRectangle(r.toFloat(), 4);
		}
		
		Colour contourColor = bgColor.brighter(.3f);
		g.setColour(contourColor);
		g.drawRoundedRectangle(r.toFloat(), 4, 2);
		
		g.setFont(g.getCurrentFont().withHeight(labelHeight));
		float textWidth = g.getCurrentFont().getStringWidth(managerUIName);
		Rectangle<int> tr = r.removeFromTop(labelHeight+2).reduced((r.getWidth() - textWidth) / 2, 0).expanded(4,0);
		g.fillRect(tr);
		Colour textColor = contourColor.withBrightness(contourColor.getBrightness() > .5f ? .1f : .9f).withAlpha(1.f);
		g.setColour(textColor);

		g.drawText(manager->niceName, tr, Justification::centred, 1);
	}else
	{
		if(!transparentBG)	g.fillAll(bgColor);
	}
}

template<class M, class T, class U>
void BaseManagerUI<M, T, U>::resized()
{
	
	Rectangle<int> r = getLocalBounds().reduced(2);
	if (drawContour) r.removeFromTop(15);

	if (useViewport)
	{
		viewport.setBounds(r);
		r.removeFromRight(drawContour?14:10);
		r.setTop(0);
	}

	
	for (auto &ui : itemsUI)
	{
		BaseItemMinimalUI<T> * bui = static_cast<BaseItemMinimalUI<T>*>(ui);
		Rectangle<int> tr = r.withHeight(bui->getHeight());
		bui->setBounds(tr);
		r.translate(0, tr.getHeight() + gap);
	}

	if (useViewport) 
	{
		float th = 0;
		if (itemsUI.size() > 0) th = static_cast<BaseItemMinimalUI<T>*>(itemsUI[itemsUI.size() - 1])->getBottom();
		container.setBounds(getLocalBounds().withHeight(th));
	}
}

template<class M, class T, class U>
void BaseManagerUI<M, T, U>::childBoundsChanged(Component *)
{
	resized();
}

template<class M, class T, class U>
void BaseManagerUI<M, T, U>::addItemFromMenu()
{
	manager->BaseManager<T>::addItem();
}

template<class M, class T, class U>
U * BaseManagerUI<M, T, U>::addItemUI(T * item)
{
	U * tui = createUIForItem(item);
	itemsUI.add(tui);
	if(useViewport) container.addAndMakeVisible(static_cast<BaseItemMinimalUI<T>*>(tui));
	else addAndMakeVisible(static_cast<BaseItemMinimalUI<T>*>(tui));
	addItemUIInternal(tui);
	resized();
	managerUIListeners.call(&ManagerUIListener::itemUIAdded, tui);
	return tui;
}

template<class M, class T, class U>
inline U * BaseManagerUI<M, T, U>::createUIForItem(T * item)
{
	return new U(item);
}

template<class M, class T, class U>
void BaseManagerUI<M, T, U>::removeItemUI(T * item)
{
	U * tui = getUIForItem(item);
	if (tui == nullptr) return;

	if(useViewport) container.removeChildComponent(static_cast<BaseItemMinimalUI<T>*>(tui));
	else removeChildComponent(static_cast<BaseItemMinimalUI<T>*>(tui));
	removeItemUIInternal(tui);
	itemsUI.removeObject(tui,false);
	managerUIListeners.call(&ManagerUIListener::itemUIRemoved, tui);
	delete tui;
	resized();
}

template<class M, class T, class U>
U * BaseManagerUI<M, T, U>::getUIForItem(T * item)
{
	//for (auto &ui : itemsUI) if (static_cast<BaseItemMinimalUI<T>*>(ui)->item == item) return ui; //brute search, not needed if ui/items are synchronized
	return itemsUI[static_cast<BaseManager<T>*>(manager)->items.indexOf(item)];
	//return nullptr;
}

template<class M, class T, class U>
int BaseManagerUI<M, T, U>::getContentHeight()
{
	return container.getHeight() + 20;
}

template<class M, class T, class U>
void BaseManagerUI<M, T, U>::itemAdded(T * item)
{
	addItemUI(item);
}

template<class M, class T, class U>
void BaseManagerUI<M, T, U>::itemRemoved(T * item)
{
	removeItemUI(item);
}

template<class M, class T, class U>
inline void BaseManagerUI<M, T, U>::itemsReordered()
{
	itemsUI.sort(managerComparator);
}


#endif  // BASEMANAGERUI_H_INCLUDED
