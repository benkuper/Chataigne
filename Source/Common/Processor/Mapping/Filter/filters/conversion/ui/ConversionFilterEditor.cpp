#include "ConversionFilterEditor.h"
/*
  ==============================================================================

	ConversionFilterEditor.cpp
	Created: 4 Mar 2020 12:02:38am
	Author:  bkupe

  ==============================================================================
*/

ConversionFilterEditor::ConversionFilterEditor(ConversionFilter* filter, bool isRoot) :
	MappingFilterEditor(filter, isRoot),
	cf(filter),
	cpmEditor(&filter->cpm, false)
{
	addAndMakeVisible(&cpmEditor);

	cf->addAsyncCoalescedConversionFilterListener(this);
	cf->cpm.addAsyncManagerListener(this);
	cpmEditor.addMouseListener(this,true);
	
	rebuildSourcesUI();
	rebuildLinksUI();
}

ConversionFilterEditor::~ConversionFilterEditor()
{
	if (!inspectable.wasObjectDeleted())
	{
		cf->removeAsyncConversionFilterListener(this);
		cf->cpm.removeAsyncManagerListener(this);
	}
}

void ConversionFilterEditor::setCollapsed(bool value, bool force, bool animate, bool doNotRebuild)
{
	MappingFilterEditor::setCollapsed(value, force, animate, doNotRebuild);
	cpmEditor.setVisible(!filter->editorIsCollapsed);
	if(!doNotRebuild) rebuildSourcesUI();
}

void ConversionFilterEditor::resizedInternalContent(Rectangle<int>& r)
{
	Rectangle<int> fullRect(r);

	cpmEditor.setBounds(r.removeFromRight(jmin<int>(getWidth() / 2.5, 300)).withHeight(cpmEditor.getHeight()));

	Rectangle<int> sr = r.removeFromLeft(getWidth() / 2.5);
	for (auto& sui : sourcesUI)
	{
		sui->setBounds(sr.withHeight(sui->getHeight()));
		sr.translate(0, sui->getHeight() + 4);
	}

	{
		GenericScopedLock lock(linksUI.getLock());
		for (auto& linkUI : linksUI)
		{
			if (linkUI->sourceConnector.wasObjectDeleted() || linkUI->sourceConnector == nullptr 
				|| linkUI->outConnector.wasObjectDeleted() || linkUI->outConnector == nullptr) continue;

			Rectangle<int> lcr = getLocalArea(linkUI->sourceConnector, linkUI->sourceConnector->getLocalBounds());
			Rectangle<int> lcr2 = getLocalArea(linkUI->outConnector, linkUI->outConnector->getLocalBounds());

			linkUI->setBounds(lcr.getUnion(lcr2).expanded(10));
		}
	}

	r.setY(jmax(sr.getY(), cpmEditor.getBottom()));

	fullRect.setBottom(r.getBottom());
	if (editingLinkUI != nullptr)
	{
		editingLinkUI->setBounds(fullRect);
	}

}

void ConversionFilterEditor::rebuildSourcesUI()
{
	for (auto& sui : sourcesUI) removeChildComponent(sui);
	sourcesUI.clear();

	if (!filter->editorIsCollapsed)
	{
		{
			GenericScopedLock lock(linksUI.getLock());
			for (auto& l : linksUI) removeChildComponent(l);
			linksUI.clear();
		}


		Array<WeakReference<Parameter>> previewSourceParams = cf->sourceParams[cf->getPreviewIndex()];
		for (auto& s : previewSourceParams)
		{
			if (s == nullptr || s.wasObjectDeleted()) continue;

			ConversionSourceParameterUI* sui = new ConversionSourceParameterUI(s);
			sui->addMouseListener(this, true);
			addAndMakeVisible(sui);
			sourcesUI.add(sui);
		}
	}

	rebuildLinksUI();
	resized();
}

