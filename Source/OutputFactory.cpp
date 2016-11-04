/*
  ==============================================================================

    OutputFactory.cpp
    Created: 4 Nov 2016 12:28:44am
    Author:  bkupe

  ==============================================================================
*/

#include "OutputFactory.h"

#include "CustomOSCOutput.h"
#include "ResolumeOutput.h"

juce_ImplementSingleton(OutputFactory)

OutputFactory::OutputFactory() {
	outputDefs.add(new OutputDefinition("Generic", "OSC", &CustomOSCOutput::create));
	outputDefs.add(new OutputDefinition("Video","Resolume",&ResolumeOutput::create));


	buildPopupMenu();
}

inline void OutputFactory::buildPopupMenu()
{
	OwnedArray<PopupMenu> subMenus;
	Array<String> subMenuNames;

	for (auto &d : outputDefs)
	{
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

		int itemID = outputDefs.indexOf(d) + 1;//start at 1 for menu
		subMenus[subMenuIndex]->addItem(itemID, d->outputType);
	}

	for (int i = 0; i < subMenus.size(); i++) menu.addSubMenu(subMenuNames[i], *subMenus[i]);
}
