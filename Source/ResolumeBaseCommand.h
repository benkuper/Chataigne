/*
  ==============================================================================

    ResolumeBaseCommand.h
    Created: 19 Feb 2017 1:38:39pm
    Author:  Ben

  ==============================================================================
*/

#ifndef RESOLUMEBASECOMMAND_H_INCLUDED
#define RESOLUMEBASECOMMAND_H_INCLUDED

#include "OSCCommand.h"
class ResolumeModule;

class ResolumeBaseCommand :
	public OSCCommand
{
public:
	ResolumeBaseCommand(ResolumeModule * _module, CommandContext context, var params);
	virtual ~ResolumeBaseCommand();

	enum Level {COMPOSITION,LAYER,CLIP,COLUMN};

	ResolumeModule * resolumeModule;
	
	bool multiLevelAccess;
	EnumParameter * levelParam;
	IntParameter * layerParam;
	IntParameter * clipParam;

	String addressSuffix;

	virtual void rebuildAddress() override;

	virtual void onContainerParameterChanged(Parameter *) override;

	InspectableEditor * getEditor(bool isRoot) override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params) { return new ResolumeBaseCommand((ResolumeModule *)module, context, params); }

};



#endif  // RESOLUMEBASECOMMAND_H_INCLUDED
