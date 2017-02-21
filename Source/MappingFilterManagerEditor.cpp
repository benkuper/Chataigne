/*
  ==============================================================================

    MappingFilterManagerEditor.cpp
    Created: 4 Feb 2017 5:55:41pm
    Author:  Ben

  ==============================================================================
*/

#include "MappingFilterManagerEditor.h"
#include "MappingFilterFactory.h"

MappingFilterManagerEditor::MappingFilterManagerEditor(MappingFilterManager * m, bool isRoot) :
	GenericManagerEditor(m,isRoot)
{

}

MappingFilterManagerEditor::~MappingFilterManagerEditor()
{
}

void MappingFilterManagerEditor::showMenuAndAddItem(bool)
{
	MappingFilter * m = MappingFilterFactory::getInstance()->showCreateMenu();
	if (m != nullptr) manager->addItem(m);
}
