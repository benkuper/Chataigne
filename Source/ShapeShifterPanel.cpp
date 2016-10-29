/*
  ==============================================================================

    MovablePanel.cpp
    Created: 2 May 2016 3:08:37pm
    Author:  bkupe

  ==============================================================================
*/

#include "ShapeShifterPanel.h"
#include "Style.h"
#include "ShapeShifterManager.h"
#include "ShapeShifterWindow.h"
#include "ShapeShifterFactory.h"

ShapeShifterPanel::ShapeShifterPanel(ShapeShifterContent *_content, ShapeShifterPanelTab * sourceTab) :
	ShapeShifter(ShapeShifter::PANEL),
	currentContent(nullptr),
	transparentBackground(false),
	targetMode(false),
	candidateZone(NONE),
	candidateTargetPoint(Point<float>())
{
	addAndMakeVisible(header);
	header.addHeaderListener(this);

	if (_content != nullptr)
	{
		if (sourceTab == nullptr)
		{
			addContent(_content);
		}
		else
		{
			attachTab(sourceTab);
		}
	}

}

ShapeShifterPanel::~ShapeShifterPanel()
{
	for (auto &c : contents) c->contentIsShown = false;

	header.removeHeaderListener(this);
	listeners.call(&Listener::panelDestroyed, this);
}


void ShapeShifterPanel::setCurrentContent(ShapeShifterContent * _content)
{
	if (_content == currentContent) return;

	if (currentContent != nullptr)
	{
		ShapeShifterPanelTab * tab = header.getTabForContent(currentContent);
		if(tab != nullptr) tab->setSelected(false);
		removeChildComponent(currentContent->contentComponent);
		currentContent->contentIsShown = false;
	}


	currentContent = _content;

	if (currentContent != nullptr)
	{
		ShapeShifterPanelTab * tab = header.getTabForContent(currentContent);
		if (tab != nullptr) tab->setSelected(true);

		addAndMakeVisible(currentContent->contentComponent);

		currentContent->contentIsShown = true;
	}
	resized();
}

void ShapeShifterPanel::setCurrentContent(const String & name)
{
	ShapeShifterContent * c = getContentForName(name);
	if (c != nullptr) setCurrentContent(c);
}

void ShapeShifterPanel::setTargetMode(bool value)
{
	if (targetMode == value) return;
	targetMode = value;
	repaint();
}

void ShapeShifterPanel::paint(Graphics & g)
{
	g.setColour(BG_COLOR.withAlpha(transparentBackground?.3f:1));
	g.fillRect(getLocalBounds().withTrimmedTop(headerHeight));
}

void ShapeShifterPanel::paintOverChildren(Graphics & g)
{
	if (!targetMode) return;
	Rectangle<int> r = getLocalBounds();

	Colour hc = HIGHLIGHT_COLOR.withAlpha(.5f);
	Colour nc = NORMAL_COLOR.withAlpha(.3f);

	if (!isDetached())
	{
		g.setColour(candidateZone == AttachZone::TOP ? hc : nc);
		g.fillRect(r.withHeight(jmin<int>(10, getHeight() / 3)).reduced(jmin<int>(30, getWidth() / 5), 0));

		g.setColour(candidateZone == AttachZone::BOTTOM ? hc : nc);
		g.fillRect(getLocalBounds().removeFromBottom(jmin<int>(10, getHeight() / 3)).reduced(jmin<int>(30, getWidth() / 5), 0));

		g.setColour(candidateZone == AttachZone::LEFT ? hc : nc);
		g.fillRect(r.withWidth(jmin<int>(10, getWidth() / 3)).reduced(0, jmin<int>(30, getHeight() / 5)));

		g.setColour(candidateZone == AttachZone::RIGHT ? hc : nc);
		g.fillRect(getLocalBounds().removeFromRight(jmin<int>(10, getWidth() / 3)).withRight(getWidth()).reduced(0, jmin<int>(30, getHeight() / 5)));
	}

	g.setColour(candidateZone == AttachZone::CENTER ? hc : nc);
	g.fillRect(r.reduced(jmin<int>(50, getWidth() / 3), jmin<int>(50, getHeight() / 3)));

}

void ShapeShifterPanel::resized()
{
	Rectangle<int> r = getLocalBounds();
	header.setBounds(r.removeFromTop(headerHeight));
	if (currentContent != nullptr)
	{
		currentContent->contentComponent->setBounds(r);
	}
}

void ShapeShifterPanel::setTransparentBackground(bool value)
{
	if (transparentBackground == value) return;
	transparentBackground = value;
	repaint();
}

void ShapeShifterPanel::attachTab(ShapeShifterPanelTab * tab)
{

	header.attachTab(tab);

	contents.add(tab->content);
	setCurrentContent(tab->content);

}

void ShapeShifterPanel::detachTab(ShapeShifterPanelTab * tab, bool createNewPanel)
{
	ShapeShifterContent * content = tab->content;

	header.removeTab(tab, false);
	contents.removeObject(content, false);

	if (createNewPanel)
	{
		Rectangle<int> r = getScreenBounds();
		ShapeShifterPanel * newPanel = ShapeShifterManager::getInstance()->createPanel(content, tab);
		ShapeShifterManager::getInstance()->showPanelWindow(newPanel, r);
	}

	if (currentContent == content)
	{
		if (contents.size() > 0)
		{
			setCurrentContent(contents[juce::jlimit<int>(0,contents.size()-1, contents.indexOf(content))]);
		}else
		{
			listeners.call(&Listener::panelEmptied, this);
		}
	}





}

