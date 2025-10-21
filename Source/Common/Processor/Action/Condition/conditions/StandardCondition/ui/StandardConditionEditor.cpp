/*
  ==============================================================================

	StandardConditionEditor.cpp
	Created: 28 Oct 2016 8:07:05pm
	Author:  bkupe

  ==============================================================================
*/

StandardConditionEditor::StandardConditionEditor(StandardCondition* _condition, bool isRoot) :
	ConditionEditor(_condition, isRoot),
	standardCondition(_condition)
{
	targetUI.reset(standardCondition->sourceTarget->getEditor(false));
	addChildComponent(targetUI.get());

	alwaysTriggerUI.reset(standardCondition->alwaysTrigger->createToggle());
	addChildComponent(alwaysTriggerUI.get());


	toggleModeUI.reset(standardCondition->toggleMode->createToggle(ImageCache::getFromMemory(BinaryData::toggle_png, BinaryData::toggle_pngSize)));

	addChildComponent(toggleModeUI.get());

	targetUI->setVisible(!standardCondition->editorIsCollapsed);
	alwaysTriggerUI->setVisible(!standardCondition->editorIsCollapsed);
	toggleModeUI->setVisible(!standardCondition->editorIsCollapsed);

	setSize(100, 50);
	updateUI();
}

StandardConditionEditor::~StandardConditionEditor()
{
}

void StandardConditionEditor::setCollapsed(bool value, bool force, bool animate, bool doNotRebuild)
{
	ConditionEditor::setCollapsed(value, force, animate, doNotRebuild);

	targetUI->setVisible(!standardCondition->editorIsCollapsed);
	alwaysTriggerUI->setVisible(!standardCondition->editorIsCollapsed);
	toggleModeUI->setVisible(!standardCondition->editorIsCollapsed);

	//if (sourceFeedbackUI != nullptr)	sourceFeedbackUI->setVisible(!standardCondition->editorIsCollapsed);
	if (comparatorUI != nullptr) comparatorUI->setVisible(!standardCondition->editorIsCollapsed);
}

void StandardConditionEditor::resetAndBuild()
{
	// do nothing
	//DBG("Here reset and build");

}

void StandardConditionEditor::resizedInternalHeaderItemInternal(Rectangle<int> & r)
{
	if (sourceFeedbackUI != nullptr)
	{
		sourceFeedbackUI->setBounds(r.removeFromRight(jlimit(100, 300, r.getWidth())).reduced(2));
		r.removeFromRight(2);
	}
	BaseItemEditor::resizedInternalHeaderItemInternal(r);
}

void StandardConditionEditor::resizedInternalContent(Rectangle<int> & r)
{
	//ConditionEditor::resizedInternalContent(r);

	Rectangle<int> sr = r.withHeight(targetUI->getHeight()).reduced(2, 0);
	targetUI->setBounds(sr);
	r.translate(0, targetUI->getHeight()+2);

	int ch = comparatorUI != nullptr ? comparatorUI->getHeight() : 16;
	Rectangle<int> cr = r.withHeight(ch);
	
	toggleModeUI->setBounds(cr.removeFromLeft(16).withHeight(16));
	cr.removeFromLeft(2);

	alwaysTriggerUI->setBounds(cr.removeFromRight(95).withHeight(16));
	cr.removeFromRight(2);

	if (comparatorUI != nullptr) comparatorUI->setBounds(cr);

	r.translate(0, ch + 2);
	r.setHeight(0);
}

void StandardConditionEditor::updateUI()
{
	if (sourceFeedbackUI != nullptr) removeChildComponent(sourceFeedbackUI.get());

	if (Controllable * c = standardCondition->getSourceControllableAt(standardCondition->getPreviewIndex()))
	{
		sourceFeedbackUI.reset(c->createDefaultUI());
		if (c->type == Parameter::POINT2D)
		{
			if (DoubleSliderUI* dui = dynamic_cast<DoubleSliderUI*>(sourceFeedbackUI.get())) dui->canShowExtendedEditor = false;
		}
		addAndMakeVisible(sourceFeedbackUI.get());
	}

	if (comparatorUI != nullptr && comparatorUI->comparator != standardCondition->comparator.get())
	{
		removeChildComponent(comparatorUI.get());
		comparatorUI = nullptr;
	}
	if (standardCondition->comparator != nullptr)
	{
		comparatorUI.reset(standardCondition->comparator->createUI());
		addChildComponent(comparatorUI.get());
		comparatorUI->setVisible(!standardCondition->editorIsCollapsed);
	}

	targetUI->setVisible(!standardCondition->editorIsCollapsed);
	alwaysTriggerUI->setVisible(!standardCondition->editorIsCollapsed);
	toggleModeUI->setVisible(!standardCondition->editorIsCollapsed);

	resized();
}

void StandardConditionEditor::updateVisibility()
{
	InspectableEditor::updateVisibility();
	targetUI->setVisible(!standardCondition->editorIsCollapsed);
	alwaysTriggerUI->setVisible(!standardCondition->editorIsCollapsed);
	toggleModeUI->setVisible(!standardCondition->editorIsCollapsed);
	if (comparatorUI != nullptr) comparatorUI->setVisible(!standardCondition->editorIsCollapsed);
}

void StandardConditionEditor::childBoundsChanged(Component* c)
{
	ConditionEditor::childBoundsChanged(c);
	if (c == targetUI.get() || c == comparatorUI.get()) resized();
}
