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
//#include "BaseManager.h" 

template<class T>
class BaseManager;

template<class T>
class GenericManagerEditor :
	public GenericControllableContainerEditor,
	public ButtonListener
{
public:
	GenericManagerEditor(BaseManager<T> * manager, bool isRoot);
	virtual ~GenericManagerEditor();

	BaseManager<T> * manager;

	//layout
	bool fixedItemHeight;
	int gap = 2;

	void addExistingItems();
	
	//menu
	ScopedPointer<ImageButton> addItemBT;
	String addItemText;


	void resized() override;


	virtual void showMenuAndAddItem(bool isFromAddButton);
	virtual void addItemFromMenu(bool isFromAddButton);

	void buttonClicked(Button *) override;
	void mouseDown(const MouseEvent &e) override;
};



#include "AssetManager.h"

template<class T>
GenericManagerEditor<T>::GenericManagerEditor(BaseManager<T> * _manager, bool isRoot) :
	GenericControllableContainerEditor(_manager, isRoot),
	manager(_manager),
	addItemText("Add item")
{
	headerHeight = 24;
	setInterceptsMouseClicks(true, true);

	addItemBT = AssetManager::getInstance()->getAddBT();
	addAndMakeVisible(addItemBT);
	addItemBT->addListener(this);
}

template<class T>
GenericManagerEditor<T>::~GenericManagerEditor()
{
}

template<class T>
void GenericManagerEditor<T>::addExistingItems()
{

	//add existing items
	for (auto &t : manager->items) addEditorUI(t, false);
	resized();
}

template<class T>
void GenericManagerEditor<T>::resized()
{
	GenericControllableContainerEditor::resized();

	juce::Rectangle<int> r = getLocalBounds().reduced(2);
	addItemBT->setBounds(r.withSize(24, 24).withX(r.getWidth() - 24));
}

template<class T>
void GenericManagerEditor<T>::showMenuAndAddItem(bool isFromAddButton)
{
	if (isFromAddButton)
	{
		manager->BaseManager<T>::addItem();
		return;
	}

	PopupMenu p;
	p.addItem(1, addItemText);

	int result = p.show();
	switch (result)
	{
	case 1:
		addItemFromMenu(isFromAddButton);
		break;
	}
}

template<class T>
void GenericManagerEditor<T>::addItemFromMenu(bool /*isFromAddButton*/)
{
	manager->BaseManager<T>::addItem();
}

template<class T>
inline void GenericManagerEditor<T>::buttonClicked(Button * b)
{
	if (b == addItemBT)
	{
		showMenuAndAddItem(true);
	}
} 

template<class T>
void GenericManagerEditor<T>::mouseDown(const MouseEvent & e)
{
	GenericControllableContainerEditor::mouseDown(e);

	if (e.mods.isLeftButtonDown())
	{
	} else if (e.mods.isRightButtonDown())
	{
		showMenuAndAddItem(false);
	}
}

#endif  // GENERICMANAGEREDITOR_H_INCLUDED