void ShapeShifterPanel::addContent(ShapeShifterContent * content, bool setCurrent)
{
	header.addTab(content);
	contents.add(content);
	if(setCurrent) setCurrentContent(content);
}

bool ShapeShifterPanel::hasContent(ShapeShifterContent * content)
{
	for (auto &c : contents)
	{
		if (c == content) return true;
	}

	return false;
}


bool ShapeShifterPanel::hasContent(const String &name)
{
	return getContentForName(name) != nullptr;
}

ShapeShifterContent * ShapeShifterPanel::getContentForName(const String & name)
{
	for (auto &c : contents)
	{
		if (c == nullptr) continue;
		if (c->contentName == name) return c;
	}

	return nullptr;
}

bool ShapeShifterPanel::isFlexible()
{
	if(currentContent == nullptr) return false;
	return currentContent->contentIsFlexible;
}

void ShapeShifterPanel::removeTab(ShapeShifterPanelTab * tab)
{
	ShapeShifterContent * content = tab->content;
	contents.removeObject(content, false);
	header.removeTab(tab, true);
	if (currentContent == content)
	{
		if (contents.size() > 0)
		{
			setCurrentContent(contents[juce::jlimit<int>(0, contents.size() - 1, contents.indexOf(content)-1)]);
		}
		else
		{
			listeners.call(&Listener::panelEmptied, this);
		}
	}

	listeners.call(&Listener::contentRemoved, content);
	delete content;

}

bool ShapeShifterPanel::attachPanel(ShapeShifterPanel * panel)
{
	switch (candidateZone)
	{
	case LEFT:
	case RIGHT:
	case TOP:
	case BOTTOM:
		if (parentContainer != nullptr) parentContainer->insertPanelRelative(panel,this,candidateZone);
		else return false;
		break;

	case NONE:
		return false;

	case CENTER:

		int numTabs = panel->header.tabs.size();

		while(numTabs > 0)
		{
			ShapeShifterPanelTab * t = panel->header.tabs[0];
			panel->detachTab(t,false);
			attachTab(t);
			numTabs--;
		}

		ShapeShifterManager::getInstance()->removePanel(panel);

		break;


	}

	return true;
}



ShapeShifterPanel::AttachZone ShapeShifterPanel::checkAttachZone(ShapeShifterPanel * source)
{
	AttachZone z = AttachZone::NONE;

	candidateTargetPoint = getLocalPoint(source, Point<float>());

	float rx = candidateTargetPoint.x / getWidth();
	float ry = candidateTargetPoint.y / getHeight();

	if (rx < 0 || rx > 1 || ry < 0 || ry > 1)
	{
		//keep none
	}else
	{
		if (rx < .2f) z = AttachZone::LEFT;
		else if (rx > .8f) z = AttachZone::RIGHT;
		else if (ry < .2f) z = AttachZone::TOP;
		else if (ry > .8f) z = AttachZone::BOTTOM;
		else z = AttachZone::CENTER;
	}

	if (isDetached() && z != AttachZone::CENTER) z = AttachZone::NONE;

	setCandidateZone(z);
	return candidateZone;
}

void ShapeShifterPanel::setCandidateZone(AttachZone zone)
{
	if (candidateZone == zone) return;
	candidateZone = zone;
	repaint();
}


var ShapeShifterPanel::getCurrentLayout()
{
	var layout = ShapeShifter::getCurrentLayout();
	var tabData;
	for (auto &t : header.tabs)
	{
		var tData(new DynamicObject());
		tData.getDynamicObject()->setProperty("name", t->content->contentName);
		tabData.append(tData);
	}

	if (currentContent != nullptr) layout.getDynamicObject()->setProperty("currentContent", currentContent->contentName);
 	layout.getDynamicObject()->setProperty("tabs", tabData);

	return layout;
}

void ShapeShifterPanel::loadLayoutInternal(var layout)
{
	juce::Array<var> * tabData = layout.getDynamicObject()->getProperty("tabs").getArray();

	if (tabData != nullptr)
	{
		for (auto &tData : *tabData)
		{
			ShapeShifterContent * c = ShapeShifterFactory::createContentForName(tData.getDynamicObject()->getProperty("name"));
			addContent(c);
		}
	}
	if (layout.getDynamicObject()->hasProperty("currentContent"))
	{
		DBG("Load panel current content " << layout.getDynamicObject()->getProperty("currentContent").toString());
		setCurrentContent(layout.getDynamicObject()->getProperty("currentContent").toString());
	}
}

void ShapeShifterPanel::tabDrag(ShapeShifterPanelTab * tab)
{
	if(!isDetached() || contents.size() > 1) detachTab(tab,true);
	else listeners.call(&Listener::tabDrag, this);
}

void ShapeShifterPanel::tabSelect(ShapeShifterPanelTab * tab)
{
	setCurrentContent(tab->content);
}

void ShapeShifterPanel::askForRemoveTab(ShapeShifterPanelTab * tab)
{
	removeTab(tab);
}

void ShapeShifterPanel::headerDrag()
{
	if (!isDetached())
	{
		DBG("Call panelDetach");
		listeners.call(&Listener::panelDetach, this);
	}
	else listeners.call(&Listener::headerDrag, this);
}
