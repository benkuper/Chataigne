/*
  ==============================================================================

    BaseCommandEditor.cpp
    Created: 21 Dec 2020 11:09:27pm
    Author:  bkupe

  ==============================================================================
*/

#include "BaseCommandEditor.h"
#include "Common/ParameterLink/ui/LinkableParameterEditor.h"

BaseCommandEditor::BaseCommandEditor(BaseCommand* command, bool isRoot) :
    GenericControllableContainerEditor(command, isRoot, false),
    baseCommand(command)
{
    resetAndBuild(); //force here to use the overriden getEditorUI function
}

BaseCommandEditor::~BaseCommandEditor()
{
}

InspectableEditor* BaseCommandEditor::getEditorUIForControllable(Controllable* c)
{
    if(!baseCommand->paramsCanBeLinked || c->type == c->TRIGGER) return  GenericControllableContainerEditor::getEditorUIForControllable(c);

    return new LinkableParameterEditor(baseCommand->paramLinkMap[(Parameter *)c], baseCommand->context == CommandContext::MAPPING);
}
