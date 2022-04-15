/*
  ==============================================================================

    MultiplexIndexConditionEditor.cpp
    Created: 15 Apr 2022 1:55:53pm
    Author:  bkupe

  ==============================================================================
*/

MultiplexIndexConditionEditor::MultiplexIndexConditionEditor(MultiplexIndexCondition* _condition, bool isRoot) :
	ConditionEditor(_condition, isRoot),
	multiplexIndexCondition(_condition)
{
	startAt0UI.reset(multiplexIndexCondition->startAt0->getEditor(false));
	addChildComponent(startAt0UI.get());

	startAt0UI->setVisible(!multiplexIndexCondition->editorIsCollapsed);

	comparatorUI.reset(multiplexIndexCondition->comp.createUI());
	addChildComponent(comparatorUI.get());
	comparatorUI->setVisible(!multiplexIndexCondition->editorIsCollapsed);

	setSize(100, 50);
}

MultiplexIndexConditionEditor::~MultiplexIndexConditionEditor()
{
}

void MultiplexIndexConditionEditor::setCollapsed(bool value, bool force, bool animate, bool doNotRebuild)
{
	ConditionEditor::setCollapsed(value, force, animate, doNotRebuild);

	startAt0UI->setVisible(!multiplexIndexCondition->editorIsCollapsed);
	comparatorUI->setVisible(!multiplexIndexCondition->editorIsCollapsed);
}

void MultiplexIndexConditionEditor::resetAndBuild()
{
	// do nothing
	//DBG("Here reset and build");
}

void MultiplexIndexConditionEditor::resizedInternalContent(Rectangle<int>& r)
{
	Rectangle<int> sr = r.withHeight(startAt0UI->getHeight()).reduced(2, 0);
	startAt0UI->setBounds(sr);
	r.translate(0, startAt0UI->getHeight() + 2);

	int ch = comparatorUI != nullptr ? comparatorUI->getHeight() : 16;
	Rectangle<int> cr = r.withHeight(ch);
	if (comparatorUI != nullptr) comparatorUI->setBounds(cr);

	r.translate(0, ch + 2);
	r.setHeight(0);
}

void MultiplexIndexConditionEditor::childBoundsChanged(Component* c)
{
	ConditionEditor::childBoundsChanged(c);
	if (c == startAt0UI.get()) resized();
}
