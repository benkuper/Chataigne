/*
  ==============================================================================

    QLabWorkspaceCommand.cpp
    Created: 14 May 2020 11:24:33am
    Author:  bkupe

  ==============================================================================
*/

#include "QLabWorkspaceCommand.h"

QLabWorkspaceCommand::QLabWorkspaceCommand(QLabModule* m, CommandContext context, var params, Multiplex * multiplex) :
    OSCCommand(m, context, params, multiplex),
    qlabModule(m)
{
    qlabModule->workspaceID->addParameterListener(this);
}

QLabWorkspaceCommand::~QLabWorkspaceCommand()
{
    if (!moduleRef.wasObjectDeleted()) qlabModule->workspaceID->removeParameterListener(this);
}

String QLabWorkspaceCommand::getTargetAddressInternal(const String& targetAddress, int multiplexIndex)
{
    if (!moduleRef.wasObjectDeleted()) return qlabModule->getWorkspacePrefix() + targetAddress;
    return targetAddress;
}


void QLabWorkspaceCommand::onExternalParameterValueChanged(Parameter* p)
{
    if (p == qlabModule->workspaceID) rebuildAddress();
    OSCCommand::onExternalParameterValueChanged(p);
}
