/*
  ==============================================================================

    ConditionEditor.cpp
    Created: 21 Feb 2017 11:40:21am
    Author:  Ben

  ==============================================================================
*/

ConditionEditor::ConditionEditor(Condition * _condition, bool isRoot) :
	BaseItemEditor(_condition, isRoot),
	condition(_condition),
	isCurrentInSequential(false)
{
	condition->addAsyncConditionListener(this);
	repaint();
}

ConditionEditor::~ConditionEditor()
{
	if (!inspectable.wasObjectDeleted())
	{
		condition->removeAsyncConditionListener(this);
	}
}

void ConditionEditor::paintOverChildren(Graphics & g)
{
	if (condition->getIsValid(condition->getPreviewIndex()) || isCurrentInSequential)
	{
		g.setColour(condition->enabled->boolValue() ? (isCurrentInSequential? BLUE_COLOR : GREEN_COLOR) : LIGHTCONTOUR_COLOR);
		g.drawRoundedRectangle(getLocalBounds().toFloat(), 2, 2);
	}
}

void ConditionEditor::conditionSourceChangedAsync(Condition *)
{
	updateUI();
}

void ConditionEditor::newMessage(const Condition::ConditionEvent &e)
{
	switch (e.type)
	{
	case Condition::ConditionEvent::VALIDATION_CHANGED:
		repaint();
		break;

	case Condition::ConditionEvent::SOURCE_CHANGED:
		conditionSourceChangedAsync(e.condition);
		break;

	case Condition::ConditionEvent::MULTIPLEX_PREVIEW_CHANGED:
		updateUI();
		break;
	}
}

void ConditionEditor::childBoundsChanged(Component * c)
{
	resized();
	repaint();
}
