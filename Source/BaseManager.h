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

template <class T>
class BaseManager :
	public ControllableContainer,
	public BaseItem::Listener
{
public : 
	BaseManager<T>(const String &name);
	virtual ~BaseManager<T>();


	OwnedArray<T> items;
	
	T * addItem();
	void addItem(T *);
	void removeItem(T *);

	bool selectItemWhenCreated;

	void clear();
	void askForRemoveBaseItem(BaseItem * item) override;


	class  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}
		virtual void itemAdded(BaseItem *) {}
		virtual void itemRemoved(BaseItem *) {}
	};

	ListenerList<Listener> baseManagerListeners;
	void addBaseManagerListener(Listener* newListener) { baseManagerListeners.add(newListener); }
	void removeBaseManagerListener(Listener* listener) { baseManagerListeners.remove(listener); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseManager<T>)
};


template<class T>
BaseManager<T>::BaseManager(const String & name) :
	ControllableContainer(name),
	selectItemWhenCreated(true)
{
	setCanHavePresets(false);
	nameCanBeChangedByUser = false;
}

template<class T>
inline BaseManager<T>::~BaseManager()
{
	clear();
}

template<class T>
T * BaseManager<T>::addItem()
{
	T * item = new T();
	addItem(item);
	return item;
}

template<class T>
inline void BaseManager<T>::addItem(T * item)
{
	jassert(items.indexOf(item) == -1); //be sure item is no here already

	items.add(item);
	BaseItem * bi = static_cast<BaseItem *>(item);
	addChildControllableContainer(bi);
	bi->nameParam->setValue(bi->niceName);
	bi->addBaseItemListener(this);
	baseManagerListeners.call(&BaseManager::Listener::itemAdded, item);

	if (selectItemWhenCreated) bi->selectThis();
}

template<class T>
void BaseManager<T>::removeItem(T * item)
{
	items.removeObject(item, false);
	BaseItem * bi = static_cast<BaseItem *>(item);
	removeChildControllableContainer(bi);
	bi->removeBaseItemListener(this);
	baseManagerListeners.call(&BaseManager::Listener::itemRemoved, item);
	delete item;
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




#endif  // BASEMANAGER_H_INCLUDED


