/*
  ==============================================================================

    CustomOSCCommandArgumentEditor.cpp
    Created: 22 Feb 2017 8:53:11am
    Author:  Ben

  ==============================================================================
*/

#include "CustomOSCCommandArgumentEditor.h"



CustomOSCCommandArgumentEditor::CustomOSCCommandArgumentEditor(CustomOSCCommandArgument * a, bool isRoot) :
	BaseItemEditor(a, isRoot),
	arg(a)
{
	paramUI = static_cast<ParameterEditor *>(arg->param->getEditor(false));
	paramUI->setShowLabel(false);

	if(arg->mappingEnabled) useInMappingUI = arg->useForMapping->createToggle();

	addAndMakeVisible(paramUI);
	addAndMakeVisible(useInMappingUI);
}

CustomOSCCommandArgumentEditor::~CustomOSCCommandArgumentEditor()
{
}

void CustomOSCCommandArgumentEditor::resizedInternalHeader(Rectangle<int>& r)
{
	if (arg->mappingEnabled) 
	{
		useInMappingUI->setBounds(r.removeFromRight(80));
		r.removeFromRight(2);
	}}

void CustomOSCCommandArgumentEditor::resizedInternalContent(Rectangle<int>& r)
{
	r.setHeight(paramUI->getHeight());
	paramUI->setBounds(r);
}

void CustomOSCCommandArgumentEditor::childBoundsChanged(Component * child)
{
	if (child == paramUI) resized();
}

