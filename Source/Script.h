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
#include "ScriptTarget.h"
#include "Engine.h"
#include "ScriptUtil.h"

class Script :
	public BaseItem,
	public Timer
{
public:
	Script(ScriptTarget * parentTarget = nullptr, bool canBeDisabled = true);
	~Script();

	enum ScriptState { SCRIPT_LOADED, SCRIPT_ERROR, SCRIPT_EMPTY, SCRIPT_CLEAR };
	
	StringParameter * filePath;
	BoolParameter * logParam;
	Trigger * reload;

	ScriptState state;
	String fileName;
	
	bool updateEnabled; //When loading the script, checks if the update function is present
	float lastUpdateTime;
	const Identifier updateIdentifier = "update";

	ControllableContainer scriptParamsContainer;

	ScriptTarget * parentTarget;

	ScopedPointer<JavascriptEngine> scriptEngine;
	
	void loadScript();
	void buildEnvironment();

	void setState(ScriptState newState);

	bool callFunction(const Identifier &function, const Array<var> args);

	void onContainerParameterChangedInternal(Parameter *) override;
	void onContainerTriggerTriggered(Trigger *) override;

	void controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;

	// Inherited via Timer
	virtual void timerCallback() override;

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

	//Script functions
	static var logFromScript(const var::NativeFunctionArgs &args);

	static var addBoolParameterFromScript(const var::NativeFunctionArgs &args);
	static var addIntParameterFromScript(const var::NativeFunctionArgs &args);
	static var addFloatParameterFromScript(const var::NativeFunctionArgs &args);
	static var addStringParameterFromScript(const var::NativeFunctionArgs &args);
	static var addEnumParameterFromScript(const var::NativeFunctionArgs &args);
	static var addTargetParameterFromScript(const var::NativeFunctionArgs &args);
	static var addTriggerFromScript(const var::NativeFunctionArgs &args);

	static bool checkNumArgs(const String &logName, const var::NativeFunctionArgs &args, int expectedArgs);
	

};

#endif  // SCRIPT_H_INCLUDED
