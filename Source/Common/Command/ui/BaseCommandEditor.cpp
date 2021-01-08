/*
  ==============================================================================

    BaseCommandEditor.cpp
    Created: 21 Dec 2020 11:09:27pm
    Author:  bkupe

  ==============================================================================
*/

#include "BaseCommandEditor.h"
#include "Common/ParameterLink/ui/LinkableParameterEditor.h"

BaseCommandContainerEditor::BaseCommandContainerEditor(BaseCommand* command, ControllableContainer * container, bool isRoot) :
    GenericControllableContainerEditor(container, isRoot, false),
    baseCommand(command)
{
    resetAndBuild(); //force here to use the overriden getEditorUI function
}

BaseCommandContainerEditor::~BaseCommandContainerEditor()
{
}


InspectableEditor* BaseCommandContainerEditor::getEditorUIForControllable(Controllable* c)
{
    if (!baseCommand->paramsCanBeLinked || c->type == c->TRIGGER) return  GenericControllableContainerEditor::getEditorUIForControllable(c);
    return new LinkableParameterEditor(baseCommand->paramLinkMap[(Parameter*)c], baseCommand->context == CommandContext::MAPPING);
}

InspectableEditor* BaseCommandContainerEditor::getEditorUIForContainer(ControllableContainer* cc)
{
    if (Script* s = dynamic_cast<Script*>(cc)) return GenericControllableContainerEditor::getEditorUIForContainer(cc);
    else if(cc == baseCommand->customValuesManager.get()) return GenericControllableContainerEditor::getEditorUIForContainer(cc);

    return new BaseCommandContainerEditor(baseCommand, cc, false);
}
