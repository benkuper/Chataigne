/*
  ==============================================================================

    QLabWorkspaceCommand.h
    Created: 14 May 2020 11:24:33am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class QLabWorkspaceCommand :
    public OSCCommand
{
public:
    QLabWorkspaceCommand(QLabModule* m, CommandContext context, var params, Multiplex * multiplex = nullptr);
    ~QLabWorkspaceCommand();

    QLabModule * qlabModule;
    
    String getTargetAddressInternal(const String& targetAddress, int multiplexIndex = 0) override;

    void onExternalParameterValueChanged(Parameter* p) override;

    static BaseCommand* create(ControllableContainer* module, CommandContext context, var params, Multiplex * multiplex) { return new QLabWorkspaceCommand((QLabModule*)module, context, params, multiplex); }
};
