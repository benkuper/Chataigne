/*
  ==============================================================================

    ControllableHelpers.h
    Created: 12 May 2016 4:21:18pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CONTROLLABLEHELPERS_H_INCLUDED
#define CONTROLLABLEHELPERS_H_INCLUDED

#include "JuceHeader.h"//keep
class ControllableContainer;
class Controllable;


class ControllableContainerPopupMenu : public PopupMenu
{
public:
	ControllableContainerPopupMenu(ControllableContainer * rootContainer);
	virtual ~ControllableContainerPopupMenu();

	Array<Controllable *> controllableList;
	void populateMenu(PopupMenu *subMenu, ControllableContainer * container, int &currentId);


	Controllable * showAndGetControllable();

};


class ControllableChooser : public TextButton, public ButtonListener
{
public :
	ControllableChooser(ControllableContainer * rootContainer = nullptr);
	virtual ~ControllableChooser();

	ControllableContainer * rootContainer;

	Controllable * currentControllable;
	void setCurrentControllale(Controllable * c);

	class  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}
		virtual void choosedControllableChanged(Controllable *) {};
	};

	ListenerList<Listener> listeners;
	void addControllableChooserListener(Listener* newListener) { listeners.add(newListener); }
	void removeControllableChooserListener(Listener* listener) { listeners.remove(listener); }

	virtual void buttonClicked(Button * b) override;
};

#endif  // CONTROLLABLEHELPERS_H_INCLUDED
