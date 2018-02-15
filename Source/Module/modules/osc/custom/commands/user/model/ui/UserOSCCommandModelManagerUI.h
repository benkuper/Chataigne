/*
  ==============================================================================

    UserOSCCommandModelManagerUI.h
    Created: 19 Dec 2016 3:06:08pm
    Author:  Ben

  ==============================================================================
*/

#ifndef USEROSCCOMMANDMODELMANAGERUI_H_INCLUDED
#define USEROSCCOMMANDMODELMANAGERUI_H_INCLUDED

#include "../UserOSCCommandModelManager.h"
#include "UserOSCCommandModelUI.h"
#include "../../../../CustomOSCModule.h"

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
	public InspectableSelectionManager::Listener,
	public Inspectable::InspectableListener
	
{
public:
	UserOSCCommandModelManagerWindow(const String &name = "OSC Models");
	~UserOSCCommandModelManagerWindow();

	CustomOSCModule * module;
	ScopedPointer<UserOSCCommandModelManagerUI> modelManagerUI;

	void editModule(CustomOSCModule * module);
	void resized() override;

	void paint(Graphics &g) override;

	void inspectablesSelectionChanged() override;
	void inspectableDestroyed(Inspectable *) override;

	static UserOSCCommandModelManagerWindow * create(const String &contentName) { return new UserOSCCommandModelManagerWindow(contentName); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UserOSCCommandModelManagerWindow)
};



#endif  // USEROSCCOMMANDMODELMANAGERUI_H_INCLUDED
