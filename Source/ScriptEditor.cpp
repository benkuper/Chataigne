/*
  ==============================================================================

    ScriptEditor.cpp
    Created: 20 Feb 2017 5:04:20pm
    Author:  Ben

  ==============================================================================
*/

#include "ScriptEditor.h"
#include "AssetManager.h"
#include "TriggerImageUI.h"
#include "BoolToggleUI.h"

ScriptEditor::ScriptEditor(Script * _script, bool isRoot) :
	BaseItemEditor(_script,isRoot),
	script(_script),
	editMode(false)
{
	script->addAsyncScriptListener(this);

	fileBT = AssetManager::getInstance()->getFileBT();
	reloadBT = script->reload->createImageUI(AssetManager::getInstance()->getReloadImage());
	editBT = AssetManager::getInstance()->getEditBT();
	logUI = script->logParam->createToggle();

	fileBT->addListener(this);
	editBT->addListener(this);

	addAndMakeVisible(fileBT);
	addAndMakeVisible(reloadBT);
	addAndMakeVisible(editBT);
	addAndMakeVisible(logUI);
}

ScriptEditor::~ScriptEditor()
{
}

void ScriptEditor::paint(Graphics & g)
{
	BaseItemEditor::paint(g);
	Colour c = BG_COLOR;
	switch (script->state)
	{
	case Script::SCRIPT_LOADED:
		c = GREEN_COLOR;
		break;
	case Script::SCRIPT_ERROR:
		c = RED_COLOR;
		break;

	case Script::SCRIPT_EMPTY:
		c = BG_COLOR.brighter().withAlpha(.4f);
		break;
	}

	g.setColour(c);
	g.fillEllipse(statusBounds.reduced(2).toFloat());
}

void ScriptEditor::resizedInternalHeader(Rectangle<int>& r)
{
	statusBounds = r.removeFromRight(r.getHeight());
	r.removeFromRight(2);
	logUI->setBounds(r.removeFromRight(30));
	r.removeFromRight(2);
	editBT->setBounds(r.removeFromRight(r.getHeight()));
	r.removeFromRight(2);
	reloadBT->setBounds(r.removeFromRight(r.getHeight()));
	r.removeFromRight(2);
	fileBT->setBounds(r.removeFromRight(r.getHeight()));
	r.removeFromRight(2);
}

void ScriptEditor::resizedInternalContent(Rectangle<int>& r)
{
	if (!editMode) return;
	r.setHeight(100);
}


void ScriptEditor::newMessage(const Script::ScriptEvent & e)
{
	switch (e.type)
	{
	case Script::ScriptEvent::STATE_CHANGE:
		repaint();
		break;
	}
}

void ScriptEditor::buttonClicked(Button * b)
{
	BaseItemEditor::buttonClicked(b);
	if (b == fileBT)
	{
		FileChooser chooser("Load a cacahuete");
		bool result = chooser.browseForFileToOpen();
		if (result) script->filePath->setValue(chooser.getResult().getFullPathName());

	} else if (b == editBT)
	{
		editMode = !editMode;
		resized();
	}
}



