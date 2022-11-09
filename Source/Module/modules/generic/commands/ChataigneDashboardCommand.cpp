/*
  ==============================================================================

    ChataigneGenericDashboardCommand.cpp
    Created: 11 Sep 2021 7:17:37pm
    Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

ChataigneDashboardCommand::ChataigneDashboardCommand(ChataigneGenericModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	target(nullptr)
{
	type = (Type)(int)(params.getProperty("type", SELECT_DASHBOARD));

	if (type == SELECT_DASHBOARD)
	{

		target = addTargetParameter("Dashboard", "The target dashboard", DashboardManager::getInstance());
		((TargetParameter*)target)->targetType = TargetParameter::CONTAINER;
		((TargetParameter*)target)->maxDefaultSearchLevel = 0;
		if (ParameterLink* pl = getLinkedParam(target)) pl->canLinkToMapping = false;
	}
	else if (type == SELECT_DASHBOARD_INDEX)
	{
		target = addIntParameter("Index", "The index of the dashboard to show", 0, 0);
	}

	setInClients = addBoolParameter("Set in Clients", "If checked, this will notify any connected client to switch to the same dashboard", false);
}

ChataigneDashboardCommand::~ChataigneDashboardCommand()
{
}

void ChataigneDashboardCommand::triggerInternal(int multiplexIndex)
{
	switch (type)
	{
	case SELECT_DASHBOARD:
	{
		if (Dashboard* d = getLinkedTargetContainerAs<Dashboard>((TargetParameter*)target, multiplexIndex))
		{
			DashboardManager::getInstance()->setCurrentDashboard(d, setInClients->boolValue(), true);
		}
	}
	break;

	case SELECT_DASHBOARD_INDEX:
	{
		int index = (int)getLinkedValue(target, multiplexIndex);
		if (index >= 0 && index < DashboardManager::getInstance()->items.size())
		{
			Dashboard* d = DashboardManager::getInstance()->items[index];
			DashboardManager::getInstance()->setCurrentDashboard(d, setInClients->boolValue(), true);

		}
	}
	break;
	}
}

BaseCommand* ChataigneDashboardCommand::create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex)
{
	return new ChataigneDashboardCommand((ChataigneGenericModule*)module, context, params, multiplex);
}