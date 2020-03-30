/*
  ==============================================================================

    MappingLayerPanel.cpp
    Created: 20 Nov 2016 3:07:42pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "MappingLayerPanel.h"
#include "UI/ChataigneAssetManager.h"

MappingLayerPanel::MappingLayerPanel(MappingLayer * layer) :
	SequenceLayerPanel(layer),
	mappingLayer(layer)
{

	if (mappingLayer->mapping->om.outParams.size() > 0 && mappingLayer->mapping->om.outParams[0] != nullptr)
	{
		mappingOutputUI.reset(dynamic_cast<ControllableEditor*>(mappingLayer->mapping->om.outParams[0]->getEditor(false)));
		mappingOutputUI->setShowLabel(false);
		if (mappingOutputUI != nullptr) addAndMakeVisible(mappingOutputUI.get());
	}

	resized();
}

MappingLayerPanel::~MappingLayerPanel()
{
}

void MappingLayerPanel::resizedInternalContent(Rectangle<int>& r)
{
	Rectangle<int> cr = r.reduced(2, 0);

	if (mappingOutputUI != nullptr)
	{
		mappingOutputUI->setBounds(cr.removeFromTop(14));
		cr.removeFromTop(2);
	}

	resizedInternalPanelContent(cr);
}

void MappingLayerPanel::mouseDown(const MouseEvent& e)
{
	SequenceLayerPanel::mouseDown(e);

	if (e.mods.isRightButtonDown())
	{
		if (e.eventComponent == this)
		{
			PopupMenu p;
			p.addItem(1, "Export baked values to clipboard");
			p.addItem(2, "Export baked values to clipboard (data only)");

			int result = p.show();
			switch (result)
			{
			case 1:
				mappingLayer->exportBakedValues(false);
				break;

			case 2:
				mappingLayer->exportBakedValues(true);
				break;
			}
		}
	}
}