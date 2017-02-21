/*
  ==============================================================================

    Script.cpp
    Created: 20 Feb 2017 5:01:11pm
    Author:  Ben

  ==============================================================================
*/

#include "Script.h"
#include "ScriptEditor.h"
#include "CustomLogger.h"
#include "TargetParameter.h"

Script::Script(ScriptTarget * _parentTarget) :
	BaseItem("Script", true,false),
	parentTarget(_parentTarget),
	scriptParamsContainer("params"),
	scriptAsyncNotifier(10)
{
	isSelectable = false;

	filePath = addStringParameter("File Path", "Path to the script file", "");
	logParam = addBoolParameter("Log", "Utility parameter to easily activate/deactivate logging from the script", false);
	logParam->setCustomShortName("enableLog");

	reload = addTrigger("Reload", "Reload the script");

	scriptObject.setMethod("log", Script::logFromScript);
}

Script::~Script()
{

}

void Script::loadScript()
{
	if (filePath->stringValue().isEmpty())
	{
		return;
	}
	File f = File(filePath->stringValue());
	if (!f.exists())
	{
		NLOG("Script", "File not found : " + f.getFileName());
		setState(SCRIPT_EMPTY);
		return;
	}

	fileName = f.getFileName();
	nameParam->setValue(f.getFileNameWithoutExtension());

	String s = f.loadFileAsString();

	stopTimer();

	buildEnvironment();
	Result result = scriptEngine->execute(s);

	if (result.getErrorMessage().isEmpty())
	{
		NLOG("Script : " + niceName, "Script loaded succesfully");
		setState(SCRIPT_LOADED);
	} else
	{
		NLOG("Script : " + niceName, "Script error : " + result.getErrorMessage());
		setState(SCRIPT_ERROR);
		return;
	}

	const NamedValueSet props = scriptEngine->getRootObjectProperties();
	updateEnabled = props.contains(updateIdentifier);
	if (updateEnabled)
	{
		lastUpdateTime = (float)Time::getMillisecondCounter() / 1000.f;
		startTimerHz(50);
	}


}

void Script::buildEnvironment()
{
	scriptEngine = new JavascriptEngine();
	scriptEngine->registerNativeObject("local", createScriptObject()); //force local for this objet
	if (parentTarget != nullptr) scriptEngine->registerNativeObject("parent", parentTarget->createScriptObject()); //force parent for this object
	if (Engine::getInstanceWithoutCreating() != nullptr) scriptEngine->registerNativeObject(Engine::getInstance()->scriptTargetName, Engine::getInstance()->createScriptObject());
	if (ScriptUtil::getInstanceWithoutCreating() != nullptr) scriptEngine->registerNativeObject(ScriptUtil::getInstance()->scriptTargetName, ScriptUtil::getInstance()->createScriptObject());
}

void Script::setState(ScriptState newState)
{
	state = newState;
	scriptAsyncNotifier.addMessage(new ScriptEvent(ScriptEvent::STATE_CHANGE));
	scriptAsyncNotifier.triggerAsyncUpdate();
}


void Script::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == filePath) loadScript();
}

void Script::onContainerTriggerTriggered(Trigger * t)
{
	if (t == reload)
	{
		loadScript();
	}
}

void Script::timerCallback()
{
	if (!enabled->boolValue()) return;

	float curTime = (float)Time::getMillisecondCounter() / 1000.f;
	float deltaTime = curTime - lastUpdateTime;
	lastUpdateTime = curTime;
	Array<var> args;
	args.add(deltaTime);
	
	Result result = Result::ok();
	scriptEngine->callFunction(updateIdentifier, var::NativeFunctionArgs(var::undefined(), (const var *)args.begin(), args.size()),&result);

	if (result.getErrorMessage().isNotEmpty())
	{
		NLOG("Script : " + niceName, "Script error : " + result.getErrorMessage());
		stopTimer();
	}

}

InspectableEditor * Script::getEditor(bool isRoot)
{
	return new ScriptEditor(this,isRoot);
}

var Script::logFromScript(const var::NativeFunctionArgs & args)
{
	Script * s = ScriptTarget::getObjectFromJS<Script>(args);
	if (!s->logParam->boolValue()) return var();

	for (int i = 0; i < args.numArguments; i++)
	{
		if(i == 0) NLOG("Script : " + s->niceName, args.arguments[i].toString());
		else NLOG("",args.arguments[i].toString());
	}

	return var();
}

var Script::addBoolParameterFromScript(const var::NativeFunctionArgs & args)
{
	Script * s = getObjectFromJS<Script>(args);
	return s->scriptParamsContainer.addBoolParameter(args.arguments[0], args.arguments[1], (bool)args.arguments[2])->createScriptObject();
}

var Script::adIntParameterFromScript(const var::NativeFunctionArgs & args)
{
	Script * s = getObjectFromJS<Script>(args);
	return s->scriptParamsContainer.addIntParameter(args.arguments[0], args.arguments[1], (int)args.arguments[2], (int)args.arguments[3], (int)args.arguments[4])->createScriptObject();
}

var Script::addFloatParameterFromScript(const var::NativeFunctionArgs & args)
{

	Script * s = getObjectFromJS<Script>(args);
	return s->scriptParamsContainer.addFloatParameter(args.arguments[0], args.arguments[1], (float)args.arguments[2], (float)args.arguments[3], (float)args.arguments[4])->createScriptObject();
}

var Script::addStringParameterFromScript(const var::NativeFunctionArgs & args)
{
	Script * s = getObjectFromJS<Script>(args);
	return s->scriptParamsContainer.addStringParameter(args.arguments[0], args.arguments[1], args.arguments[2])->createScriptObject();
}

var Script::addEnumParameterFromScript(const var::NativeFunctionArgs & args)
{
	Script * s = getObjectFromJS<Script>(args);
	return s->scriptParamsContainer.addEnumParameter(args.arguments[0], args.arguments[1])->createScriptObject();
}

var Script::addTargetParameterFromScript(const var::NativeFunctionArgs & args)
{
	Script * s = getObjectFromJS<Script>(args);
	return s->scriptParamsContainer.addTargetParameter(args.arguments[0], args.arguments[1])->createScriptObject();
}
