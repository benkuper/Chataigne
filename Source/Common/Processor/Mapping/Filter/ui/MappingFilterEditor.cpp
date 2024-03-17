/*
  ==============================================================================

    MappingFilterEditor.cpp
    Created: 4 Feb 2017 8:31:15pm
    Author:  Ben

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

MappingFilterEditor::MappingFilterEditor(MappingFilter * m, bool isRoot) :
	BaseItemEditor(m,isRoot),
	filter(m),
	filteredUI(nullptr)
{
    if (filter->canFilterChannels)
    {
        channelEdit.reset(new ChannelSelectorUI(filter));
        addAndMakeVisible(channelEdit.get());
    }

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

    if (channelEdit != nullptr)
    {
        channelEdit->setVisible(filter->sourceParams[0].size() > 1);
        if(channelEdit->isVisible()) channelEdit->setBounds(r.removeFromRight(60).reduced(2));
    }
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
        if (targetParam->type == Parameter::POINT2D)
        {
            if (DoubleSliderUI* dui = dynamic_cast<DoubleSliderUI*>(filteredUI.get())) dui->canShowExtendedEditor = false;
        }
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

ChannelSelectorUI::ChannelSelectorUI(MappingFilter* f) :
    TextButton("Channels", "Choose which channels will be processed by this filter"),
    filter(f),
    model(f)
{
}

ChannelSelectorUI::~ChannelSelectorUI()
{
}

void ChannelSelectorUI::clicked()
{
    model.removeModelListener(this);

    std::unique_ptr<ListBox> box(new ListBox());
    listBox = box.get();

    int numTypes = filter->sourceParams[0].size();
    box->setModel(&model);
    box->setRowHeight(20);
    box->setBounds(0, 0, 100, 20 * numTypes);
    box->setMultipleSelectionEnabled(true);
    box->setClickingTogglesRowSelection(true);

    SparseSet<int> rows;
    for (int i = 0; i < numTypes; i++) {
        if (!filter->excludedChannels.contains(i)) rows.addRange(Range<int>(i, i + 1));
    }
    box->setSelectedRows(rows);

    CallOutBox::launchAsynchronously(std::move(box), localAreaToGlobal(getLocalBounds()), nullptr);

    model.addModelListener(this);

}

void ChannelSelectorUI::selectionChanged()
{
    Array<int> channels;
    for (int i = 0; i < filter->sourceParams[0].size(); i++) if(!listBox->isRowSelected(i)) channels.add(i);
    filter->setExcludedChannels(channels);
}


// MODEL

ChannelListModel::ChannelListModel(MappingFilter * f) :
    filter(f)
{
}

ChannelListModel::~ChannelListModel()
{
}

void ChannelListModel::selectedRowsChanged(int lastRowSelected)
{
    modelListeners.call(&ModelListener::selectionChanged);
}

int ChannelListModel::getNumRows()
{
    return filter->sourceParams[0].size();
}

void ChannelListModel::paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected)
{
    g.setColour(BG_COLOR.brighter((rowNumber % 2 == 0) ? .2 : .3f));
    g.fillRect(0, 0, width, height);

    Rectangle<float> r = Rectangle<float>(0, 0, width, height).reduced(4, 0);
    g.setColour(rowIsSelected ? GREEN_COLOR : TEXT_COLOR.darker(.3f));
    g.fillRoundedRectangle(r.removeFromRight(r.getHeight()).reduced(4), 2);
    g.setFont(12);
    g.drawText("Input #" + String(rowNumber + 1), r.reduced(2), Justification::centredLeft);
}