void ConversionFilterEditor::rebuildLinksUI()
{
	{
		GenericScopedLock lock(linksUI.getLock());

		for (auto& l : linksUI) removeChildComponent(l);
		linksUI.clear();

		if (filter->editorIsCollapsed) return;

		for (auto& l : cf->links)
		{
			ConversionSourceParameterUI* sui = sourcesUI[l->sourceIndex];
			ConvertedParameterEditor* cpe = (ConvertedParameterEditor*)cpmEditor.getEditorForInspectable(l->out);
			//jassert(sui != nullptr && cpe != nullptr && sui->connectors.size() > l->sourceValueIndex);
			if (sui == nullptr) continue;
			ConversionParamValueLinkUI* linkUI = new ConversionParamValueLinkUI(sui->connectors[l->sourceValueIndex], cpe->connectors[l->outValueIndex], l);
			addAndMakeVisible(linkUI);
			linksUI.add(linkUI);
		}
	}

	resized();
}

void ConversionFilterEditor::mouseDown(const MouseEvent& e)
{

	if (ConversionConnector* c = dynamic_cast<ConversionConnector*>(e.eventComponent))
	{
		editingLinkUI.reset(new ConversionParamValueLinkUI(c->isSource ? c : nullptr, c->isSource ? nullptr : c, nullptr));
		addAndMakeVisible(editingLinkUI.get());
		resized();
	}
	else
	{
		MappingFilterEditor::mouseDown(e);
	}
}

void ConversionFilterEditor::mouseDrag(const MouseEvent& e)
{
	if (e.eventComponent == this) MappingFilterEditor::mouseMove(e);
	
	if (ConversionConnector* c = dynamic_cast<ConversionConnector*>(e.eventComponent))
	{
		Array<ConversionConnector*> targetConnectors;
		if (c->isSource)
		{
			for (auto& ce : cpmEditor.childEditors)
			{
				if (ConvertedParameterEditor* cpe = dynamic_cast<ConvertedParameterEditor*>(ce))
				{
					for (auto& tc : cpe->connectors) targetConnectors.add(tc);
				}
			}
		}
		else
		{
			for (auto& sui : sourcesUI)
			{
				for (auto& tc : sui->connectors) targetConnectors.add(tc);
			}
		}

		bool found = false;
		for (auto& tc : targetConnectors)
		{
			if (tc->contains(tc->getMouseXYRelative()))
			{
				if (c->isSource) editingLinkUI->setOutConnector(tc);
				else editingLinkUI->setSourceConnector(tc);
				found = true;
				break;
			}
		}

		if (!found)
		{
			if (c->isSource) editingLinkUI->setOutConnector(nullptr);
			else editingLinkUI->setSourceConnector(nullptr);
		}

		if (editingLinkUI != nullptr) editingLinkUI->repaint();
	}
	
}

void ConversionFilterEditor::mouseUp(const MouseEvent& e)
{
	if(e.eventComponent == this) MappingFilterEditor::mouseUp(e);

	if (ConversionConnector* c = dynamic_cast<ConversionConnector*>(e.eventComponent))
	{
		if (editingLinkUI != nullptr)
		{
			if (editingLinkUI->sourceConnector != nullptr && editingLinkUI->outConnector != nullptr)
			{
				WeakReference<Parameter> sourceP = editingLinkUI->sourceConnector->param;
				int sourceIndex = cf->sourceParams[cf->getPreviewIndex()].indexOf(sourceP); //check index in array of the preview index because this is the one we show
				cf->createLink(sourceIndex, editingLinkUI->sourceConnector->valueIndex,
					editingLinkUI->outConnector->convertedParam, editingLinkUI->outConnector->valueIndex);
			}
			removeChildComponent(editingLinkUI.get());
			editingLinkUI.reset();
		}
	}
}

void ConversionFilterEditor::newMessage(const ConversionFilter::ConversionFilterEvent& e)
{
	switch (e.type)
	{
	case ConversionFilter::ConversionFilterEvent::SOURCES_UPDATED:
		rebuildSourcesUI();
		break;

	case ConversionFilter::ConversionFilterEvent::LINKS_UPDATED:
		rebuildLinksUI();
		break;
	}
}

void ConversionFilterEditor::newMessage(const ConvertedParameterManager::BManagerEvent& e)
{
	rebuildLinksUI();
}
