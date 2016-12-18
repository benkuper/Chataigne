/*
  ==============================================================================

    GenericControllableContainerEditor.cpp
    Created: 9 May 2016 6:41:59pm
    Author:  bkupe

  ==============================================================================
*/

#include "GenericControllableContainerEditor.h"
#include "InspectableComponent.h"
#include "ControllableUI.h"

ControllableUIComparator CCInnerContainer::comparator;

GenericControllableContainerEditor::GenericControllableContainerEditor(ControllableContainer * _sourceContainer) :
	InspectorEditor((Inspectable *)_sourceContainer),
	sourceContainer(_sourceContainer),
	parentBT("Up","Go back to parent container")
{

	parentBT.addListener(this);
	addChildComponent(parentBT);	
	setCurrentInspectedContainer(sourceContainer);
	sourceContainer->addControllableContainerListener(this);

	resized();
}

GenericControllableContainerEditor::~GenericControllableContainerEditor()
{
	if(sourceContainer.get())sourceContainer->removeControllableContainerListener(this);
	parentBT.removeListener(this);
	innerContainer->clear();
}

void GenericControllableContainerEditor::setCurrentInspectedContainer(ControllableContainer * cc)
{
	if (cc == nullptr) return;

	if (innerContainer != nullptr)
	{
		if (cc == innerContainer->container) return;

		removeChildComponent(innerContainer);
		innerContainer = nullptr;
	}


	int ccLevel = 0;
	ControllableContainer * tc = cc;
	while (tc != sourceContainer)
	{
		ccLevel++;
		tc = tc->parentContainer;

		jassert(tc != nullptr); //If here, trying to inspect a container that is not a child of the source inspectable container
	}

	innerContainer = new CCInnerContainer(this,cc, 0, ccLevel == 0? sourceInspectable->recursiveInspectionLevel:0, sourceInspectable->canInspectChildContainersBeyondRecursion);
	addAndMakeVisible(innerContainer);


	parentBT.setVisible(ccLevel > 0);
	if(ccLevel > 0) parentBT.setButtonText("Up : " + cc->parentContainer->niceName);

	resized();

}

int GenericControllableContainerEditor::getContentHeight()
{
	if (innerContainer == nullptr) return InspectorEditor::getContentHeight();
	else return InspectorEditor::getContentHeight()+innerContainer->getContentHeight() + parentBT.getHeight() + 5;
}

void GenericControllableContainerEditor::resized()
{
	InspectorEditor::resized();

	if (innerContainer == nullptr) return;
	Rectangle<int> r = getLocalBounds();

	if (parentBT.isVisible())
	{
		parentBT.setBounds(r.removeFromTop(20));
		r.removeFromTop(2);
	}

	innerContainer->setBounds(r);
}

void GenericControllableContainerEditor::clear()
{
	if (innerContainer == nullptr) return;

	innerContainer->clear();
}

void GenericControllableContainerEditor::buttonClicked(Button * b)
{
	if (b == &parentBT)
	{
		setCurrentInspectedContainer(innerContainer->container->parentContainer);
	}
}

void GenericControllableContainerEditor::childStructureChanged(ControllableContainer *)
{
  postCommandMessage(CHILD_STRUCTURE_CHANGED);
	
}
void GenericControllableContainerEditor::handleCommandMessage(int cID){
  switch(cID){
    case CHILD_STRUCTURE_CHANGED:
      inspectorEditorListeners.call(&InspectorEditorListener::contentSizeChanged, this);
      break;
    default:
      jassertfalse;
      break;
  }
}

//Inner Container

