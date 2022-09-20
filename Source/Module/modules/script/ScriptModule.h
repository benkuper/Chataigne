#pragma once

class ScriptModule :
	public Module
{
public: 
	ScriptModule(const String &name = "Script");
	~ScriptModule();

	// Routing support
	class ScriptRouteParams : public RouteParams
	{
	public:
		ScriptRouteParams(Module* sourceModule, ScriptModule* dstModule, Controllable* c);
		~ScriptRouteParams() {}
		TargetParameter* target;
	};

	virtual RouteParams* createRouteParamsForSourceValue(Module* sourceModule, Controllable* c, int /*index*/) override
	{
		return new ScriptRouteParams(sourceModule, this, c);
	}

	virtual void handleRoutedModuleValue(Controllable* c, RouteParams* p) override;

	virtual String getDefaultTypeString() const override { return "Script"; }
	static ScriptModule* create() { return new ScriptModule(); }
};
