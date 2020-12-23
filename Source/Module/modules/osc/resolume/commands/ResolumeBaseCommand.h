/*
  ==============================================================================

    ResolumeBaseCommand.h
    Created: 19 Feb 2017 1:38:39pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../commands/OSCCommand.h"

class ResolumeModule;

class ResolumeBaseCommand :
	public OSCCommand
{
public:
	ResolumeBaseCommand(ResolumeModule * _module, CommandContext context, var params, IteratorProcessor * iterator = nullptr, bool customRebuild = false);
	virtual ~ResolumeBaseCommand();

	enum Level { COMPOSITION, LAYER, CLIP, COLUMN, SELECTED_CLIP, SELECTED_LAYER, DECK };

	ResolumeModule * resolumeModule;
	bool customRebuild;

	EnumParameter * levelParam;
	IntParameter * layerParam;
	IntParameter * clipParam;

	Array<WeakReference<Controllable>> resolumeControllables;

	String addressSuffix;

	virtual void rebuildParameters();
	virtual void rebuildParametersInternal() {}
	virtual void rebuildAddress() override;

	virtual void onContainerParameterChanged(Parameter *) override;
	virtual void onExternalParameterValueChanged(Parameter *) override;

	InspectableEditor * getEditor(bool isRoot) override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator) { return new ResolumeBaseCommand((ResolumeModule *)module, context, params, iterator); }

};