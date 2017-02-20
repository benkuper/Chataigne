/*
  ==============================================================================

    Script.h
    Created: 20 Feb 2017 5:01:11pm
    Author:  Ben

  ==============================================================================
*/

#ifndef SCRIPT_H_INCLUDED
#define SCRIPT_H_INCLUDED

#include "BaseItem.h"

class Script :
	public BaseItem,
	public JavascriptEngine
{
public:
	Script();
	~Script();

	enum ScriptState { LOADED, ERROR, EMPTY };
	
	StringParameter * filePath;
	BoolParameter * log;
	Trigger * reload;

	ScriptState state;
	String fileName;
	
	void loadScript();

	void onContainerParameterChangedInternal(Parameter *) override;
	void onContainerTriggerTriggered(Trigger *) override;

	class ScriptEvent
	{
		public:
		enum Type { STATE_CHANGE };
		ScriptEvent(Type _type) : type(_type){}
		Type type;
	};

	QueuedNotifier<ScriptEvent> scriptAsyncNotifier;
	typedef QueuedNotifier<ScriptEvent>::Listener AsyncListener;


	void addAsyncScriptListener(AsyncListener* newListener) { scriptAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedScriptListener(AsyncListener* newListener) { scriptAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncScriptListener(AsyncListener* listener) { scriptAsyncNotifier.removeListener(listener); }

	InspectableEditor * getEditor(bool isRoot) override;
};

#endif  // SCRIPT_H_INCLUDED
