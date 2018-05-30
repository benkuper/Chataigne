/*
  ==============================================================================

    ResolumeFXCommand.cpp
    Created: 19 Feb 2017 1:42:30pm
    Author:  Ben

  ==============================================================================
*/

#include "ResolumeFXCommand.h"
#include "../ResolumeModule.h"

ResolumeFXCommand::ResolumeFXCommand(ResolumeModule * _module, CommandContext context, var params) :
	ResolumeBaseCommand(_module,context,params),
	valueParam(nullptr),
	fxParamType(nullptr),
	fxName(nullptr)
{
	int resolumeVersion = (int)resolumeModule->version->getValueData();

	fxType = params.getProperty("fxType", "transform");

	if (fxType == "transform" || fxType == "audio")
	{
		nameParam = addEnumParameter("Parameter", "Name of the target parameter in the transform menu");
		if (fxType == "transform")
		{
			nameParam->addOption("Opacity", "opacity");
			nameParam->addOption("Width", "width");
			nameParam->addOption("Height", "height");

			
			String suf = resolumeVersion >= 6 ? "effects/transform/" : "";

			nameParam->addOption("Scale", suf+"scale");
			if (resolumeVersion >= 6)
			{
				nameParam->addOption("ScaleW", suf + "scalew");
				nameParam->addOption("ScaleH", suf + "scaleh");

			}
			nameParam->addOption("Position X", suf + "positionx");
			nameParam->addOption("Position Y", suf + "positiony");
			nameParam->addOption("Rotate X", suf + (resolumeVersion >= 6 ? "rotationx" : "rotatex"));
			nameParam->addOption("Rotate Y", suf + (resolumeVersion >= 6 ? "rotationy" : "rotatex"));
			nameParam->addOption("Rotate Z", suf + (resolumeVersion >= 6 ? "rotationz" : "rotatex"));
			nameParam->addOption("Anchor X", suf + "anchorx");
			nameParam->addOption("Anchor Y", suf + "anchory");
			nameParam->addOption("Anchor Z", suf + "anchorz");
		} else
		{
			nameParam->addOption("Volume", "volume");
			nameParam->addOption("Pan", "pan");
		}
	} else if(fxType == "videofx" || fxType == "vst" || fxType == "source")
	{
		if (resolumeVersion < 6)
		{
			if (fxType != "source") fxIndexParam = addIntParameter("Effect ID", "Id of the effect, depending on its position in the effect chain", 1, 1, 100);
			indexParam = addIntParameter("Parameter ID", "Id of the parameter, depending on its position inside in the panel", 1, 1, 100);
		}else
		{
			fxParamType = addEnumParameter("Parameter type", "The type of parameter");
			fxParamType->addOption("Float", FloatParameter::getTypeStringStatic())->addOption("Integer", IntParameter::getTypeStringStatic())
					   ->addOption("Boolean", BoolParameter::getTypeStringStatic())->addOption("String", StringParameter::getTypeStringStatic())
					   ->addOption("Color", ColorParameter::getTypeStringStatic());
			if(fxType != "source") fxName = addStringParameter("Effect name", "Name of the effeect, either its view name (e.g. \"My Effect\") or its control name (e.g. \"myeffect\")", "");
			fxParamName = addStringParameter("Parameter name", "Name of the parameter, either its view name (e.g. \"My Param\") or its control name (e.g. \"myparam\")", "");
		}
	}

	setupValueParam();
	rebuildAddress();
}

ResolumeFXCommand::~ResolumeFXCommand()
{
}

void ResolumeFXCommand::rebuildAddress()
{
	int resolumeVersion = (int)resolumeModule->version->getValueData();

	if (fxType == "transform") addressSuffix = "video/" + nameParam->getValueData().toString();
	else if (fxType == "audio") addressSuffix = "audio/" + nameParam->getValueData().toString();
	else if (resolumeVersion < 6)
	{
		if (fxType == "videofx") addressSuffix = "video/effect" + fxIndexParam->stringValue() + "/param" + indexParam->stringValue();
		else if (fxType == "vst") addressSuffix = "audio/effect" + fxIndexParam->stringValue() + "/param" + indexParam->stringValue();
		else if (fxType == "source") addressSuffix = "video/param" + indexParam->stringValue();

		if (resolumeVersion == 5) addressSuffix += "/values";
	} else
	{
		String fxn = fxName != nullptr ? fxName->stringValue().toLowerCase().replace(" ", "") : "";
		String fxpn = fxParamName->stringValue().toLowerCase().replace(" ", "");

		if (fxType == "videofx") addressSuffix = "video/effects/" + fxn + "/effect/" + fxpn;
		else if (fxType == "vst") addressSuffix = "audio/effects/" + fxn + "/effect/" + fxpn;
		else if (fxType == "source") addressSuffix = "video/source/params/" + fxpn;
	}
	

	ResolumeBaseCommand::rebuildAddress();
}

void ResolumeFXCommand::onContainerParameterChanged(Parameter * p)
{
	ResolumeBaseCommand::onContainerParameterChanged(p);

	if (p == nameParam || p == fxIndexParam || p == indexParam || p == fxParamName || p == fxName)
	{
		rebuildAddress();
	} else if (p == fxParamType)
	{
		setupValueParam();
	}
}

void ResolumeFXCommand::setupValueParam()
{
	int resolumeVersion = (int)resolumeModule->version->getValueData();

	if (valueParam != nullptr)
	{
		argumentsContainer.removeControllable(valueParam);
	}

	if (resolumeVersion < 6)
	{
		valueParam = argumentsContainer.addFloatParameter("Value", "Target parameter value", 0, 0, 1);
	} else
	{
		if (fxParamType == nullptr) valueParam = new FloatParameter("Value", "Target parameter value", 0, 0, 1);
		else valueParam = dynamic_cast<Parameter *>(ControllableFactory::createControllable(fxParamType->getValueData().toString()));
		
		if (valueParam != nullptr)
		{
			argumentsContainer.addParameter(valueParam);
		}
	}

	if(valueParam != nullptr) setTargetMappingParameterAt(valueParam, 0);
}
