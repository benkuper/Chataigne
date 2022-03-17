/*
  ==============================================================================

    EnablingNetworkControllableContainerEditor.h
    Created: 19 Jan 2018 4:48:00pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class EnablingNetworkControllableContainerEditor :
	public EnablingControllableContainerEditor
{
public:
	EnablingNetworkControllableContainerEditor(Array<EnablingControllableContainer *> cc, bool isRoot);
	~EnablingNetworkControllableContainerEditor();

	void resizedInternalHeader(Rectangle<int> &r) override;

	Label ipLabel;

	static InspectableEditor * create(bool isRoot, Array<ControllableContainer *> containers) { 
		return new EnablingNetworkControllableContainerEditor(Inspectable::getArrayAs<ControllableContainer, EnablingControllableContainer>(containers), isRoot); }
};