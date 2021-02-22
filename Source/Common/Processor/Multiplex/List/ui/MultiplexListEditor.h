/*
  ==============================================================================

    MultiplexListEditor.h
    Created: 22 Feb 2021 11:23:30am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../MultiplexList.h"

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
		EnumListOptionManager(EnumMultiplexList * eList);
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