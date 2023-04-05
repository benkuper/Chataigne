/*
  ==============================================================================

	GenericScriptCommand.h
	Created: 6 Apr 2019 1:01:44pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class GenericScriptCommand :
	public BaseCommand
{
public:
	GenericScriptCommand(ChataigneGenericModule * _module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	~GenericScriptCommand();

	static String commandScriptTemplate;
	Script script;
	ParamLinkContainer* scriptParamContainer;

	const Identifier setValueId = "setValue";
	const Identifier triggerId = "trigger";

	virtual void updateMappingInputValue(var value, int multiplexIndex) override;
	virtual void setInputNamesFromParams(Array<WeakReference<Parameter>> outParams) override;

	void setValueInternal(var value, int multiplexIndex) override;
	void triggerInternal(int multiplexIndex) override;

	Array<var> getArgs(int multiplexIndex);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex = nullptr);
};
