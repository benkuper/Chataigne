/*
  ==============================================================================

    ProcessorManager.h
    Created: 15 Oct 2017 1:24:52pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class Mapping;
class Action;
class Multiplex;

class ProcessorManager :
	public Manager<Processor>
{
public:
	ProcessorManager(const String &name, Multiplex * multiplex = nullptr);
	~ProcessorManager();

	bool forceDisabled;
	void setForceDisabled(bool value, bool force = false, bool fromActivation = false);
	void addItemInternal(Processor* item, var data) override;
	void addItemsInternal(Array<Processor *> item, var data) override;

	bool canAddItemOfType(const String &typeToCheck) override;

	Array<Action *> getAllActions(bool includeMultiplexes = true, bool includeConductors = true);
	Array<Mapping *> getAllMappings(bool includeMultiplexes = true);
	void checkAllActivateActions();
	void checkAllDeactivateActions();
	void processAllMappings();

	Factory<Processor> factory;

	void loadJSONDataInternal(var data) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorManager)

};