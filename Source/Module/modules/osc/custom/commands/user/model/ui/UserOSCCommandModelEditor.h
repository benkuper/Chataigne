/*
  ==============================================================================

    UserOSCCommandModelEditor.h
    Created: 15 Jan 2017 5:01:13pm
    Author:  Ben

  ==============================================================================
*/

#ifndef USEROSCCOMMANDMODELEDITOR_H_INCLUDED
#define USEROSCCOMMANDMODELEDITOR_H_INCLUDED



#include "../UserOSCCommandModel.h"

class OSCCommandModelArgumentEditor :
	public BaseItemEditor
{
public:
	OSCCommandModelArgumentEditor(OSCCommandModelArgument * a, bool isRoot);
	~OSCCommandModelArgumentEditor();

	OSCCommandModelArgument * arg;

	ScopedPointer<BoolToggleUI> useInMappingUI;
	ScopedPointer<BoolToggleUI> editableUI;

	void resetAndBuild() override;
	void resizedInternalHeaderItemInternal(Rectangle<int> &r) override;

};

class UserOSCCommandModelArgumentManagerEditor :
	public GenericManagerEditor<OSCCommandModelArgument>
{
public:
	UserOSCCommandModelArgumentManagerEditor(OSCCommandModelArgumentManager * manager, bool isRoot);
	~UserOSCCommandModelArgumentManagerEditor() {};
	
	OSCCommandModelArgumentManager * aManager;

	void showMenuAndAddItem(bool) override;
};

#endif  // USEROSCCOMMANDMODELEDITOR_H_INCLUDED
