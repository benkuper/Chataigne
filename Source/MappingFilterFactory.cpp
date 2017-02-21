/*
  ==============================================================================

    MappingFilterFactory.cpp
    Created: 4 Feb 2017 5:25:21pm
    Author:  Ben

  ==============================================================================
*/

#include "MappingFilterFactory.h"
#include "InverseFilter.h"
#include "CurveMapFilter.h"
#include "LagFilter.h"
#include "SimpleRemapFilter.h"
#include "SimpleSmoothFilter.h"
#include  "ScriptFilter.h"


juce_ImplementSingleton(MappingFilterFactory)

MappingFilterFactory::MappingFilterFactory()
{
	defs.add(new MappingFilterDefinition("Remap", "Inverse", &InverseFilter::create));
	defs.add(new MappingFilterDefinition("Remap", "Simple Remap", &SimpleRemapFilter::create));
	defs.add(new MappingFilterDefinition("Remap", "Curve Map", &CurveMapFilter::create));
	defs.add(new MappingFilterDefinition("", "Script", &ScriptFilter::create));

	buildPopupMenu();
}

void MappingFilterFactory::buildPopupMenu()
{
	OwnedArray<PopupMenu> subMenus;
	Array<String> subMenuNames;

	for (auto &d : defs)
	{
		int itemID = defs.indexOf(d) + 1; //start at 1 for menu

		if (d->menuPath.isEmpty())
		{
			menu.addItem(itemID, d->type);
			continue;
		}

		int subMenuIndex = -1;

		for (int i = 0; i < subMenus.size(); i++)
		{
			if (subMenuNames[i] == d->menuPath)
			{
				subMenuIndex = i;
				break;
			}
		}
		if (subMenuIndex == -1)
		{
			subMenuNames.add(d->menuPath);
			subMenus.add(new PopupMenu());
			subMenuIndex = subMenus.size() - 1;
		}

		subMenus[subMenuIndex]->addItem(itemID, d->type);
	}

	for (int i = 0; i < subMenus.size(); i++) menu.addSubMenu(subMenuNames[i], *subMenus[i]);
}
