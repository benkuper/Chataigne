/*
  ==============================================================================

    PresetChooser.cpp
    Created: 20 Oct 2016 6:28:03pm
    Author:  bkupe

  ==============================================================================
*/

#include "PresetChooser.h"
#include "DebugHelpers.h"

PresetChooser::PresetChooser(ControllableContainer * _container) :
ComboBox("Preset"),
container(_container)
{
	updatePresetComboBox();
	addListener(this);
	container->addControllableContainerListener(this);
	setTextWhenNothingSelected("Preset");
	setTooltip("Set the current preset at :\n" + container->currentPresetName->getControlAddress() + " <presetName>");
}

PresetChooser::~PresetChooser()
{
	removeListener(this);
	container->removeControllableContainerListener(this);
}


void PresetChooser::updatePresetComboBox(bool forceUpdate)
{

	bool emptyFilter = container->getPresetFilter().isEmpty();
	setEnabled(!emptyFilter);

	if (!emptyFilter)
	{
		PresetManager::getInstance()->fillWithPresets(this, container->getPresetFilter(), container->currentPreset != nullptr);
		if (container->currentPreset != nullptr) this->setSelectedId(container->currentPreset->presetId, forceUpdate ? sendNotification : dontSendNotification);
	}
}

void PresetChooser::comboBoxChanged(ComboBox * cb)
{

	int presetID = cb->getSelectedId();
	if (container->currentPreset != nullptr && presetID == container->currentPreset->presetId) return;

	if (presetID == PresetChoice::SaveCurrent)
	{
		bool result = container->saveCurrentPreset();
		if (result) cb->setSelectedId(container->currentPreset->presetId, NotificationType::dontSendNotification);
		else cb->setSelectedItemIndex(-1, NotificationType::dontSendNotification);

	} else if (presetID == PresetChoice::SaveToNew)
	{
		AlertWindow nameWindow("Save a new Preset", "Choose a name for the new preset", AlertWindow::AlertIconType::QuestionIcon, this);
		nameWindow.addTextEditor("newPresetName", "New Preset");
		nameWindow.addButton("OK", 1, KeyPress(KeyPress::returnKey));
		nameWindow.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));

		int nameResult = nameWindow.runModalLoop();

		if (nameResult)
		{
			String presetName = nameWindow.getTextEditorContents("newPresetName");
			PresetManager::Preset * p = container->saveNewPreset(presetName);
			cb->clear(NotificationType::dontSendNotification);
			updatePresetComboBox();
			cb->setSelectedId(p->presetId, dontSendNotification);
		} else
		{
			cb->setSelectedItemIndex(-1, dontSendNotification);
		}


	} else if (presetID == PresetChoice::ResetToDefault) //Reset to default
	{
		container->resetFromPreset();
		updatePresetComboBox(true);
		cb->setSelectedItemIndex(-1, NotificationType::dontSendNotification);
	} else if (presetID >= 0 && presetID < PresetChoice::deleteStartId)
	{
		String nameOfPreset = cb->getItemText(cb->getSelectedItemIndex());
		container->currentPresetName->setValue(nameOfPreset);
		updatePresetComboBox(false);

	} else if (presetID >= PresetChoice::deleteStartId)
	{
		bool ok = AlertWindow::showOkCancelBox(AlertWindow::AlertIconType::QuestionIcon, "Oh man, d'ya know watcha doin' ?", "Do you REALLY want to delete this preset ?\nLike, really really ?\nJust think about it man.", "Oh yeah", "F* No");
		if (ok)
		{
			PresetManager * pm = PresetManager::getInstance();
			int originId = cb->getSelectedId() - PresetChoice::deleteStartId - 1;
			String originText = cb->getItemText(cb->getNumItems() - container->getNumPresets() * 2 + originId);
			PresetManager::Preset * pre = pm->getPreset(container->getPresetFilter(), originText);
			pm->presets.removeObject(pre);

			container->currentPreset = nullptr;
			updatePresetComboBox(true);
		} else
		{
			//reselect last Id
			if (container->currentPreset != nullptr)
			{
				cb->setSelectedId(container->currentPreset->presetId, juce::dontSendNotification);
			} else
			{
				cb->setSelectedId(0, juce::dontSendNotification);
			}

		}

	} else {
		jassertfalse;
	}

}

void PresetChooser::controllableContainerPresetLoaded(ControllableContainer *)
{
	updatePresetComboBox(false);
}
