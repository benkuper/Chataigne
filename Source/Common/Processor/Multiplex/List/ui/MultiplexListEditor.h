/*
  ==============================================================================

	MultiplexListEditor.h
	Created: 22 Feb 2021 11:23:30am
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class BaseMultiplexListEditor :
	public BaseItemEditor
{
public:
	BaseMultiplexListEditor(BaseMultiplexList* list, bool isRoot);
	~BaseMultiplexListEditor();

	BaseMultiplexList* baseList;
	TextButton fillBT;

	void resizedInternalHeaderItemInternal(Rectangle<int>& r) override;

	void buttonClicked(Button* b) override;

	virtual void addItemsToFillMenu(PopupMenu& p) {}
	virtual void handleFillMenuResult(int result) {}

	class ExpressionComponentWindow :
		public Component,
		public Button::Listener
	{
	public:
		ExpressionComponentWindow(BaseMultiplexList* list);
		~ExpressionComponentWindow() {}

		BaseMultiplexList* list;

		Label instructions;
		TextEditor editor;
		TextButton assignBT;
		TextButton closeBT;

		IntParameter startIndex;
		IntParameter endIndex;
		IntStepperUI startUI;
		IntStepperUI endUI;

		void resized() override;

		void buttonClicked(Button* b) override;
	};

	std::unique_ptr<ExpressionComponentWindow> expressionWindow;
};

class NumberListEditor :
	public BaseMultiplexListEditor
{
public:
	NumberListEditor(MultiplexList<FloatParameter>* list, bool isRoot);
	NumberListEditor(MultiplexList<IntParameter>* list, bool isRoot);
	~NumberListEditor();

	MultiplexList<FloatParameter>* floatList;
	MultiplexList<IntParameter>* intList;

	virtual void addPopupMenuItems(PopupMenu*) override;
	virtual void handleMenuSelectedID(int) override;

	void setAllRange(var min, var max);
	void clearAllRange();

	void showEditRangeWindow();
};

class EnumMultiplexListEditor :
	public BaseMultiplexListEditor
{
public:
	EnumMultiplexListEditor(EnumMultiplexList* eList, bool isRoot);
	~EnumMultiplexListEditor();

	EnumMultiplexList* eList;
	TextButton optionsBT;

	void resizedInternalHeaderItemInternal(Rectangle<int>& r) override;

	void showOptionsWindow();
	void buttonClicked(Button* b) override;

	class EnumListOptionManager :
		public Component,
		public Label::Listener
	{
	public:
		EnumListOptionManager(EnumMultiplexList* eList);
		~EnumListOptionManager();

		EnumMultiplexList* eList;
		Viewport viewport;
		Component container;

		class EnumOptionUI :
			public Component
		{
		public:
			EnumOptionUI(EnumMultiplexList* eList, int index);

			EnumMultiplexList* eList;
			int index;

			Label keyLabel;
			Label valueLabel;

			void resized() override;
		};

		OwnedArray<EnumOptionUI> optionsUI;

		void paint(Graphics& g) override;
		void resized() override;
		void labelTextChanged(Label* l) override;
	};
};


class TargetMultiplexListEditor :
	public BaseMultiplexListEditor
{
public:
	TargetMultiplexListEditor(TargetMultiplexList* list, bool isRoot);
	~TargetMultiplexListEditor();

	TargetMultiplexList* targetList;

	virtual void addPopupMenuItems(PopupMenu*) override;
};

class CVPresetMultiplexListEditor :
	public BaseMultiplexListEditor
{
public:
	CVPresetMultiplexListEditor(CVPresetMultiplexList* eList, bool isRoot);
	~CVPresetMultiplexListEditor();

	std::unique_ptr<TargetParameterUI> cvTargetUI;

	void resizedInternalHeaderItemInternal(Rectangle<int>& r) override;
};

class InputValueListEditor :
	public BaseMultiplexListEditor
{
public:
	InputValueListEditor(InputValueMultiplexList* eList, bool isRoot);
	~InputValueListEditor();

	InputValueMultiplexList* list;

	void addItemsToFillMenu(PopupMenu& p) override;
	void handleFillMenuResult(int result) override;

	void showRangeMenuAndFillFromControllables(Array<WeakReference<Controllable>> controllables);
};


