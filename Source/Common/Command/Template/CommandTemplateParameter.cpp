/*
  ==============================================================================

    CommandTemplateParameter.cpp
    Created: 31 May 2018 11:27:54am
    Author:  Ben

  ==============================================================================
*/

CommandTemplateParameter::CommandTemplateParameter(Parameter * sourceParam) :
	ControllableContainer(sourceParam->niceName)
{
	parameter = ControllableFactory::createParameterFrom(sourceParam, true, true);
	parameter->forceSaveValue = true;
	addParameter(parameter);
	editable = addBoolParameter("Editable", "If enabled, the parameter will be editable when instantiating this template", true);
	editable->forceSaveValue = true;
	editable->saveValueOnly = false;
	saveAndLoadName = true;
}

CommandTemplateParameter::~CommandTemplateParameter()
{
}

InspectableEditor * CommandTemplateParameter::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new CommandTemplateParameterEditor(this, isRoot);
}
