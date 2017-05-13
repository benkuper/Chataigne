/*
  ==============================================================================

    ControllableUI.h
    Created: 9 Mar 2016 12:02:16pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CONTROLLABLEUI_H_INCLUDED
#define CONTROLLABLEUI_H_INCLUDED


#include "Controllable.h"
#include "Style.h"//keep 

class ControllableUI :
	public Component,
	public SettableTooltipClient,
	public Controllable::Listener
{
public:
	ControllableUI(Controllable * controllable);
	virtual ~ControllableUI();

	String tooltip;
	WeakReference<Controllable>  controllable;

	bool showLabel;
	bool opaqueBackground;

	void mouseDoubleClick(const MouseEvent &e) override;

	void mouseDown(const MouseEvent &e) override;
	void mouseUp(const MouseEvent &e) override;

	virtual void mouseDownInternal(const MouseEvent &) {} //avoid right click check in each child class
	virtual void mouseUpInternal(const MouseEvent &) {} //
	
	//PopupMenu
	virtual void addPopupMenuItems(PopupMenu *) {} //for child classes
	virtual void handleMenuSelectedID(int ) {} // to override

	virtual void setOpaqueBackground(bool value);

	bool forceFeedbackOnly; //if false, will rely on Controllable isEditable property. if true, will be force to no editable
	void setForceFeedbackOnly(bool value);
	virtual void setForceFeedbackOnlyInternal() {}


	virtual void showEditWindow() {} //to be overriden by children

    // Inherited via Listener
    virtual void controllableStateChanged(Controllable * c) override;
    virtual void controllableControlAddressChanged(Controllable * c) override;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControllableUI)

protected :
    void updateTooltip();
};


#endif  // CONTROLLABLEUI_H_INCLUDED
