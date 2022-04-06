/*
  ==============================================================================

	CommandChooserUI.cpp
	Created: 4 Nov 2016 11:23:37am
	Author:  bkupe

  ==============================================================================
*/

CommandChooserUI::CommandChooserUI(CommandContext _context, bool multiplexMode) :
	context(_context),
	multiplexMode(multiplexMode),
	lockedModule(nullptr),
	commandIsSet(false),
	commandIsGhost(false),
	noTargetText("[Click to select a command]")
{
	setInterceptsMouseClicks(true, true);

	targetBT.reset(AssetManager::getInstance()->getTargetBT());
	targetBT->setInterceptsMouseClicks(false, false);

	addAndMakeVisible(targetBT.get());

	targetBT->addListener(this);
	setRepaintsOnMouseActivity(true);

	label.setFont(label.getFont().withHeight(12));
	label.setInterceptsMouseClicks(false, false);
	label.setText(noTargetText, dontSendNotification);
	addAndMakeVisible(label);
}

CommandChooserUI::~CommandChooserUI()
{
}

void CommandChooserUI::paint(Graphics& g)
{

	Colour c = commandIsSet ? (commandIsGhost ? RED_COLOR : GREEN_COLOR) : NORMAL_COLOR;
	if (isMouseOver()) c = c.brighter();

	g.setGradientFill(ColourGradient(c.brighter(.2f), (float)getLocalBounds().getCentreX(), (float)getLocalBounds().getCentreY(), c.darker(), 2.f, 2.f, true));
	g.fillRoundedRectangle(targetBT->getBounds().expanded(2).toFloat(), 6);
}

void CommandChooserUI::resized()
{
	Rectangle<int> r = getLocalBounds();
	targetBT->setBounds(r.removeFromLeft(r.getHeight()).reduced(5));
	r.removeFromLeft(2);
	label.setBounds(r.reduced(0, 2));
}


void CommandChooserUI::setLabel(const String& text, bool ghostMode)
{
	label.setText(text.isNotEmpty() ? text : noTargetText, dontSendNotification);
	commandIsSet = text.isNotEmpty();
	commandIsGhost = ghostMode;
	repaint();
}

void CommandChooserUI::showPopupAndGetDefinition()
{
	CommandFactory::showMenuAndGetCommand(context,[this](CommandDefinition* def)
		{
			if (def != nullptr) chooserListeners.call(&Listener::definitionChosen, def);
		},
		lockedModule
	);
}

void CommandChooserUI::mouseDown(const MouseEvent&)
{
	showPopupAndGetDefinition();
}

void CommandChooserUI::buttonClicked(Button* b)
{
	if (b == targetBT.get()) {} // move code here ?
}
