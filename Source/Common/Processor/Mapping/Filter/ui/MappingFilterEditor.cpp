/*
  ==============================================================================

    MappingFilterEditor.cpp
    Created: 4 Feb 2017 8:31:15pm
    Author:  Ben

  ==============================================================================
*/

MappingFilterEditor::MappingFilterEditor(MappingFilter * m, bool isRoot) :
	BaseItemEditor(m,isRoot),
	filter(m),
	filteredUI(nullptr)
{
	updateFilteredUI();
	filter->addAsyncFilterListener(this);
}

MappingFilterEditor::~MappingFilterEditor()
{
	if (!inspectable.wasObjectDeleted()) filter->removeAsyncFilterListener(this);
}

void MappingFilterEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	if (filteredUI != nullptr) filteredUI->setBounds(r.removeFromRight(140).reduced(2));
}


void MappingFilterEditor::updateFilteredUI()
{
	int pi = filter->getPreviewIndex();
	OwnedArray<Parameter>* fParams = filter->filteredParameters[pi];
	Parameter* targetParam = fParams != nullptr && fParams->size() > 0 ? fParams->getUnchecked(0) : nullptr;

	if (filteredUI != nullptr && filteredUI->parameter == targetParam) return;

	if (filteredUI != nullptr)
	{
		removeChildComponent(filteredUI.get());
	}

	filteredUI.reset();

	if (targetParam != nullptr)
	{
		filteredUI.reset((ParameterUI*)(targetParam->createDefaultUI()));
		filteredUI->showLabel = false;
		addAndMakeVisible(filteredUI.get());
	}

	resized();
}

void MappingFilterEditor::newMessage(const MappingFilter::FilterEvent& e)
{
	if (e.type == MappingFilter::FilterEvent::FILTER_REBUILT)
	{
		updateFilteredUI();
	}
}