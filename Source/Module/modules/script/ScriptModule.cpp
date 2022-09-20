ScriptModule::ScriptModule(const String & name) :
	Module(name)
{
	setupIOConfiguration(false, false);

	includeValuesInSave = true;
	valuesCC.userCanAddControllables = true;
	moduleParams.userCanAddControllables = true;

	canHandleRouteValues = true;
}

ScriptModule::~ScriptModule()
{
}

void ScriptModule::handleRoutedModuleValue(Controllable* c, RouteParams* p)
{
	if (c == nullptr || p == nullptr)
	{
		return;
	}
	
	if (ScriptRouteParams* op = dynamic_cast<ScriptRouteParams*>(p))
	{
		if (Parameter* targetParameter = dynamic_cast<Parameter*>(op->target->target.get()))
		{
			var v = dynamic_cast<Parameter*>(c)->getValue();
			targetParameter->setValue(v);
		}
	}
}

ScriptModule::ScriptRouteParams::ScriptRouteParams(Module* sourceModule, ScriptModule* dstModule, Controllable* c)
{
	target = addTargetParameter("Target", "Value to assign the routing to.", dstModule);

	// try to automatically find the corresponding value in the dstModule
	String sourceAddress;
	if (sourceModule->getTypeString() == "OSC")
	{
		sourceAddress = c->niceName;  // on CustomOSCModule, niceName is the actual address
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
