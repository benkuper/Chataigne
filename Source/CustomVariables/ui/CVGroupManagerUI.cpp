/*
  ==============================================================================

	CVGroupManagerUI.cpp
	Created: 22 Feb 2018 3:42:48pm
	Author:  Ben

  ==============================================================================
*/

CVGroupManagerUI::CVGroupManagerUI(const String & contentName) :
	ManagerShapeShifterUI(contentName, CVGroupManager::getInstance())
{
	addItemText = "Add Custom Variable Group";
	noItemText = "You can manage you own variables by first creating a group here.";
	
	setShowSearchBar(true);
	addExistingItems();
}

CVGroupManagerUI::~CVGroupManagerUI()
{
}
