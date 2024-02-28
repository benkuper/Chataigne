/*
  ==============================================================================

	MultiplexListEditor.cpp
	Created: 22 Feb 2021 11:23:30am
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"
#include "MultiplexListEditor.h"


BaseMultiplexListEditor::BaseMultiplexListEditor(BaseMultiplexList* list, bool isRoot) :
	BaseItemEditor(list, isRoot),
	baseList(list),
	fillBT("Fill...")
{
	if (MultiplexList<Trigger>* mt = dynamic_cast<MultiplexList<Trigger>*>(list))
	{
		//no fill bt
	}
	else
	{
		fillBT.addListener(this);
		addAndMakeVisible(&fillBT);
	}
}

BaseMultiplexListEditor::~BaseMultiplexListEditor()
{
}



void BaseMultiplexListEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	fillBT.setBounds(r.removeFromRight(100).reduced(1));
}

void BaseMultiplexListEditor::buttonClicked(Button* b)
{
	if (b == &fillBT)
	{
		PopupMenu p;
		p.addItem(1, "From Expression");
		p.addItem(2, "From First Element");

		addItemsToFillMenu(p);

		p.showMenuAsync(PopupMenu::Options(), [this](int result)
			{
				switch (result)
				{
				case 1:
				{
					expressionWindow.reset(new ExpressionComponentWindow(baseList));
					DialogWindow::LaunchOptions dw;
					dw.content.set(expressionWindow.get(), false);
					dw.dialogTitle = "Fill from expression";
					dw.escapeKeyTriggersCloseButton = true;
					dw.dialogBackgroundColour = BG_COLOR;
					dw.launchAsync();
				}
				break;



				case 2:
				{
					for (auto& c : baseList->list) ((Parameter*)c)->setValue(((Parameter*)baseList->list[0])->value);
				}
				break;

				default:
					handleFillMenuResult(result);
					break;
				}
			}
		);
	}

	BaseItemEditor::buttonClicked(b);

}



EnumMultiplexListEditor::EnumMultiplexListEditor(EnumMultiplexList* eList, bool isRoot) :
	BaseMultiplexListEditor(eList, isRoot),
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
	else BaseMultiplexListEditor::buttonClicked(b);
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
	BaseMultiplexListEditor(eList, isRoot)
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
	BaseMultiplexListEditor(eList, isRoot),
	list(eList)
{
}

InputValueListEditor::~InputValueListEditor()
{

}

void InputValueListEditor::addItemsToFillMenu(PopupMenu& p)
{
	p.addItem(3, "From Input Values");
	p.addItem(4, "From Container");
}

void InputValueListEditor::handleFillMenuResult(int result)
{
	switch (result)
	{

	case 3:
	{
		ControllableContainer* cc = nullptr;
		PopupMenu cp;
		int offset = 0;
		Array<Module*> modules = ModuleManager::getInstance()->getModuleList();
		OwnedArray<ContainerChooserPopupMenu>* choosers = new OwnedArray<ContainerChooserPopupMenu>();
		for (auto& m : modules)
		{
			ContainerChooserPopupMenu* chooser = new ContainerChooserPopupMenu(&m->valuesCC, offset, -1, nullptr, StringArray(), StringArray(), true);
			
			choosers->add(chooser);
			
			cp.addSubMenu(m->niceName, *chooser);
			offset += 100000;
		}

		cp.showMenuAsync(PopupMenu::Options(), [this, choosers](int ccResult)
			{
				if (ccResult > 0)
				{
					int chooserIndex = (int)floor(ccResult / 100000.0f);
					ControllableContainer* cc = (*choosers)[chooserIndex]->getContainerForResult(ccResult);
					Array<WeakReference<Controllable>> cList = cc->getAllControllables();
					showRangeMenuAndFillFromControllables(cList);
				}

				delete choosers;
			}
		);

		if (cc != nullptr)
		{

		}
	}
	break;

	case 4:
	{
		ContainerChooserPopupMenu* chooser = new ContainerChooserPopupMenu(Engine::mainEngine, 0, -1, nullptr, StringArray(), StringArray(), true);
		chooser->showAndGetContainer([this, chooser](ControllableContainer* cc)
			{
				if (cc != nullptr)
				{
					Array<WeakReference<Controllable>> cList = cc->getAllControllables();
					showRangeMenuAndFillFromControllables(cList);
				}

				delete chooser;
			}
		);
	}
	break;
	}
}

void InputValueListEditor::showRangeMenuAndFillFromControllables(Array<WeakReference<Controllable>> cList)
{
	AlertWindow* nameWindow = new AlertWindow("Set the assignation range", "Set the start and end index to use to assign", AlertWindow::AlertIconType::NoIcon, this);


	nameWindow->addTextEditor("cOffset", "0", "Source Offset");
	nameWindow->addTextEditor("start", "1", "Start Index");
	nameWindow->addTextEditor("end", String(cList.size()), "End Index");

	nameWindow->addButton("OK", 1, KeyPress(KeyPress::returnKey));
	nameWindow->addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));

	nameWindow->enterModalState(true, ModalCallbackFunction::create([this, cList, nameWindow](int result)
		{
			if (result)
			{
				String cOffsetS = nameWindow->getTextEditorContents("cOffset");
				String startS = nameWindow->getTextEditorContents("start");
				String endS = nameWindow->getTextEditorContents("end");
				int cOffset = cOffsetS.isNotEmpty() ? cOffsetS.getIntValue() : 0;
				int start = startS.isNotEmpty() ? jlimit(1, baseList->listSize, startS.getIntValue()) - 1 : 0;
				int end = endS.isNotEmpty() ? jlimit(start, baseList->listSize, endS.getIntValue()) : baseList->listSize;


				for (int i = start; i < end && i < cList.size(); i++)
				{
					int cIndex = i - start + cOffset;
					if (cIndex >= cList.size()) break;

					((TargetParameter*)baseList->list[i])->setValueFromTarget(cList[cIndex]);
				}
			}
		}),
		true
	);
}


BaseMultiplexListEditor::ExpressionComponentWindow::ExpressionComponentWindow(BaseMultiplexList* list) :
	list(list),
	assignBT("Assign"),
	startIndex("Start", "Index at which to start the expression evaluation", 1, 1, list->listSize),
	endIndex("End", "Index at which to stop the expression evaluation (inclusive)", list->listSize, 1, list->listSize),
	startUI(&startIndex),
	endUI(&endIndex)
{
	instructions.setText("This expression will be used to fill each item in this list. You can use wildcards {index} and {index0} to replace with index of the item that is processed. You can also modify the start and end index for narrowing down the evaluation.", dontSendNotification);

	addAndMakeVisible(&instructions);
	addAndMakeVisible(&editor);
	addAndMakeVisible(&assignBT);
	addAndMakeVisible(&startUI);
	addAndMakeVisible(&endUI);

	assignBT.addListener(this);
	//addAndMakeVisible(&closeBT);

	editor.setColour(editor.backgroundColourId, Colours::black);
	editor.setColour(CaretComponent::caretColourId, Colours::orange);
	editor.setColour(editor.textColourId, TEXT_COLOR);
	editor.setMultiLine(true);

	setSize(600, 330);
}

void BaseMultiplexListEditor::ExpressionComponentWindow::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(2);
	instructions.setBounds(r.removeFromTop(60).reduced(8));

	Rectangle<int> ir = r.removeFromTop(30).reduced(8);
	startUI.setBounds(ir.removeFromLeft(150));
	ir.removeFromLeft(20);
	endUI.setBounds(ir.removeFromLeft(150));

	editor.setBounds(r.removeFromTop(100));
	assignBT.setBounds(r.removeFromTop(40).reduced(2));
}

void BaseMultiplexListEditor::ExpressionComponentWindow::buttonClicked(Button* b)
{
	if (b == &assignBT)
	{
		list->fillFromExpression(editor.getText(), startIndex.intValue(), endIndex.intValue());
	}
}

NumberListEditor::NumberListEditor(MultiplexList<FloatParameter>* list, bool isRoot) :
	BaseMultiplexListEditor(list, isRoot),
	floatList(list),
	intList(nullptr)
{
}


NumberListEditor::NumberListEditor(MultiplexList<IntParameter>* list, bool isRoot) :
	BaseMultiplexListEditor(list, isRoot),
	floatList(nullptr),
	intList(list)
{

}


NumberListEditor::~NumberListEditor()
{
}

void NumberListEditor::addPopupMenuItems(PopupMenu* p)
{
	PopupMenu rangeMenu;
	rangeMenu.addItem(-4, "Custom");
	rangeMenu.addSeparator();

	if (floatList != nullptr)
	{
		rangeMenu.addItem(-50, "0 : 1");
		rangeMenu.addItem(-51, "-1 : 1");
		rangeMenu.addItem(-52, "-90 : 90");
		rangeMenu.addItem(-53, "0 : 180");
		rangeMenu.addItem(-54, "-180 : 180");
		rangeMenu.addItem(-55, "0 : 360");

	}
	else if (intList != nullptr)
	{
		rangeMenu.addItem(-60, "0 : 100");
		rangeMenu.addItem(-61, "0 : 127");
		rangeMenu.addItem(-62, "0 : 255");
		rangeMenu.addItem(-63, "0 : 65535");
		rangeMenu.addItem(-52, "-90 : 90");
		rangeMenu.addItem(-53, "0 : 180");
		rangeMenu.addItem(-54, "-180 : 180");
		rangeMenu.addItem(-55, "0 : 360");
	}

	int numCustomRanges = ProjectSettings::getInstance()->customRangesCC.controllables.size();
	if (numCustomRanges > 0)
	{
		rangeMenu.addSeparator();
		for (int i = 0; i < numCustomRanges; i++)
		{
			Point2DParameter* rp = dynamic_cast<Point2DParameter*>(ProjectSettings::getInstance()->customRangesCC.controllables[i]);
			rangeMenu.addItem(-100 - i, String(rp->x) + " : " + String(rp->y));
		}
	}

	p->addSubMenu("Set Range...", rangeMenu);
	p->addItem(-5, "Clear Range");
}

void NumberListEditor::handleMenuSelectedID(int id)
{
	switch (id)
	{
	case -4: showEditRangeWindow(); break;
	case -5: clearAllRange(); break;
	case -50: setAllRange(0, 1); break;
	case -51: setAllRange(-1, 1); break;
	case -52: setAllRange(-90, 90); break;
	case -53: setAllRange(0, 180); break;
	case -54: setAllRange(-180, 180); break;
	case -55: setAllRange(0, 360); break;
	case -60: setAllRange(0, 100); break;
	case -61: setAllRange(0, 127); break;
	case -62: setAllRange(0, 255); break;
	case -63: setAllRange(0, 65535); break;

	default:
	{
		int numCustomRanges = ProjectSettings::getInstance()->customRangesCC.controllables.size();
		int cid = -(id + 100);
		if (cid < numCustomRanges)
		{
			Point2DParameter* rp = dynamic_cast<Point2DParameter*>(ProjectSettings::getInstance()->customRangesCC.controllables[cid]);
			setAllRange(rp->x, rp->y);
		}
		break;
	}
	}
}

void NumberListEditor::setAllRange(var min, var max)
{
	if (floatList != nullptr)
	{
		for (int i = 0; i < floatList->list.size(); i++)
		{
			if (FloatParameter* fp = dynamic_cast<FloatParameter*>(floatList->list[i]))
			{
				fp->setRange(min, max);
			}
		}
	}
	else if (intList != nullptr)
	{
		for (int i = 0; i < intList->list.size(); i++)
		{
			if (IntParameter* ip = dynamic_cast<IntParameter*>(intList->list[i]))
			{
				ip->setRange(min, max);
			}
		}
	}
}

void NumberListEditor::clearAllRange()
{
	if (floatList != nullptr)
	{
		for (int i = 0; i < floatList->list.size(); i++)
		{
			if (FloatParameter* fp = dynamic_cast<FloatParameter*>(floatList->list[i]))
			{
				fp->clearRange();
			}
		}
	}
	else if (intList != nullptr)
	{
		for (int i = 0; i < intList->list.size(); i++)
		{
			if (IntParameter* ip = dynamic_cast<IntParameter*>(intList->list[i]))
			{
				ip->clearRange();
			}
		}
	}
}

void NumberListEditor::showEditRangeWindow()
{
	AlertWindow* nameWindow = new AlertWindow("Set the range", "Set a new range for all parameters in the list", AlertWindow::AlertIconType::NoIcon, this);

	Parameter* parameter = floatList != nullptr ? (Parameter*)floatList->list[0] : (Parameter*)intList->list[0];
	if (parameter == nullptr) return;

	nameWindow->addTextEditor("minVal", parameter->hasRange() ? String((float)parameter->minimumValue) : "", "Minimum");
	nameWindow->addTextEditor("maxVal", parameter->hasRange() ? String((float)parameter->maximumValue) : "", "Maximum");

	nameWindow->addButton("OK", 1, KeyPress(KeyPress::returnKey));
	nameWindow->addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));

	nameWindow->enterModalState(true, ModalCallbackFunction::create([this, nameWindow](int result)
		{
			if (result)
			{
				String minRangeString = nameWindow->getTextEditorContents("minVal");
				String maxRangeString = nameWindow->getTextEditorContents("maxVal");
				float newMin = minRangeString.isNotEmpty() ? minRangeString.getFloatValue() : INT32_MIN;
				float newMax = maxRangeString.isNotEmpty() ? maxRangeString.getFloatValue() : INT32_MAX;
				setAllRange(newMin, jmax(newMin, newMax));
			}
		}),
		true
	);
}

TargetMultiplexListEditor::TargetMultiplexListEditor(TargetMultiplexList* list, bool isRoot) :
	BaseMultiplexListEditor(list, isRoot),
	targetList(list)
{
}

TargetMultiplexListEditor::~TargetMultiplexListEditor()
{
}

void TargetMultiplexListEditor::addPopupMenuItems(PopupMenu* p)
{
	p->addItem("Container Mode", true, targetList->containerMode, [&]() { targetList->setContainerMode(!targetList->containerMode); });
}
