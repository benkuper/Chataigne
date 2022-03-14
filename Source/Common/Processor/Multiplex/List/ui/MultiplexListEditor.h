/*
  ==============================================================================

	MultiplexListEditor.h
	Created: 22 Feb 2021 11:23:30am
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class NumberListEditor :
	public BaseItemEditor
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
	public BaseItemEditor
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

class CVPresetMultiplexListEditor :
	public BaseItemEditor
{
public:
	CVPresetMultiplexListEditor(CVPresetMultiplexList* eList, bool isRoot);
	~CVPresetMultiplexListEditor();

	std::unique_ptr<TargetParameterUI> cvTargetUI;

	void resizedInternalHeaderItemInternal(Rectangle<int>& r) override;
};

class InputValueListEditor :
	public BaseItemEditor
{
public:
	InputValueListEditor(InputValueMultiplexList* eList, bool isRoot);
	~InputValueListEditor();

	TextButton fillBT;
	InputValueMultiplexList* list;

	void resizedInternalHeaderItemInternal(Rectangle<int>& r) override;

	void buttonClicked(Button* b) override;

	class ExpressionComponentWindow :
		public Component,
		public Button::Listener
	{
	public:
		ExpressionComponentWindow(InputValueMultiplexList* list);
		~ExpressionComponentWindow() {}

		InputValueMultiplexList* list;

		Label instructions;
		TextEditor editor;
		TextButton assignBT;
		TextButton closeBT;

		void resized() override;

		void buttonClicked(Button* b) override;
	};

	std::unique_ptr<ExpressionComponentWindow> expressionWindow;
};

