/*
  ==============================================================================

    UserOSCCommandModelManagerUI.h
    Created: 19 Dec 2016 3:06:08pm
    Author:  Ben

  ==============================================================================
*/

#ifndef USEROSCCOMMANDMODELMANAGERUI_H_INCLUDED
#define USEROSCCOMMANDMODELMANAGERUI_H_INCLUDED

#include "UserOSCCommandModelManager.h"
#include "UserOSCCommandModelUI.h"
#include "CustomOSCModule.h"
#include "BaseManagerShapeShifterUI.h"

class CustomOSCArgumentUI :
	public Component
{
public:
	CustomOSCArgumentUI();
	ComboBox typeBox;
	Label defaultValue;
	ToggleButton getFromMapping;
	Slider mappingValueId;

	void resized() override;
};


class UserOSCCommandModelManagerUI :
	public BaseManagerUI<UserOSCCommandModelManager, UserOSCCommandModel, UserOSCCommandModelUI>
{
public:
	UserOSCCommandModelManagerUI(UserOSCCommandModelManager * manager);
	~UserOSCCommandModelManagerUI();
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UserOSCCommandModelManagerUI)
};

class UserOSCCommandModelManagerWindow :
	public ResizableWindow
	
{
public:
	juce_DeclareSingleton(UserOSCCommandModelManagerWindow,false)
public:
	UserOSCCommandModelManagerWindow();
	~UserOSCCommandModelManagerWindow();

	CustomOSCModule * module;
	ScopedPointer<UserOSCCommandModelManagerUI> modelManagerUI;

	void editModule(CustomOSCModule * module);

	void userTriedToCloseWindow() override;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UserOSCCommandModelManagerWindow)
};



#endif  // USEROSCCOMMANDMODELMANAGERUI_H_INCLUDED
