/*
  ==============================================================================

    GenericManagerEditor.h
    Created: 19 Dec 2016 4:29:28pm
    Author:  Ben

  ==============================================================================
*/

#ifndef GENERICMANAGEREDITOR_H_INCLUDED
#define GENERICMANAGEREDITOR_H_INCLUDED

#include "GenericControllableContainerEditor.h"
#include "BaseManager.h" 

template<class T>
class BaseManager;

template<class T>
class GenericManagerEditor :
	public GenericControllableContainerEditor
{
public:
	GenericManagerEditor(BaseManager<T> * manager);
	~GenericManagerEditor();

	String addItemText;
	BaseManager<T> * manager;

	void mouseDown(const MouseEvent &e) override;
};


template<class T>
GenericManagerEditor<T>::GenericManagerEditor(BaseManager<T> * _manager) :
	GenericControllableContainerEditor(_manager),
	manager(_manager),
	addItemText("Add item")
{
	DBG("Generic Manager editor !");
	setInterceptsMouseClicks(true, true);
}

template<class T>
GenericManagerEditor<T>::~GenericManagerEditor()
{
}

template<class T>
void GenericManagerEditor<T>::mouseDown(const MouseEvent & e)
{
	GenericControllableContainerEditor::mouseDown(e);

	DBG("Mouse down");
	if (e.mods.isLeftButtonDown())
	{
	}
	else if (e.mods.isRightButtonDown())
	{
		PopupMenu p;
		p.addItem(1,addItemText);
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

#endif  // GENERICMANAGEREDITOR_H_INCLUDED
