/*
  ==============================================================================

    MultiplexListEditor.cpp
    Created: 22 Feb 2021 11:23:30am
    Author:  bkupe

  ==============================================================================
*/

#include "MultiplexListEditor.h"

EnumMultiplexListEditor::EnumMultiplexListEditor(EnumMultiplexList* eList, bool isRoot) :
	BaseItemEditor(eList, isRoot),
	eList(eList),
	optionsBT("Set Options", "Set the available options for this list")
{
	addAndMakeVisible(&optionsBT);
	optionsBT.addListener(this);
}

EnumMultiplexListEditor::~EnumMultiplexListEditor()
{
}

void EnumMultiplexListEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	optionsBT.setBounds(r.removeFromRight(80).reduced(2));
}

void EnumMultiplexListEditor::showOptionsWindow()
{
	std::unique_ptr<Component> editComponent(new EnumListOptionManager(eList));
	CallOutBox* box = &CallOutBox::launchAsynchronously(std::move(editComponent), localAreaToGlobal(optionsBT.getBounds()), nullptr);
	box->setArrowSize(8);
}

void EnumMultiplexListEditor::buttonClicked(Button* b)
{
	if (b == &optionsBT) showOptionsWindow();
	else BaseItemEditor::buttonClicked(b);
}


EnumMultiplexListEditor::EnumListOptionManager::EnumListOptionManager(EnumMultiplexList * eList) :
	eList(eList)
{
	int numRowsToDisplay = eList->referenceOptions.size() + 5;
	for (int i = 0; i < numRowsToDisplay; i++)
	{
		EnumOptionUI* ui = new EnumOptionUI(eList, i);
		optionsUI.add(ui);
		ui->keyLabel.addListener(this);
		ui->valueLabel.addListener(this);
		container.addAndMakeVisible(ui);
	}

	viewport.setScrollBarsShown(true, false);
	addAndMakeVisible(&viewport);
	viewport.setViewedComponent(&container, false);

	setSize(200, 140);
}

EnumMultiplexListEditor::EnumListOptionManager::~EnumListOptionManager()
{
}

void EnumMultiplexListEditor::EnumListOptionManager::paint(Graphics& g)
{
	Rectangle<int> hr = getLocalBounds().removeFromTop(20);
	g.setColour(TEXT_COLOR);
	g.drawText("Key", hr.removeFromRight(getWidth() / 2).reduced(2).toFloat(), Justification::centred, false);
	g.drawText("Value", hr.reduced(2).toFloat(), Justification::centred, false);
}

void EnumMultiplexListEditor::EnumListOptionManager::resized()
{
	Rectangle<int> r = getLocalBounds().withHeight(20);
	for (int i = 0; i < optionsUI.size(); i++) optionsUI[i]->setBounds(r.translated(0, i * r.getHeight()));

	int th = optionsUI.size() * r.getHeight();
	container.setSize(getWidth() - 10, th);
	viewport.setBounds(getLocalBounds().withTrimmedTop(20));
}

void EnumMultiplexListEditor::EnumListOptionManager::labelTextChanged(Label* l)
{
	EnumOptionUI* ui = (EnumOptionUI*)l->getParentComponent();
	eList->updateOption(optionsUI.indexOf(ui), ui->keyLabel.getText(), ui->valueLabel.getText());
}

EnumMultiplexListEditor::EnumListOptionManager::EnumOptionUI::EnumOptionUI(EnumMultiplexList * eList, int index) :
	eList(eList),
	index(index)
{
	if (index < eList->referenceOptions.size())
	{
		keyLabel.setText(eList->referenceOptions[index]->key, dontSendNotification);
		valueLabel.setText(eList->referenceOptions[index]->value.toString(), dontSendNotification);
	}


	keyLabel.setEditable(true);
	keyLabel.setBorderSize(BorderSize<int>(1));
	keyLabel.setColour(Label::backgroundColourId, BG_COLOR);
	keyLabel.setColour(Label::textColourId, TEXT_COLOR.darker(.2f));
	keyLabel.setColour(Label::textWhenEditingColourId, TEXT_COLOR);

	valueLabel.setEditable(true);
	valueLabel.setBorderSize(BorderSize<int>(1));
	valueLabel.setColour(Label::backgroundColourId, BG_COLOR);
	valueLabel.setColour(Label::textColourId, TEXT_COLOR.darker(.2f));
	valueLabel.setColour(Label::textWhenEditingColourId, TEXT_COLOR);


	addAndMakeVisible(&keyLabel);
	addAndMakeVisible(&valueLabel);
}

void EnumMultiplexListEditor::EnumListOptionManager::EnumOptionUI::resized()
{
	keyLabel.setBounds(getLocalBounds().removeFromLeft(getWidth() / 2).reduced(2));
	valueLabel.setBounds(getLocalBounds().removeFromRight(getWidth() / 2).reduced(2));
}
