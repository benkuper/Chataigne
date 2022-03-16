/*
  ==============================================================================

    ModuleEditor.h
    Created: 28 Dec 2016 1:37:59pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ModuleEditor :
	public BaseItemEditor,
	public Module::ModuleListener
{
public:
	ModuleEditor(Array<Module *> modules, bool isRoot);
	virtual ~ModuleEditor();

	Array<Module*> modules;
	Module * module;
	std::unique_ptr<BoolToggleUI> logIncomingUI;
	std::unique_ptr<BoolToggleUI> logOutgoingUI;

	virtual void resizedInternalHeaderItemInternal(Rectangle<int> &r) override; 
	
	virtual void moduleIOConfigurationChanged() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleEditor)
};