CCInnerContainer::CCInnerContainer(GenericControllableContainerEditor * _editor, ControllableContainer * _container, int _level, int _maxLevel, bool _canAccessLowerContainers) :
	editor(_editor),
	container(_container),
	level(_level),
	maxLevel(_maxLevel),
	canAccessLowerContainers(_canAccessLowerContainers),
	containerLabel("containerLabel",_container->niceName)
{
	container->addAsyncContainerListener(this);

	addAndMakeVisible(containerLabel);
	containerLabel.setFont(containerLabel.getFont().withHeight(10));
	containerLabel.setColour(containerLabel.backgroundColourId,BG_COLOR.brighter(.2f));
	containerLabel.setSize(containerLabel.getFont().getStringWidth(containerLabel.getText()) + 10,14);
	containerLabel.setColour(containerLabel.textColourId, TEXTNAME_COLOR);

	if (container->canHavePresets)
	{
		presetChooser = new PresetChooser(container);
		addAndMakeVisible(presetChooser);
	}

	resetAndBuild();

}

void CCInnerContainer::resetAndBuild()
{
	clear();
	if (container->hideInEditor) return;

	for (auto &c : container->controllables)
	{
		if (!c->hideInEditor) addControllableUI(c);
	}

	if (level < maxLevel || container->recursiveInspectionLevel > 0)
	{
		for (auto &cc : container->controllableContainers)
		{
			if(!cc->hideInEditor) addCCInnerUI(cc);
		}
	} else if (level == maxLevel && canAccessLowerContainers)
	{
		for (auto &cc : container->controllableContainers)
		{
			if (!cc->hideInEditor) addCCLink(cc);
		}
	}
}

CCInnerContainer::~CCInnerContainer()
{
	container->removeAsyncContainerListener(this);
	clear();
}

void CCInnerContainer::addCCInnerUI(ControllableContainer * cc)
{
	CCInnerContainer * ccui = new CCInnerContainer(editor, cc, level + 1, maxLevel, canAccessLowerContainers);
	innerContainers.add(ccui);
	addAndMakeVisible(ccui);
}

void CCInnerContainer::removeCCInnerUI(ControllableContainer * cc)
{
	CCInnerContainer * ccui = getInnerContainerForCC(cc);
	if (ccui == nullptr) return;

	removeChildComponent(ccui);
	innerContainers.removeObject(ccui);
}

void CCInnerContainer::addCCLink(ControllableContainer * cc)
{
	CCLinkBT * bt = new CCLinkBT(cc);
	bt->addListener(this);
	addAndMakeVisible(bt);
	lowerContainerLinks.add(bt);
}

void CCInnerContainer::removeCCLink(ControllableContainer * cc)
{

	CCLinkBT * bt = getCCLinkForCC(cc);
	if (bt == nullptr) return;

	bt->removeListener(this);
	removeChildComponent(bt);
	lowerContainerLinks.removeObject(bt);
}

void CCInnerContainer::addControllableUI(Controllable * c)
{
	if (c->isControllableFeedbackOnly || !c->isControllableExposed) return;

	NamedControllableUI * cui = new NamedControllableUI(c->createDefaultUI(), 100);
	controllablesUI.add(cui);
	addAndMakeVisible(cui);
}

void CCInnerContainer::removeControllableUI(Controllable * c)
{
	NamedControllableUI * cui = getUIForControllable(c);
	if (cui == nullptr) return;

	removeChildComponent(cui);
	controllablesUI.removeObject(cui);

}

NamedControllableUI * CCInnerContainer::getUIForControllable(Controllable * c)
{
	for (auto &cui : controllablesUI)
	{
		if (cui->controllable == c) return cui;
	}

	return nullptr;
}

CCInnerContainer * CCInnerContainer::getInnerContainerForCC(ControllableContainer * cc)
{
	for (auto &ccui : innerContainers)
	{
		if (ccui->container == cc) return ccui;
	}

	return nullptr;
}

CCInnerContainer::CCLinkBT * CCInnerContainer::getCCLinkForCC(ControllableContainer * cc)
{
	for (auto &cclink : lowerContainerLinks)
	{
		if (cclink->targetContainer == cc) return cclink;
	}

	return nullptr;
}

