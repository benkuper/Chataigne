/*
  ==============================================================================

    ConvertedParameterManager.cpp
    Created: 3 Mar 2020 10:30:51pm
    Author:  bkupe

  ==============================================================================
*/


ConvertedParameterManager::ConvertedParameterManager() :
	BaseManager("Converted Parameters")
{
	managerFactory = &factory;
	selectItemWhenCreated = false;
	editorCanBeCollapsed = false;

	factory.defs.add(Factory<ConvertedParameter>::Definition::createDef("", BoolParameter::getTypeStringStatic(), &ConvertedParameter::create)->addParam("type", BoolParameter::getTypeStringStatic()));
	factory.defs.add(Factory<ConvertedParameter>::Definition::createDef("", FloatParameter::getTypeStringStatic(), &ConvertedParameter::create)->addParam("type", FloatParameter::getTypeStringStatic()));
	factory.defs.add(Factory<ConvertedParameter>::Definition::createDef("", IntParameter::getTypeStringStatic(), &ConvertedParameter::create)->addParam("type", IntParameter::getTypeStringStatic()));
	factory.defs.add(Factory<ConvertedParameter>::Definition::createDef("", StringParameter::getTypeStringStatic(), &ConvertedParameter::create)->addParam("type", StringParameter::getTypeStringStatic()));
	factory.defs.add(Factory<ConvertedParameter>::Definition::createDef("", ColorParameter::getTypeStringStatic(), &ConvertedParameter::create)->addParam("type", ColorParameter::getTypeStringStatic()));
	factory.defs.add(Factory<ConvertedParameter>::Definition::createDef("", Point2DParameter::getTypeStringStatic(), &ConvertedParameter::create)->addParam("type", Point2DParameter::getTypeStringStatic()));
	factory.defs.add(Factory<ConvertedParameter>::Definition::createDef("", Point3DParameter::getTypeStringStatic(), &ConvertedParameter::create)->addParam("type", Point3DParameter::getTypeStringStatic()));
}

ConvertedParameterManager::~ConvertedParameterManager()
{
}

InspectableEditor* ConvertedParameterManager::getEditorInternal(bool isRoot)
{
	return new ConvertedParameterManagerEditor(this, isRoot);
}
