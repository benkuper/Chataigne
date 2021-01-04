/*
  ==============================================================================

    ResolumeFXCommand.cpp
    Created: 19 Feb 2017 1:42:30pm
    Author:  Ben

  ==============================================================================
*/

#include "ResolumeFXCommand.h"
#include "../ResolumeModule.h"

ResolumeFXCommand::ResolumeFXCommand(ResolumeModule * _module, CommandContext context, var params, Multiplex * multiplex) :
	ResolumeBaseCommand(_module,context,params, multiplex, true),
    nameParam(nullptr),
    fxIndexParam(nullptr),
    valueParam(nullptr),
    indexParam(nullptr),
    fxParamType(nullptr),
    fxName(nullptr),
	fxParamName(nullptr)
{
	rebuildParameters();
}

ResolumeFXCommand::~ResolumeFXCommand()
{
}


void ResolumeFXCommand::rebuildParametersInternal()
{
	float resolumeVersion = (float)resolumeModule->version->getValueData();

	//Set to null for later checks
	fxParamName = nullptr;
	fxParamType = nullptr;
	fxIndexParam = nullptr;
	fxName = nullptr;
	indexParam = nullptr;
	nameParam = nullptr;

	fxType = params.getProperty("fxType", "transform");

	if (fxType == "video" || fxType == "audio")
	{
		nameParam = addEnumParameter("Parameter", "Name of the target parameter in the transform menu");
		if (fxType == "video")
		{
			nameParam->addOption("Opacity", "opacity");
			nameParam->addOption("Width", "width");
			nameParam->addOption("Height", "height");


			String suf = resolumeVersion >= 6 ? "effects/transform/" : "";

			nameParam->addOption("Scale", suf + "scale");
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

			Level level = levelParam->getValueDataAsEnum<Level>();
			if (level == LAYER || level == SELECTED_LAYER)
			{
				nameParam->addOption("Transition", "transition/mixer/blendmode");
				nameParam->addOption("Transition", "transition/duration");
			}
		} else
		{
			nameParam->addOption("Volume", "volume");
			nameParam->addOption("Pan", "pan");
		}
	} else if (fxType == "videofx" || fxType == "vst" || fxType == "source")
	{
		if (resolumeVersion < 6)
		{
			if (fxType != "source") fxIndexParam = addIntParameter("Effect ID", "Id of the effect, depending on its position in the effect chain", 1, 1, 100);
			indexParam = addIntParameter("Parameter ID", "Id of the parameter, depending on its position inside in the panel", 1, 1, 100);
		} else //Resolume 6+
		{

			fxParamType = addEnumParameter("Parameter type", "The type of parameter");
			fxParamType->addOption("Float", FloatParameter::getTypeStringStatic())->addOption("Integer", IntParameter::getTypeStringStatic())
				->addOption("Boolean", BoolParameter::getTypeStringStatic())->addOption("String", StringParameter::getTypeStringStatic())
				->addOption("Color", ColorParameter::getTypeStringStatic());
			
			if (resolumeVersion > 6 || (resolumeVersion == 6 && fxType != "source")) fxName = addStringParameter("Effect name", "Name of the effect, either its view name (e.g. \"My Effect\") or its control name (e.g. \"myeffect\")", "myeffect");
			fxParamName = addStringParameter("Parameter name", "Name of the parameter, either its view name (e.g. \"My Param\") or its control name (e.g. \"myparam\")", "myparam");
		}
	}

	if(nameParam != nullptr) resolumeControllables.add(nameParam);
	if(fxParamName != nullptr) resolumeControllables.add(fxParamName);
	if(fxIndexParam != nullptr) resolumeControllables.add(fxIndexParam);
	if(fxParamType != nullptr) resolumeControllables.add(fxParamType);
	if(fxName != nullptr) resolumeControllables.add(fxName);
	if(indexParam != nullptr) resolumeControllables.add(indexParam);

	setupValueParam();
}

String ResolumeFXCommand::getTargetAddress(int multiplexIndex)
{
	float resolumeVersion = (float)resolumeModule->version->getValueData();

	String nameParamValue = nameParam != nullptr ? getLinkedValue(nameParam, multiplexIndex) : "";
	
	if (resolumeVersion == 5)
	{
		String paramId = indexParam == nullptr ? "[error]": getLinkedValue(indexParam, multiplexIndex);

		if (fxType == "video")
		{
			if (nameParamValue == "transition/duration") addressSuffix = nameParamValue;
			else addressSuffix = "video/" + nameParamValue;
		}
		else if (fxType == "audio") addressSuffix = "audio/" + nameParamValue;
		else if (fxType == "videofx") addressSuffix = "video/effect" + getLinkedValue(fxIndexParam, multiplexIndex).toString() + "/param" + paramId;
		else if (fxType == "vst") addressSuffix = "audio/effect" + getLinkedValue(fxIndexParam, multiplexIndex).toString() + "/param" + paramId;
		else if (fxType == "source") addressSuffix = "video/param" + paramId;
		 
		addressSuffix += "/values";
	} else //Resolume 6+
	{
		String fxn = fxName != nullptr ? getLinkedValue(fxName, multiplexIndex).toString().toLowerCase().replace(" ", "") : "";
		String fxpn = fxParamName != nullptr ? getLinkedValue(fxParamName, multiplexIndex).toString().toLowerCase().replace(" ", "") : "";
		String sourceName = resolumeVersion == 6 ? "params" : fxn;
		String effectSeparator = "/effect/";
		if (fxpn == "opacity") effectSeparator = "/";
		else if (fxpn == "blendmode") effectSeparator = "/mixer/";

		if (fxType == "video")
		{
			if (nameParamValue == "transition/duration") addressSuffix = nameParamValue;
			else addressSuffix = "video/" + nameParamValue;
		}
		else if (fxType == "audio") addressSuffix = "audio/" + nameParamValue;
		else if (fxType == "videofx") addressSuffix = "video/effects/" + fxn + effectSeparator + fxpn;
		else if (fxType == "vst") addressSuffix = "audio/effects/" + fxn + effectSeparator + fxpn;
		else if (fxType == "source") addressSuffix = "video/source/"+sourceName+"/" + fxpn;
	}
	
	return ResolumeBaseCommand::getTargetAddress(multiplexIndex);
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
	float resolumeVersion = (float)resolumeModule->version->getValueData();

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
		valueParam->setNiceName("Value");
		if (valueParam != nullptr)
		{
			if (valueParam->type == Controllable::STRING)
			{
				((StringParameter *)valueParam)->defaultUI = StringParameter::TEXT;
				((StringParameter *)valueParam)->multiline = true;
			}

			argumentsContainer.addParameter(valueParam);
		}
	}

	if(valueParam != nullptr) linkParamToMappingIndex(valueParam, 0);
}
