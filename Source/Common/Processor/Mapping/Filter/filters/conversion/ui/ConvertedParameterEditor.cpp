/*
  ==============================================================================

    ConvertedParameterEditor.cpp
    Created: 4 Mar 2020 12:04:38am
    Author:  bkupe

  ==============================================================================
*/

#include "ConvertedParameterEditor.h"

ConvertedParameterEditor::ConvertedParameterEditor(ConvertedParameter* cp, bool isRoot) :
	BaseItemEditor(cp, isRoot),
	cp(cp)
{

	for (auto& e : childEditors)
	{
		if (ControllableEditor* ce = dynamic_cast<ControllableEditor*>(e))
		{
			ce->setShowLabel(false);
		}
	}

	if (cp->outParamReference != nullptr && !cp->outParamReference.wasObjectDeleted())
	{
		outParamUI.reset((ParameterUI*)cp->outParamReference->createDefaultUI());
		addAndMakeVisible(outParamUI.get());
	}

	StringArray valueNames = cp->defaultParam->getValuesNames();
	for (int i = 0; i < valueNames.size(); i++)
	{
		ConversionConnector* cc = new ConversionConnector(valueNames[i], cp, i);
		addAndMakeVisible(cc);
		connectors.add(cc);
	}

	cp->addAsyncCPListener(this);

	resized();
}

ConvertedParameterEditor::~ConvertedParameterEditor()
{
	if(!inspectable.wasObjectDeleted()) cp->removeAsyncCPListener(this);
}

InspectableEditor* ConvertedParameterEditor::addEditorUI(ControllableContainer* cc, bool resize)
{
	InspectableEditor * e = BaseItemEditor::addEditorUI(cc, resize);
	if (ControllableEditor* ce = dynamic_cast<ControllableEditor*>(e))
	{
		ce->setShowLabel(false);
	}
	return e;
}

void ConvertedParameterEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	if(outParamUI != nullptr) outParamUI->setBounds(r.removeFromRight(60).reduced(2));
}

void ConvertedParameterEditor::resizedInternalContent(Rectangle<int>& r)
{
	if (connectors.size() == 0 || getHeight() == 0) return;

	const int connectorHeight = 12;
	const int margin = 6;
	const int minHeight = connectors.size() * connectorHeight + margin;

	int top = r.getY();
	Rectangle<int> cr = r.removeFromLeft(50);
	BaseItemEditor::resizedInternalContent(r);
	r.setY(jmax(r.getY(), top + minHeight));
	cr.setHeight(r.getY() - top);


	for (int i = 0; i < connectors.size(); i++)
	{
		int th = connectors.size() == 1?cr.getHeight()/2:margin + (cr.getHeight() - margin*2) * i * 1.0f / (connectors.size() - 1);
		connectors[i]->setBounds(Rectangle<int>(5, cr.getY() + th- connectorHeight /2, cr.getWidth(), connectorHeight));
	}

}

void ConvertedParameterEditor::newMessage(const ConvertedParameter::CPEvent& e)
{
	if (e.type == ConvertedParameter::CPEvent::OUT_PARAM_CHANGED)
	{
		if (outParamUI != nullptr) removeChildComponent(outParamUI.get());
		if (cp->outParamReference != nullptr && !cp->outParamReference.wasObjectDeleted())
		{
			outParamUI.reset((ParameterUI*)cp->outParamReference->createDefaultUI());
			addAndMakeVisible(outParamUI.get());
			resized();
		}
	}

}
