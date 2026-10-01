/*
  ==============================================================================

    ChataigneAudioLayerPanel.cpp
    Created: 20 Nov 2016 3:08:49pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "ChataigneAudioLayerPanel.h"

ChataigneAudioLayerPanel::ChataigneAudioLayerPanel(ChataigneAudioLayer * layer) :
	AudioLayerPanel(layer),
	chataigneAudioLayer(layer)
{
	
	moduleChooser.setTextWhenNoChoicesAvailable("No audio module");
	moduleChooser.setTextWhenNothingSelected("Choose an audio module");
	
	moduleChooser.filterModuleFunc = &ChataigneAudioLayerPanel::isAudioModule;
	moduleChooser.buildModuleBox();

	moduleChooser.addChooserListener(this);
	moduleChooser.setModuleSelected(chataigneAudioLayer->audioModule,true);

	contentComponents.add(&moduleChooser);
	addAndMakeVisible(&moduleChooser);

	outputRoutingLabel.setColour(Label::textColourId, TEXT_COLOR);
	outputRoutingLabel.setJustificationType(Justification::centredLeft);
	outputRoutingLabel.setMinimumHorizontalScale(1.0f);
	addAndMakeVisible(outputRoutingLabel);
	contentComponents.add(&outputRoutingLabel);

	outputRoutingButton.setWantsKeyboardFocus(false);
	outputRoutingButton.setMouseClickGrabsKeyboardFocus(false);
	outputRoutingButton.onClick = [this] { showOutputRoutingMenu(); };
	addAndMakeVisible(outputRoutingButton);
	contentComponents.add(&outputRoutingButton);

	chataigneAudioLayer->addAudioLayerListener(this);
	chataigneAudioLayer->channelsCC.addAsyncContainerListener(this);
	updateOutputRouting();
}

ChataigneAudioLayerPanel::~ChataigneAudioLayerPanel()
{
	if (!inspectable.wasObjectDeleted())
	{
		chataigneAudioLayer->removeAudioLayerListener(this);
		chataigneAudioLayer->channelsCC.removeAsyncContainerListener(this);
	}
}


void ChataigneAudioLayerPanel::resizedInternalContent(Rectangle<int>& r)
{
	SequenceLayerPanel::resizedInternalContent(r);  
	
	volumeUI->setBounds(r.removeFromTop(18).reduced(2));

	Rectangle<int> gr = r.removeFromTop(18).reduced(2, 1);
	moduleChooser.setBounds(gr.removeFromLeft(80));
	enveloppeUI->setBounds(gr);

	if (chataigneAudioLayer->showOutputRouting->boolValue())
	{
		Rectangle<int> routingBounds = r.removeFromTop(20).reduced(2, 1);
		outputRoutingButton.setBounds(routingBounds.removeFromRight(54));
		routingBounds.removeFromRight(2);
		outputRoutingLabel.setFont(GlobalSettings::getInstance()->fontSize->floatValue());
		outputRoutingLabel.setBounds(routingBounds);
	}
}

void ChataigneAudioLayerPanel::updateOutputRouting()
{
	if (inspectable.wasObjectDeleted()) return;

	StringArray channelNumbers, channelNames;
	for (int i = 0; i < chataigneAudioLayer->channelsCC.controllables.size(); ++i)
	{
		auto* channel = static_cast<BoolParameter*>(chataigneAudioLayer->channelsCC.controllables[i]);
		if (!channel->boolValue()) continue;
		channelNumbers.add(String(i + 1));
		channelNames.add(channel->niceName);
	}

	String summary;
	if (chataigneAudioLayer->audioModule == nullptr) summary = "Outputs: No audio module";
	else if (chataigneAudioLayer->channelsCC.controllables.isEmpty()) summary = "Outputs: None available";
	else if (channelNumbers.isEmpty()) summary = "Outputs: None selected";
	else summary = "Outputs: " + channelNumbers.joinIntoString(", ");

	outputRoutingLabel.setText(summary, dontSendNotification);
	String tooltip = channelNames.isEmpty() ? summary : channelNames.joinIntoString("\n");
	tooltip += "\n";
	tooltip += item->isUILocked->boolValue() ? "Layer is locked."
		: chataigneAudioLayer->lockOutputRouting->boolValue() ? "Output routing is locked. Use the routing menu to unlock it."
		: "Use Edit... to select output channels.";
	outputRoutingLabel.setTooltip(tooltip);
	outputRoutingButton.setTooltip(tooltip);
	outputRoutingButton.setButtonText(canEditOutputRouting() ? "Edit..." : "Locked");

	const bool show = chataigneAudioLayer->showOutputRouting->boolValue();
	const bool expanded = !item->miniMode->boolValue();
	outputRoutingLabel.setVisible(show && expanded);
	outputRoutingButton.setVisible(show && expanded);
	minContentHeight = show ? 56 : 36;
	const int minimumHeight = headerHeight + headerGap + margin * 2 + resizerHeight + minContentHeight;
	constrainer.setMinimumHeight(minimumHeight);
	if (expanded && getHeight() < minimumHeight) setSize(getWidth(), minimumHeight);
	else resized();
}

bool ChataigneAudioLayerPanel::canEditOutputRouting() const
{
	return !inspectable.wasObjectDeleted()
		&& chataigneAudioLayer->showOutputRouting->boolValue()
		&& !chataigneAudioLayer->lockOutputRouting->boolValue()
		&& !item->isUILocked->boolValue();
}

void ChataigneAudioLayerPanel::addOutputRoutingMenuItems(PopupMenu& menu)
{
	Component::SafePointer<ChataigneAudioLayerPanel> panel(this);
	menu.addItem("Show output routing", true, chataigneAudioLayer->showOutputRouting->boolValue(), [panel]
		{
			if (panel == nullptr || panel->inspectable.wasObjectDeleted()) return;
			auto* parameter = panel->chataigneAudioLayer->showOutputRouting;
			parameter->setUndoableValue(parameter->boolValue(), !parameter->boolValue());
			panel->updateOutputRouting();
		});
	menu.addItem("Lock output routing", !item->isUILocked->boolValue(),
		chataigneAudioLayer->lockOutputRouting->boolValue() || item->isUILocked->boolValue(), [panel]
		{
			if (panel == nullptr || panel->inspectable.wasObjectDeleted() || panel->item->isUILocked->boolValue()) return;
			auto* parameter = panel->chataigneAudioLayer->lockOutputRouting;
			parameter->setUndoableValue(parameter->boolValue(), !parameter->boolValue());
			panel->updateOutputRouting();
		});
}

void ChataigneAudioLayerPanel::showOutputRoutingMenu()
{
	if (inspectable.wasObjectDeleted()) return;

	PopupMenu menu;
	addOutputRoutingMenuItems(menu);
	menu.addSeparator();
	menu.addSectionHeader("Output channels");
	Component::SafePointer<ChataigneAudioLayerPanel> panel(this);
	for (auto* controllable : chataigneAudioLayer->channelsCC.controllables)
	{
		auto* channel = static_cast<BoolParameter*>(controllable);
		WeakReference<Controllable> channelRef(channel);
		menu.addItem(channel->niceName, canEditOutputRouting(), channel->boolValue(), [panel, channelRef]
			{
				// Device/module changes rebuild these parameters while the menu may still be open.
				if (panel == nullptr || !panel->canEditOutputRouting() || channelRef == nullptr) return;
				if (channelRef->parentContainer != &panel->chataigneAudioLayer->channelsCC) return;
				auto* parameter = static_cast<BoolParameter*>(channelRef.get());
				parameter->setUndoableValue(parameter->boolValue(), !parameter->boolValue());
			});
	}
	if (chataigneAudioLayer->channelsCC.controllables.isEmpty())
		menu.addSectionHeader(chataigneAudioLayer->audioModule == nullptr ? "No audio module" : "No output channels available");
	menu.showMenuAsync(PopupMenu::Options().withTargetComponent(&outputRoutingButton));
}

void ChataigneAudioLayerPanel::controllableFeedbackUpdateInternal(Controllable* c)
{
	AudioLayerPanel::controllableFeedbackUpdateInternal(c);
	if (c == chataigneAudioLayer->showOutputRouting || c == chataigneAudioLayer->lockOutputRouting
		|| c == item->isUILocked || c == item->miniMode)
		updateOutputRouting();
}

void ChataigneAudioLayerPanel::newMessage(const ContainerAsyncEvent& e)
{
	if (inspectable.wasObjectDeleted()) return;
	if (e.source == &chataigneAudioLayer->channelsCC) updateOutputRouting();
	else BaseItemMinimalUI<SequenceLayer>::newMessage(e);
}

void ChataigneAudioLayerPanel::targetAudioModuleChanged(ChataigneAudioLayer *)
{
	moduleChooser.setModuleSelected(chataigneAudioLayer->audioModule,true);
	if (chataigneAudioLayer->audioModule == nullptr) moduleChooser.setSelectedId(0, dontSendNotification);
	updateOutputRouting();
}

void ChataigneAudioLayerPanel::selectedModuleChanged(ModuleChooserUI *, Module * m)
{
	chataigneAudioLayer->setAudioModule(dynamic_cast<AudioModule *>(m));
}

void ChataigneAudioLayerPanel::outputChannelsChanged(ChataigneAudioLayer*)
{
	updateOutputRouting();
}

void ChataigneAudioLayerPanel::moduleListChanged(ModuleChooserUI *)
{
	moduleChooser.setModuleSelected(chataigneAudioLayer->audioModule, true);

}

void ChataigneAudioLayerPanel::mouseDown(const MouseEvent& e)
{
	AudioLayerPanel::mouseDown(e);

	if (e.mods.isRightButtonDown())
	{
		if (e.eventComponent == this || e.eventComponent == &outputRoutingLabel)
		{
			PopupMenu p;
			addOutputRoutingMenuItems(p);
			p.addSeparator();
			p.addItem(1, "Export Enveloppe to new mapping layer");
			p.addItem(2, "Export Enveloppe to clipboard");
			p.addItem(3, "Export Enveloppe to clipboard (data only)");

			Component::SafePointer<ChataigneAudioLayerPanel> panel(this);
			p.showMenuAsync(PopupMenu::Options(), [panel](int result)
				{
					if (panel == nullptr || panel->inspectable.wasObjectDeleted()) return;
					switch (result)
					{
					case 1:
						panel->chataigneAudioLayer->exportRMS(true, false, false);
						break;

					case 2:
						panel->chataigneAudioLayer->exportRMS(false, true, false);
						break;

					case 3:
						panel->chataigneAudioLayer->exportRMS(false, true, true);
						break;
					}
				}); 
		}
	}
	
}
