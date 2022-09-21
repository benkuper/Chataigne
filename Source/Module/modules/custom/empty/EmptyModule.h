#pragma once

class EmptyModule :
	public Module
{
public: 
	EmptyModule(const String &name = "Empty");
	~EmptyModule();

	// Routing support
	class EmptyRouteParams : public RouteParams
	{
	public:
		EmptyRouteParams(Module* sourceModule, EmptyModule* dstModule, Controllable* c);
		~EmptyRouteParams() {}
		TargetParameter* target;
	};

	virtual RouteParams* createRouteParamsForSourceValue(Module* sourceModule, Controllable* c, int /*index*/) override
	{
		return new EmptyRouteParams(sourceModule, this, c);
	}

	virtual void handleRoutedModuleValue(Controllable* c, RouteParams* p) override;

	virtual String getDefaultTypeString() const override { return "Empty"; }
	static EmptyModule* create() { return new EmptyModule(); }
};
