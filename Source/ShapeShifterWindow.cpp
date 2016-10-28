/*
  ==============================================================================

    ShapeShifterWindow.cpp
    Created: 2 May 2016 4:10:48pm
    Author:  bkupe

  ==============================================================================
*/

#include "ShapeShifterWindow.h"
#include "ShapeShifterManager.h"
#include "Style.h"

ShapeShifterWindow::ShapeShifterWindow(ShapeShifterPanel * _panel, Rectangle<int> bounds) :
	panel(_panel),
	ResizableWindow(_panel->currentContent->contentName, true),
	dragMode(PANEL)
{
	setTopLeftPosition(bounds.getTopLeft());
	_panel->setBounds(bounds);

	panel->setPreferredWidth(getWidth());
	panel->setPreferredHeight(getHeight());

	// panel->addShapeShifterPanelListener(this); //is it necessary ?

	setContentNonOwned(_panel,true);

	setBackgroundColour(BG_COLOR.darker(.1f).withAlpha(.3f));

	setResizable(true, true);
	setDraggable(true);

	setVisible(true);
	toFront(true);


	addMouseListener(this,true);

}

ShapeShifterWindow::~ShapeShifterWindow()
{
	removeMouseListener(this);

	//panel->removeShapeShifterPanelListener(this);

}

void ShapeShifterWindow::paintOverChildren(Graphics & g)
{
	g.setColour(BG_COLOR.brighter());
	g.drawRect(getLocalBounds());
}

void ShapeShifterWindow::resized()
{
	ResizableWindow::resized();

	if (panel == nullptr) return;

	panel->setPreferredWidth(getWidth());
	panel->setPreferredHeight(getHeight());
}

void ShapeShifterWindow::mouseDown(const MouseEvent & e)
{
	if (e.eventComponent == &panel->header || dynamic_cast<ShapeShifterPanelTab *>(e.eventComponent) != nullptr)
	{
		dragMode = e.eventComponent == &panel->header ? PANEL : TAB;
		dragger.startDraggingComponent(this, e);
	}else
	{
		dragMode = NONE;
	}

}

void ShapeShifterWindow::mouseDrag(const MouseEvent & e)
{
	if (dragMode == NONE) return;
	panel->setTransparentBackground(true);
	ShapeShifterManager::getInstance()->checkCandidateTargetForPanel(panel);
	dragger.dragComponent(this, e, 0);
}

void ShapeShifterWindow::mouseUp(const MouseEvent &)
{
	panel->setTransparentBackground(false);
	ShapeShifterManager::getInstance()->checkDropOnCandidateTarget(panel);
}


void ShapeShifterWindow::userTriedToCloseWindow()
{
	ShapeShifterManager::getInstance()->closePanelWindow(this,true);
}

var ShapeShifterWindow::getCurrentLayout()
{
	var data(new DynamicObject());
	data.getDynamicObject()->setProperty("panel", panel->getCurrentLayout());
	data.getDynamicObject()->setProperty("x", getBounds().getPosition().x);
	data.getDynamicObject()->setProperty("y", getBounds().getPosition().y);
	data.getDynamicObject()->setProperty("width", getWidth());
	data.getDynamicObject()->setProperty("height", getHeight());
	return data;
}

void ShapeShifterWindow::panelEmptied(ShapeShifterPanel *)
{
	ShapeShifterManager::getInstance()->closePanelWindow(this, true);
}
