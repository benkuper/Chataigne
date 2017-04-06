/*
  ==============================================================================

    BaseManager.h
    Created: 28 Oct 2016 8:03:13pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef BASEMANAGER_H_INCLUDED
#define BASEMANAGER_H_INCLUDED

#include "BaseItem.h"
#include "GenericManagerEditor.h"

template <class T>
class BaseManager :
	public ControllableContainer,
	public BaseItem::Listener
{
public : 
	BaseManager<T>(const String &name);
	virtual ~BaseManager<T>();

	OwnedArray<T> items;

	virtual T * createItem(); //to override if special constructor to use
	T * addItem();
	void addItem(T *, var data = var()); //if data is not empty, load data
	virtual void addItemFromData(var data); //to be overriden for specific item creation (from data)
	void removeItem(T *);

	virtual void reorderItems(); //to be overriden if needed

	//to override for specific handling like adding custom listeners, etc.
	virtual void addItemInternal(T *, var data) {}
	virtual void removeItemInternal(T *) {}

	bool selectItemWhenCreated;

	T * getItemWithName(const String &itemShortName);

	void clear();
	void askForRemoveBaseItem(BaseItem * item) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;


	DynamicObject * createScriptObject() override;

	PopupMenu getItemsMenu(int startID);
	T * getItemForMenuResultID(int id, int startID);

	class  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}
		virtual void itemAdded(T *) {}
		virtual void itemRemoved(T *) {}
		virtual void itemsReordered() {}
	};

	ListenerList<Listener> baseManagerListeners;
	void addBaseManagerListener(Listener* newListener) { baseManagerListeners.add(newListener); }
	void removeBaseManagerListener(Listener* listener) { baseManagerListeners.remove(listener); }
	 
	InspectableEditor * getEditor(bool /*isRoot*/) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseManager<T>)
};


template<class T>
BaseManager<T>::BaseManager(const String & name) :
	ControllableContainer(name),
	selectItemWhenCreated(true)
{
	setCanHavePresets(false);
	nameCanBeChangedByUser = false;
	isSelectable = false;
}

template<class T>
BaseManager<T>::~BaseManager()
{
	clear();
}

template<class T>
T * BaseManager<T>::createItem() {
	return new T();
}

template<class T>
T * BaseManager<T>::addItem()
{
	T * item = createItem();
	addItem(item);
	return item;
}

template<class T>
inline void BaseManager<T>::addItem(T * item, var data)
{
	jassert(items.indexOf(item) == -1); //be sure item is no here already
	if (item == nullptr) return;
	
	items.add(item);
	BaseItem * bi = static_cast<BaseItem *>(item);
	addChildControllableContainer(bi);
	bi->nameParam->setValue(bi->niceName);
	bi->addBaseItemListener(this);
	
	if(!data.isVoid()) bi->loadJSONData(data);
	addItemInternal(item, data);
	
	baseManagerListeners.call(&BaseManager::Listener::itemAdded, item);
	reorderItems();

	if (selectItemWhenCreated) bi->selectThis();
}

//if data is not empty, load data
template<class T>
void BaseManager<T>::addItemFromData(var data) 
{ 
	addItem(createItem(), data);
}

template<class T>
void BaseManager<T>::removeItem(T * item)
{
	items.removeObject(item, false);
	BaseItem * bi = static_cast<BaseItem *>(item);
	removeChildControllableContainer(bi);
	removeItemInternal(item);
	bi->removeBaseItemListener(this);

	baseManagerListeners.call(&BaseManager::Listener::itemRemoved, item);

	delete item;
}

template<class T>
void BaseManager<T>::reorderItems()
{
	baseManagerListeners.call(&Listener::itemsReordered);
}

template<class T>
inline T * BaseManager<T>::getItemWithName(const String & itemShortName)
{
	for (auto &t : items)
	{
		if (((BaseItem *)t)->shortName == itemShortName) return t;
	}

	return nullptr;
}

template<class T>
void BaseManager<T>::clear()
{
	while (items.size() > 0) removeItem(items[0]);
}

template<class T>
void BaseManager<T>::askForRemoveBaseItem(BaseItem * item)
{
	removeItem(static_cast<T*>(item));
}

template<class T>
var BaseManager<T>::getJSONData()
{
	var data = ControllableContainer::getJSONData();
	var itemsData = var();
	for (auto &t : items)
	{
		itemsData.append(t->getJSONData());
	}
	data.getDynamicObject()->setProperty("items", itemsData);

	return data;
}

template<class T>
void BaseManager<T>::loadJSONDataInternal(var data)
{ 
	clear();
	Array<var> * itemsData = data.getProperty("items",var()).getArray();
	if (itemsData == nullptr) return;
	for (auto &td : *itemsData)
	{ 
		addItemFromData(td);
	}
}

template<class T>
DynamicObject * BaseManager<T>::createScriptObject()
{
	DynamicObject * o = ControllableContainer::createScriptObject();
	var itemsArray = var();
	for (auto &t : items) itemsArray.append(t->createScriptObject());
	o->setProperty("items", itemsArray);
	return o;
}

template<class T>
inline PopupMenu BaseManager<T>::getItemsMenu(int startID)
{
	PopupMenu menu;
	int numValues = items.size();
	for (int j = 0; j < numValues; j++)
	{
		T * c = items[j];
		menu.addItem(startID + j, c->niceName);
	}
	return menu;
}

template<class T>
inline T * BaseManager<T>::getItemForMenuResultID(int id, int startID)
{
	return items[id - startID];
}


template<class T>
InspectableEditor * BaseManager<T>::getEditor(bool isRoot)
{
	return new GenericManagerEditor<T>(this, isRoot);
}


#endif  // BASEMANAGER_H_INCLUDED


