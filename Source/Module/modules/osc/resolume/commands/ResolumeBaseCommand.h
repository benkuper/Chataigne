/*
  ==============================================================================

	ResolumeBaseCommand.h
	Created: 19 Feb 2017 1:38:39pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

class ResolumeModule;

class ResolumeBaseCommand :
	public OSCCommand
{
public:
	ResolumeBaseCommand(ResolumeModule* _module, CommandContext context, var params, Multiplex* multiplex = nullptr, bool customRebuild = false);
	virtual ~ResolumeBaseCommand();

	enum Level { COMPOSITION, LAYER, CLIP, COLUMN, SELECTED_CLIP, SELECTED_LAYER, DECK };

	ResolumeModule* resolumeModule;
	bool customRebuild;

	EnumParameter* levelParam;
	IntParameter* layerParam;
	IntParameter* clipParam;

	Array<WeakReference<Controllable>> resolumeControllables;

	String addressSuffix;

	virtual void rebuildParameters();
	virtual void rebuildParametersInternal() {}
	virtual String getTargetAddress(int multiplexIndex = 0) override;

	virtual void onContainerParameterChanged(Parameter*) override;
	virtual void onExternalParameterValueChanged(Parameter*) override;

	InspectableEditor* getEditorInternal(bool isRoot) override;

	static BaseCommand* create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex) { return new ResolumeBaseCommand((ResolumeModule*)module, context, params, multiplex); }

};