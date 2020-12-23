/*
  ==============================================================================

    QLabWorkspaceCommand.cpp
    Created: 14 May 2020 11:24:33am
    Author:  bkupe

  ==============================================================================
*/

#include "QLabWorkspaceCommand.h"

QLabWorkspaceCommand::QLabWorkspaceCommand(QLabModule* m, CommandContext context, var params, IteratorProcessor * iterator) :
    OSCCommand(m, context, params, iterator),
    qlabModule(m)
{
    qlabModule->workspaceID->addParameterListener(this);
}

QLabWorkspaceCommand::~QLabWorkspaceCommand()
{
    if (!moduleRef.wasObjectDeleted()) qlabModule->workspaceID->removeParameterListener(this);
}

void QLabWorkspaceCommand::rebuildAddressInternal(String& targetAddress)
{
    if (!moduleRef.wasObjectDeleted()) targetAddress = qlabModule->getWorkspacePrefix() + targetAddress;
}

void QLabWorkspaceCommand::onExternalParameterValueChanged(Parameter* p)
{
    if (p == qlabModule->workspaceID) rebuildAddress();
    OSCCommand::onExternalParameterValueChanged(p);
}
