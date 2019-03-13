/*
  ==============================================================================

    ResolumeBaseCommand.h
    Created: 19 Feb 2017 1:38:39pm
    Author:  Ben

  ==============================================================================
*/

#ifndef RESOLUMEBASECOMMAND_H_INCLUDED
#define RESOLUMEBASECOMMAND_H_INCLUDED

#include "../../commands/OSCCommand.h"

class ResolumeModule;

class ResolumeBaseCommand :
	public OSCCommand
{
public:
	ResolumeBaseCommand(ResolumeModule * _module, CommandContext context, var params, bool customRebuild = false);
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

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params) { return new ResolumeBaseCommand((ResolumeModule *)module, context, params); }

};

#endif  // RESOLUMEBASECOMMAND_H_INCLUDED