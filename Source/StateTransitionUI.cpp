/*
  ==============================================================================

    StateTransitionUI.cpp
    Created: 28 Oct 2016 8:20:06pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateTransitionUI.h"
#include "StateManagerUI.h"

StateTransitionUI::StateTransitionUI(StateTransition * st, StateViewUI * _sourceSUI, StateViewUI * _destSUI) :
	BaseItemMinimalUI(st),
	sourceSUI(_sourceSUI),
	destSUI(_destSUI)
{
	jassert(sourceSUI != nullptr && destSUI != nullptr);

	sourceSUI->addStateViewUIListener(this);
	destSUI->addStateViewUIListener(this);

}

StateTransitionUI::~StateTransitionUI()
{
	sourceSUI->removeStateViewUIListener(this);
	destSUI->removeStateViewUIListener(this);
}

void StateTransitionUI::updateBounds()
{
	Rectangle<int> r = getParentComponent()->getLocalArea(sourceSUI, sourceSUI->getLocalBounds());
	Rectangle<int> r2 = getParentComponent()->getLocalArea(destSUI, destSUI->getLocalBounds());
	Rectangle<int> tr = Rectangle<int>::leftTopRightBottom(jmin<int>(r.getX(), r2.getX()), jmin<int>(r.getY(), r2.getY()), jmax<int>(r.getRight(), r2.getRight()), jmax<int>(r.getBottom(), r2.getBottom()));
	setBounds(tr);
}

void StateTransitionUI::paint(Graphics & g)
{
	if (sourceSUI == nullptr || destSUI == nullptr) return;

	//g.fillAll(Colours::purple.withAlpha(.3f));

	Rectangle<int> sourceRect = getLocalArea(sourceSUI, sourceSUI->getLocalBounds()).expanded(2,2);
	Rectangle<int> destRect = getLocalArea(destSUI, destSUI->getLocalBounds()).expanded(2,2);
	Point<int> midPoint = (sourceRect.getCentre() + destRect.getCentre()) / 2;
	Point<int> sourceP = sourceRect.getConstrainedPoint(midPoint);
	Point<int> destP = destRect.getConstrainedPoint(midPoint);

	g.setColour(HIGHLIGHT_COLOR);
	g.drawLine(sourceP.x, sourceP.y, destP.x, destP.y, 2);
}

void StateTransitionUI::paintOverChildren(Graphics & g)
{
}

void StateTransitionUI::resized()
{
}

void StateTransitionUI::editorGrabbed(StateViewUI *)
{
	updateBounds();
	
}
