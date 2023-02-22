#include "Module/ModuleIncludes.h"

EmptyModule::EmptyModule(const String& name) :
	Module(name)
{
	setupIOConfiguration(false, false);

	alwaysShowValues = true;

	includeValuesInSave = true;
	valuesCC.userCanAddControllables = true;
	moduleParams.userCanAddControllables = true;

	canHandleRouteValues = true;
}

EmptyModule::~EmptyModule()
{
}

void EmptyModule::handleRoutedModuleValue(Controllable* c, RouteParams* p)
{
	if (c == nullptr || p == nullptr)
	{
		return;
	}

	if (EmptyRouteParams* op = dynamic_cast<EmptyRouteParams*>(p))
	{
		if (Parameter* targetParameter = dynamic_cast<Parameter*>(op->target->target.get()))
		{
			var v = dynamic_cast<Parameter*>(c)->getValue();
			targetParameter->setValue(v);
		}
	}
}

EmptyModule::EmptyRouteParams::EmptyRouteParams(Module* sourceModule, EmptyModule* dstModule, Controllable* c)
{
	target = addTargetParameter("Target", "Value to assign the routing to.", dstModule);

	// try to automatically find the corresponding value in the dstModule
	String sourceAddress;
	if (sourceModule->getTypeString() == "OSC")
	{
		sourceAddress = c->niceName;  // on CustomOSCModule, niceName is the actual address
	}
	else if (sourceModule->getTypeString() == "CustomVariables")
	{
		// On custom variables, c->getControlAddress(&sourceModule->valuesCC) -> "/groupName/varName"
		// It would probably never match anything on the destination, but "/varName" has a good change to match something.
		sourceAddress = '/' + c->niceName;
	}
	else
	{
		sourceAddress = c->getControlAddress(&sourceModule->valuesCC);
	}

	Controllable* dstControllable = dstModule->valuesCC.getControllableForAddress(sourceAddress);
	if (dstControllable)
	{
		target->setValueFromTarget(dstControllable);
	}
}
