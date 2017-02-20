/*
  ==============================================================================

    Script.cpp
    Created: 20 Feb 2017 5:01:11pm
    Author:  Ben

  ==============================================================================
*/

#include "Script.h"
#include "ScriptEditor.h"

Script::Script() :
	BaseItem("Script", true,false),
	scriptAsyncNotifier(10)
{
	isSelectable = false;

	filePath = addStringParameter("File Path", "Path to the script file", "");
	log = addBoolParameter("Log", "Utility parameter to easily activate/deactivate logging from the script", false);

	reload = addTrigger("Reload", "Reload the script");
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

	Result result = execute(s);

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


void Script::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == filePath) loadScript();
}

void Script::onContainerTriggerTriggered(Trigger * t)
{
	if (t == reload)
	{
		DBG("LOAD !");
		loadScript();
	}
}

InspectableEditor * Script::getEditor(bool isRoot)
{
	return new ScriptEditor(this,isRoot);
}