void CCInnerContainer::newMessage(const ContainerAsyncEvent & p)
{
	switch (p.type)
	{
	case ContainerAsyncEvent::ControllableAdded:
		if (p.targetControllable->parentContainer != container) return;
		if (p.targetControllable->hideInEditor) return;
		addControllableUI(p.targetControllable);
		break;

	case ContainerAsyncEvent::ControllableRemoved:
		removeControllableUI(p.targetControllable);
		break;

	case ContainerAsyncEvent::ControllableContainerAdded:
		if (p.targetContainer->parentContainer != container) return;

		if (level < maxLevel) addCCInnerUI(p.targetContainer);
		else if (canAccessLowerContainers) addCCLink(p.targetContainer);
		break;

	case ContainerAsyncEvent::ControllableContainerRemoved:
		removeCCInnerUI(p.targetContainer);
		removeCCLink(p.targetContainer);
		break;

	case ContainerAsyncEvent::ChildStructureChanged:
		//nothing ?
		break;

	case ContainerAsyncEvent::ControllableContainerReordered:
		controllablesUI.sort(CCInnerContainer::comparator, true);
		resized();
		break;

    default:
    //not handled
    break;

	}
}

int CCInnerContainer::getContentHeight()
{
	int gap = 2;
	int ccGap = 5;
	int controllableHeight = 15;
	int ccLinkHeight = 20;
	int margin = 5;
	int presetChooserHeight = 15;

	int h = ccGap;
	h += controllablesUI.size()* (controllableHeight + gap) + ccGap;
	h += lowerContainerLinks.size() * (ccLinkHeight + gap) + ccGap;

	for (auto &ccui : innerContainers) h += ccui->getContentHeight() + ccGap;

	if(container->canHavePresets) h += presetChooserHeight + gap;
	h += containerLabel.getHeight();
	h += margin * 2;

	return h;
}



void CCInnerContainer::paint(Graphics & g)
{
	//if (level == 0) return;
	g.setColour(BG_COLOR.brighter(.3f));
	g.drawRoundedRectangle(getLocalBounds().toFloat(),4,2);
}

void CCInnerContainer::resized()
{
	int gap = 2;
	int ccGap = 5;
	int controllableHeight = 15;
	int ccLinkHeight = 20;
	int margin = 5;
	int presetChooserHeight = 15;

	Rectangle<int> r = getLocalBounds();
	containerLabel.setBounds(r.removeFromTop(containerLabel.getHeight()).withSizeKeepingCentre(containerLabel.getWidth(), containerLabel.getHeight()));

	r.reduce(margin, margin);

	if (container->canHavePresets)
	{
		presetChooser->setBounds(r.removeFromTop(presetChooserHeight));
		r.removeFromTop(gap);
	}

	for (auto &cui : controllablesUI)
	{
		cui->setBounds(r.removeFromTop(controllableHeight));
		r.removeFromTop(gap);
	}

	r.removeFromTop(ccGap);

	if (canAccessLowerContainers)
	{
		for (auto &cclink : lowerContainerLinks)
		{
			cclink->setBounds(r.removeFromTop(ccLinkHeight));
			r.removeFromTop(gap);
		}

		r.removeFromTop(ccGap);
	}

	for (auto &ccui : innerContainers)
	{
		ccui->setBounds(r.removeFromTop(ccui->getContentHeight()));
		r.removeFromTop(ccGap);
	}
}

void CCInnerContainer::clear()
{
	controllablesUI.clear();
	innerContainers.clear();
	lowerContainerLinks.clear();
}


void CCInnerContainer::buttonClicked(Button * b)
{
	CCLinkBT * bt = dynamic_cast<CCLinkBT *>(b);
	if (bt == nullptr) return;

	editor->setCurrentInspectedContainer(bt->targetContainer);
}

CCInnerContainer::CCLinkBT::CCLinkBT(ControllableContainer * _targetContainer) :
	targetContainer(_targetContainer),
	TextButton("[ Inspect "+_targetContainer->niceName+" >> ]")
{
}
