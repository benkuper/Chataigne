/*
  ==============================================================================

    ScriptManager.h
    Created: 20 Feb 2017 5:00:57pm
    Author:  Ben

  ==============================================================================
*/

#ifndef SCRIPTMANAGER_H_INCLUDED
#define SCRIPTMANAGER_H_INCLUDED

#include "Script.h"
#include "BaseManager.h"

class ScriptManager :
	public BaseManager<Script>
{
public:
	ScriptManager(ScriptTarget * parentTarget);
	~ScriptManager();

	ScriptTarget * parentTarget;

	Script * createItem() override;

	bool callFunctionOnAllItems(const Identifier &function, var a = var());
	bool callFunctionOnAllItems(const Identifier &function, Array<var> args);
};


#endif  // SCRIPTMANAGER_H_INCLUDED
