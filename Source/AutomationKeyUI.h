/*
  ==============================================================================

    AutomationKeyUI.h
    Created: 11 Dec 2016 1:22:27pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUTOMATIONKEYUI_H_INCLUDED
#define AUTOMATIONKEYUI_H_INCLUDED

#include "AutomationKey.h"
#include "EasingUI.h"

class AutomationKeyUI :
	public BaseItemMinimalUI<AutomationKey>
{
public:
	AutomationKeyUI(AutomationKey *);
	virtual ~AutomationKeyUI();

	ScopedPointer<EasingUI> easingUI;

	const static int handleSize = 6;
	const static int handleClickZone = 10;


	float posAtMouseDown;
	float valueAtMouseDown;


	class Handle :
		public Component
	{
	public:
		Handle();
		bool highlight;
		Colour color;
		void paint(Graphics &g) override;
	};

	Handle handle;

	int keyYPos1;
	int keyYPos2;

	void paint(Graphics &) override {}; //avoid default item painting

	void setEasingUI(EasingUI * eui);

	void setKeyPositions(const int &k1, const int &k2);

	void showKeyEditorWindow();
	
	void resized() override;

	bool hitTest(int tx, int ty) override;

	void mouseDown(const MouseEvent &e) override;
	void mouseUp(const MouseEvent &e) override;

	void controllableFeedbackUpdateInternal(Controllable * c) override;

	void inspectableSelectionChanged(Inspectable *) override;
	void inspectablePreselectionChanged(Inspectable *) override;
};


#endif  // AUTOMATIONKEYUI_H_INCLUDED
