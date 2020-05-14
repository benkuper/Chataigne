/*
  ==============================================================================

    QLabWorkspaceCommand.h
    Created: 14 May 2020 11:24:33am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../QLabModule.h"
#include "../../commands/OSCCommand.h"

class QLabWorkspaceCommand :
    public OSCCommand
{
public:
    QLabWorkspaceCommand(QLabModule* m, CommandContext context, var params);
    ~QLabWorkspaceCommand();

    QLabModule * qlabModule;
    
    void rebuildAddressInternal(String& targetAddress);

    void onExternalParameterValueChanged(Parameter* p) override;

    static BaseCommand* create(ControllableContainer* module, CommandContext context, var params) { return new QLabWorkspaceCommand((QLabModule*)module, context, params); }
};