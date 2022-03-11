/*
  ==============================================================================

	MultiplexListEditor.cpp
	Created: 22 Feb 2021 11:23:30am
	Author:  bkupe

  ==============================================================================
*/

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


EnumMultiplexListEditor::EnumListOptionManager::EnumListOptionManager(EnumMultiplexList* eList) :
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
	g.drawText("Key", hr.removeFromLeft(getWidth() / 2).reduced(2).toFloat(), Justification::centred, false);
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

EnumMultiplexListEditor::EnumListOptionManager::EnumOptionUI::EnumOptionUI(EnumMultiplexList* eList, int index) :
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

CVPresetMultiplexListEditor::CVPresetMultiplexListEditor(CVPresetMultiplexList* eList, bool isRoot) :
	BaseItemEditor(eList, isRoot)
{
	cvTargetUI.reset(eList->cvTarget->createTargetUI());
	addAndMakeVisible(cvTargetUI.get());
}

CVPresetMultiplexListEditor::~CVPresetMultiplexListEditor()
{
}

void CVPresetMultiplexListEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	cvTargetUI->setBounds(r.removeFromRight(120));
}


//Input values list

InputValueListEditor::InputValueListEditor(InputValueMultiplexList* eList, bool isRoot) :
	BaseItemEditor(eList, isRoot),
	fillBT("Fill..."),
	list(eList)
{
	fillBT.addListener(this);
	addAndMakeVisible(&fillBT);
}

InputValueListEditor::~InputValueListEditor()
{

}

void InputValueListEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	fillBT.setBounds(r.removeFromRight(100).reduced(1));
}

void InputValueListEditor::buttonClicked(Button* b)
{
	if (b == &fillBT)
	{
		PopupMenu p;
		p.addItem(1, "From Expression");
		p.addItem(2, "From Input Values");
		p.addItem(3, "From Container");

		p.showMenuAsync(PopupMenu::Options(), [this](int result)
		{
			switch (result)
			{
			case 1:
			{
				expressionWindow.reset(new ExpressionComponentWindow(list));
				DialogWindow::LaunchOptions dw;
				dw.content.set(expressionWindow.get(), false);
				dw.dialogTitle = "Fill from expression";
				dw.escapeKeyTriggersCloseButton = true;
				dw.dialogBackgroundColour = BG_COLOR;
				dw.launchAsync();
			}
			break;

			case 2:
			case 3:
			{
				ControllableContainer* cc = nullptr;
				if (result == 2)
				{
					PopupMenu cp;
					int offset = 0;
					Array<Module*> modules = ModuleManager::getInstance()->getModuleList();
					OwnedArray<ContainerChooserPopupMenu> choosers;
					for (auto& m : modules)
					{
						ContainerChooserPopupMenu* chooser = new ContainerChooserPopupMenu(&m->valuesCC, offset, -1, nullptr, StringArray(), StringArray(), true);
						choosers.add(chooser);
						cp.addSubMenu(m->niceName, *chooser);
						offset += 100000;
					}

					cp.showMenuAsync(PopupMenu::Options(), [this, &choosers](int ccResult)
						{
							int chooserIndex = (int)floor(ccResult / 100000.0f);
							ControllableContainer* cc = choosers[chooserIndex]->getContainerForResult(ccResult);
							Array<WeakReference<Controllable>> cList = cc->getAllControllables();
							for (int i = 0; i < list->listSize && i < cList.size(); i++)
							{
								((TargetParameter*)list->list[i])->setValueFromTarget(cList[i]);
							}
						}
					);

					if (cc != nullptr)
					{

					}
				}
				else
				{
					ContainerChooserPopupMenu chooser(Engine::mainEngine, 0, -1, nullptr, StringArray(), StringArray(), true);
					chooser.showAndGetContainer([this](ControllableContainer* cc)
						{
							Array<WeakReference<Controllable>> cList = cc->getAllControllables();
							for (int i = 0; i < list->listSize && i < cList.size(); i++)
							{
								((TargetParameter*)list->list[i])->setValueFromTarget(cList[i]);
							}
						}
					);
				}
			}
			break;
			}
		}
		);
	}

	BaseItemEditor::buttonClicked(b);

}

InputValueListEditor::ExpressionComponentWindow::ExpressionComponentWindow(InputValueMultiplexList* list) :
	list(list),
	assignBT("Assign")
{
	instructions.setText("This expression will be used to fill each item in this list. You can use wildcards {index} and {index0} to replace with index of the item that is processed.", dontSendNotification);

	addAndMakeVisible(&instructions);
	addAndMakeVisible(&editor);
	addAndMakeVisible(&assignBT);
	assignBT.addListener(this);
	//addAndMakeVisible(&closeBT);

	editor.setColour(editor.backgroundColourId, Colours::black);
	editor.setColour(CaretComponent::caretColourId, Colours::orange);
	editor.setColour(editor.textColourId, TEXT_COLOR);
	editor.setMultiLine(true);

	setSize(600, 300);
}

void InputValueListEditor::ExpressionComponentWindow::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(2);
	instructions.setBounds(r.removeFromTop(60).reduced(8));
	editor.setBounds(r.removeFromTop(100));
	assignBT.setBounds(r.removeFromTop(40).reduced(2));
}

void InputValueListEditor::ExpressionComponentWindow::buttonClicked(Button* b)
{
	if (b == &assignBT)
	{
		list->fillFromExpression(editor.getText());
	}
}
