/*
  ==============================================================================

    ChataigneGenericDashboardCommand.cpp
    Created: 11 Sep 2021 7:17:37pm
    Author:  bkupe

  ==============================================================================
*/

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
			if (DashboardManagerView* v = ShapeShifterManager::getInstance()->getContentForType<DashboardManagerView>())
			{
				v->setCurrentDashboard(d);
			}
		}
	}
	break;

	case SELECT_DASHBOARD_INDEX:
	{
		int index = (int)getLinkedValue(target, multiplexIndex);
		if (index >= 0 && index < DashboardManager::getInstance()->items.size())
		{
			Dashboard* d = DashboardManager::getInstance()->items[index];
			if (DashboardManagerView* v = ShapeShifterManager::getInstance()->getContentForType<DashboardManagerView>())
			{
				v->setCurrentDashboard(d);
			}
		}
	}
	break;
	}
}

BaseCommand* ChataigneDashboardCommand::create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex)
{
	return new ChataigneDashboardCommand((ChataigneGenericModule*)module, context, params, multiplex);
}