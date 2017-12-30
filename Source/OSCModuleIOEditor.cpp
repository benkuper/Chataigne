/*
  ==============================================================================

    OSCModuleIOEditor.cpp
    Created: 30 Dec 2017 7:47:46pm
    Author:  Ben

  ==============================================================================
*/

#include "OSCModuleIOEditor.h"
#include "ChataigneAssetManager.h"

OSCIOEditor::OSCIOEditor(OSCIOContainer * cc, bool isRoot) :
	GenericControllableContainerEditor(cc,isRoot),
	ioContainer(cc)
{
	enabledUI = ioContainer->enabled->createImageToggle(AssetManager::getInstance()->getPowerBT());
	addAndMakeVisible(enabledUI);
}

void OSCIOEditor::resizedInternalHeader(Rectangle<int>& r)
{
	enabledUI->setBounds(r.removeFromLeft(r.getHeight()).reduced(2));
	GenericControllableContainerEditor::resizedInternalHeader(r);
}
