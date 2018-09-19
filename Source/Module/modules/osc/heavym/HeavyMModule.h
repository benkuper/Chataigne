/*
  ==============================================================================

    HeavyMModule.h
    Created: 7 Sep 2018 12:15:30am
    Author:  Ben

  ==============================================================================
*/

#pragma once



#pragma once
#include "../OSCModule.h"

class HeavyMModule :
	public OSCModule
{
public:
	HeavyMModule();
	~HeavyMModule() {}

	static HeavyMModule * create() { return new HeavyMModule(); }
	virtual String getDefaultTypeString() const override { return "HeavyM"; }

	enum ParamType { Trigger, Range, Toggle };

	class EffectParam
	{
	public:
		EffectParam(StringRef name, StringRef address, ParamType type) :
			name(name), address(address), type(type) {}

		String name;
		String address;
		ParamType type;

	};

	class Effect
	{
	public:
		Effect(StringRef name, StringRef prefix) :
			name(name), prefix(prefix) {}
		String name;
		String prefix;
		OwnedArray<EffectParam> params;

		Effect * add(StringRef pName, StringRef pAddress, ParamType pType)
		{
			params.add(new EffectParam(pName, pAddress, pType));
			return this;
		}

		static Effect * create(StringRef name, StringRef prefix) { return new Effect(name, prefix); }
	};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeavyMModule)
};


