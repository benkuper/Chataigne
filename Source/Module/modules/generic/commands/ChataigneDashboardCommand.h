/*
  ==============================================================================

    ChataigneGenericDashboardCommand.h
    Created: 11 Sep 2021 7:17:37pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ChataigneDashboardCommand :
	public BaseCommand
{
public:
	ChataigneDashboardCommand(ChataigneGenericModule* _module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	~ChataigneDashboardCommand();

	enum Type { SELECT_DASHBOARD, SELECT_DASHBOARD_INDEX };
	Type type;

	Parameter* target;

	void triggerInternal(int multiplexIndex) override;

	static BaseCommand* create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex = nullptr);
};
