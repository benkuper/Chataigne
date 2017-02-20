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
#include "Engine.h"

Script::Script(ScriptTarget * _parentTarget) :
	BaseItem("Script", true,false),
	parentTarget(_parentTarget),
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
		state = EMPTY;
		return;
	}

	fileName = f.getFileName();
	nameParam->setValue(f.getFileNameWithoutExtension());

	
	String s = f.loadFileAsString();

	buildEnvironment();
	Result result = scriptEngine->execute(s);

	if (result.getErrorMessage().isEmpty())
	{
		NLOG("Script : " + niceName, "Script loaded succesfully");
		state = LOADED;
	} else
	{
		NLOG("Script : " + niceName, "Script error : " + result.getErrorMessage());
		state = ERROR;
	}

}

void Script::buildEnvironment()
{
	scriptEngine = new JavascriptEngine();
	scriptEngine->registerNativeObject("this", createScriptObject()); //force local for this objet
	if (Engine::getInstanceWithoutCreating() != nullptr) scriptEngine->registerNativeObject(Engine::getInstance()->scriptTargetName, Engine::getInstance()->createScriptObject());
	if (parentTarget != nullptr) scriptEngine->registerNativeObject("parent", parentTarget->createScriptObject()); 
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
