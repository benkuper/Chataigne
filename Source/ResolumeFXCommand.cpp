/*
  ==============================================================================

    ResolumeFXCommand.cpp
    Created: 19 Feb 2017 1:42:30pm
    Author:  Ben

  ==============================================================================
*/

#include "ResolumeFXCommand.h"

ResolumeFXCommand::ResolumeFXCommand(ResolumeModule * _module, CommandContext context, var params) :
	ResolumeBaseCommand(_module,context,params)
{
	fxType = params.getProperty("fxType", "transform");

	if (fxType == "transform" || fxType == "audio")
	{
		nameParam = addEnumParameter("Parameter", "Name of the target parameter in the transform menu");
		if (fxType == "transform")
		{
			nameParam->addOption("Opacity", "opacity");
			nameParam->addOption("Width", "width");
			nameParam->addOption("Height", "height");
			nameParam->addOption("Scale", "scale");
			nameParam->addOption("Position X", "positionx");
			nameParam->addOption("Position Y", "positiony");
			nameParam->addOption("Rotate X", "rotatex");
			nameParam->addOption("Rotate Y", "rotatey");
			nameParam->addOption("Rotate Z", "rotatez");
			nameParam->addOption("Anchor X", "anchorx");
			nameParam->addOption("Anchor Y", "anchory");
			nameParam->addOption("Anchor Z", "anchorz");
		} else
		{
			nameParam->addOption("Volume", "volume");
			nameParam->addOption("Pan", "pan");
		}
	} else if(fxType == "videofx" || fxType == "vst" || fxType == "source")
	{

		if (fxType == "videofx" || fxType == "vst")
		{
			fxIndexParam = addIntParameter("Effect ID", "Id of the effect, depending on its position in the effect chain", 1, 1, 100);
		}

		indexParam = addIntParameter("Parameter ID", "Id of the parameter, depending on its position inside in the panel", 1, 1, 100);
	}

	valueParam = argumentsContainer.addFloatParameter("Value", "Target parameter value",0, 0, 1);
	setTargetMappingParameterAt(valueParam, 0);
	rebuildAddress();
}

ResolumeFXCommand::~ResolumeFXCommand()
{
}

void ResolumeFXCommand::rebuildAddress()
{
	if (fxType == "transform") addressSuffix = "video/" + nameParam->getValueData().toString() + "/values";
	else if (fxType == "audio") addressSuffix = "audio/" + nameParam->getValueData().toString() + "/values";
	else if (fxType == "videofx") addressSuffix = "video/effect" + fxIndexParam->stringValue() + "/param" + indexParam->stringValue() + "/values";
	else if (fxType == "vst") addressSuffix = "audio/effect" + fxIndexParam->stringValue() + "/param" + indexParam->stringValue() + "/values";
	else if (fxType == "source") addressSuffix = "video/param" + indexParam->stringValue() + "/values";

	ResolumeBaseCommand::rebuildAddress();
}

void ResolumeFXCommand::onContainerParameterChanged(Parameter * p)
{
	ResolumeBaseCommand::onContainerParameterChanged(p);

	if (p == nameParam || p == fxIndexParam || p == indexParam)
	{
		rebuildAddress();
	}
}
