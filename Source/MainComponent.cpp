/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#include "MainIncludes.h"
#include "Module/ModuleIncludes.h"

using namespace std::placeholders;

String getAppVersion();

//==============================================================================
MainContentComponent::MainContentComponent()
{
	getCommandManager().registerAllCommandsForTarget(this);
}


MainContentComponent::~MainContentComponent()
{
	SharedTextureManager::deleteInstance();
	ActionUITimers::deleteInstance();
}

void MainContentComponent::init()
{
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Module Router", &ModuleRouterPanelView::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Modules", &ModuleManagerUI::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Custom Variables", &CVGroupManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Morpher", &MorpherPanel::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Sequences", &createSequenceManagerUI));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Sequence Editor", &TimeMachineView::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("States", &StateManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("State Machine", &StateMachineView::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Command Templates", &CommandTemplateManagerPanel::create));

	OrganicMainContentComponent::init();

	String lastVersion = getAppProperties().getUserSettings()->getValue("lastVersion", "");

#if !JUCE_LINUX
	if (lastVersion != getAppVersion())
	{
		welcomeScreen.reset(new WelcomeScreen());
		DialogWindow::showDialog("Welcome", welcomeScreen.get(), getTopLevelComponent(), Colours::black, true);
	}
#endif

	ParameterUI::customAddToContextMenuFunc = MainContentComponent::parameterAddToContextMenu;
	ParameterUI::handleCustomContextMenuResultFunc = MainContentComponent::parameterHandleContextMenuResult;
}

SequenceManagerUI* MainContentComponent::createSequenceManagerUI(const String& name)
{
	return SequenceManagerUI::create(name, ChataigneSequenceManager::getInstance());
}


void MainContentComponent::parameterAddToContextMenu(ControllableUI* ui, PopupMenu* m)
{
	if (ui->controllable.wasObjectDeleted() || ui->controllable->type == Controllable::TRIGGER || ui->controllable->isControllableFeedbackOnly) return;
	
	{
		PopupMenu cvMenu;
		for (auto& g : CVGroupManager::getInstance()->items)
		{
			cvMenu.addItem(g->niceName, [g, ui]() { g->addItemFromParameter((Parameter*)ui->controllable.get()); });
		}
		m->addSubMenu("Add & Link to Custom Variable...", cvMenu);
	}
	
	{
		Module * genericModule = static_cast<ChataigneEngine*>(Engine::mainEngine)->module.get();
		CommandDefinition* commandDef = genericModule->defManager->getCommandDefinitionFor("", "Set Parameter Value");
		static const auto addOutput = [] (MappingLayer* layer, CommandDefinition* commandDef, Controllable* target)
		{
			MappingOutput* output = layer->mapping->om.createItem();
			output->setCommand(commandDef);
			GenericControllableCommand* command = dynamic_cast<GenericControllableCommand*>(output->command.get());
			if (command)
			{
				command->target->setValueFromTarget(target);
			}
			layer->mapping->om.addItem(output);
		};

		PopupMenu seqMenu;
		for (auto& sequence : ChataigneSequenceManager::getInstance()->items)
		{
			PopupMenu layerMenu;
			for (auto& layer : sequence->layerManager->items)
			{
				MappingLayer* mappingLayer = dynamic_cast<MappingLayer*>(layer);
				if (mappingLayer)
				{
					layerMenu.addItem(layer->niceName, [commandDef, mappingLayer, ui]
						{
							addOutput(mappingLayer, commandDef, ui->controllable);
						});
				}
			}

			if (layerMenu.getNumItems() > 0) layerMenu.addSeparator();
			layerMenu.addItem("Create new Mapping", [commandDef, sequence, ui]
				{
					Controllable* controllable = ui->controllable;
					MappingLayer* mappingLayer = nullptr;
					if (controllable->type == Controllable::COLOR)
					{
						mappingLayer = ColorMappingLayer::create(sequence, {});
					}
					else
					{
						Mapping1DLayer * mapping1DLayer = Mapping1DLayer::create(sequence, {});
						if (Parameter* parameter = dynamic_cast<Parameter*>(controllable))
						{
							if (parameter->hasRange() && 
								(dynamic_cast<FloatParameter*>(controllable) || dynamic_cast<IntParameter*>(controllable))
								)
							{
								const Point<float> range{ parameter->minimumValue, parameter->maximumValue };
								mapping1DLayer->automation->valueRange->setPoint(range);
								mapping1DLayer->automation->viewValueRange->setPoint(range);
							}
						}
						mappingLayer = mapping1DLayer;
					}
					mappingLayer->setNiceName(controllable->niceName);
					sequence->layerManager->addItem(mappingLayer);
					addOutput(mappingLayer, commandDef, controllable);
				});

			seqMenu.addSubMenu(sequence->niceName, layerMenu);
		}
		m->addSubMenu("Add & Link to Sequence...", seqMenu);
	}
}

bool MainContentComponent::parameterHandleContextMenuResult(ControllableUI* ui, int result)
{
	return false;
}


//Menubar Component

ChataigneMenuBarComponent::ChataigneMenuBarComponent(MainContentComponent* mainComp, ChataigneEngine* engine) :
	Component("Chataigne Menu Bar"),
	Thread("Sponsors"),
	sponsorsLoaded(false),
	curSponsor(nullptr)
#if !JUCE_MAC
	, menuBarComp(mainComp)
#endif
{
#if !JUCE_MAC
	addAndMakeVisible(menuBarComp);
#endif

	startThread();
}

ChataigneMenuBarComponent::~ChataigneMenuBarComponent()
{
	stopThread(200);
}

void ChataigneMenuBarComponent::paint(Graphics& g)
{
	g.fillAll(BG_COLOR);

	if (curSponsor != nullptr)
	{
		g.setColour(TEXTNAME_COLOR);
		String t = "Thank you " + curSponsor->name + " for the " + (curSponsor->isSponsor ? "sponsorship" : "donation") + " !";
		g.drawFittedText(t, getLocalBounds().removeFromRight(290).reduced(2), Justification::centredRight, 1);
	}
}

void ChataigneMenuBarComponent::resized()
{
	Rectangle<int> r = getLocalBounds();
	r.removeFromRight(300);
#if !JUCE_MAC	
	menuBarComp.setBounds(r);
#endif
}

void ChataigneMenuBarComponent::timerCallback()
{
	if (sponsorsLoaded && sponsors.size() > 0)
	{
		Random r;
		curSponsor = sponsors[r.nextInt(sponsors.size())];
		repaint();
	}
}

void ChataigneMenuBarComponent::run()
{

	URL url = URL("http://benjamin.kuperberg.fr/chataigne/releases/donation.json");

	String result = url.readEntireTextStream(false);

	if (result.isEmpty()) return;

	var data = JSON::parse(result);

	if (data.isObject())
	{
		var sponsorData = data.getProperty("sponsors", var());
		for (int i = 0; i < sponsorData.size(); i++)
		{
			sponsors.add(new DonationData({ sponsorData[i].toString(), true }));
		}

		var donationData = data.getProperty("donation", var());
		for (int i = 0; i < donationData.size(); i++)
		{
			sponsors.add(new DonationData({ donationData[i].toString(), false }));
		}

		sponsorsLoaded = true;

		startTimer(120000); // every 2 minutes

		MessageManager::callAsync([this]() {this->timerCallback(); });
	}

}
