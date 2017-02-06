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
#include "BaseManagerUI.h"
#include "ShapeShifterPanel.h"
#include "InspectableSelectionManager.h"

class UserOSCCommandModelManagerUI :
	public BaseManagerUI<UserOSCCommandModelManager, UserOSCCommandModel, UserOSCCommandModelUI>
{
public:
	UserOSCCommandModelManagerUI(UserOSCCommandModelManager * manager, const String &moduleName);
	~UserOSCCommandModelManagerUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UserOSCCommandModelManagerUI)
};

class UserOSCCommandModelManagerWindow : 
	public ShapeShifterContentComponent,
	public InspectableSelectionManager::Listener
	
{
public:
	UserOSCCommandModelManagerWindow(const String &name = "OSC Models");
	~UserOSCCommandModelManagerWindow();

	CustomOSCModule * module;
	ScopedPointer<UserOSCCommandModelManagerUI> modelManagerUI;

	void editModule(CustomOSCModule * module);
	void resized() override;

	void currentInspectableSelectionChanged(Inspectable *, Inspectable *) override;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UserOSCCommandModelManagerWindow)
};



#endif  // USEROSCCOMMANDMODELMANAGERUI_H_INCLUDED
