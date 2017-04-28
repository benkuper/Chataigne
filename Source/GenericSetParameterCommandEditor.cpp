/*
  ==============================================================================

    GenericSetParameterCommandEditor.cpp
    Created: 28 Apr 2017 5:52:28pm
    Author:  Ben

  ==============================================================================
*/

#include "GenericSetParameterCommandEditor.h"

GenericSetParameterCommandEditor::GenericSetParameterCommandEditor(GenericSetParameterCommand * _genericCommand, bool isRoot) :
	GenericControllableContainerEditor(_genericCommand, isRoot)
{

}

GenericSetParameterCommandEditor::~GenericSetParameterCommandEditor()
{
}
