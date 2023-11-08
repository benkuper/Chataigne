/*
  ==============================================================================

    ModuleIncludes3.cpp
    Created: 1 Mar 2022 4:03:40pm
    Author:  bkupe

  ==============================================================================
*/

#include "ModuleIncludes.h"

#include "MainIncludes.h"

#include "StateMachine/StateMachineIncludes.h" //for state module
#include "modules/state/StateModule.cpp"

#include "modules/custom/empty/EmptyModule.cpp"

#include "modules/serial/SerialModule.cpp"
#include "modules/state/commands/StateCommand.cpp"
#include "modules/system/os/OSModule.cpp"
#include "modules/system/os/commands/OSExecCommand.cpp"
#include "modules/system/os/commands/OSPowerCommand.cpp"
#include "modules/system/os/commands/OSWindowCommand.cpp"
#include "modules/system/os/commands/WakeOnLanCommand.cpp"
#include "modules/system/os/commands/ui/WakeOnLanCommandEditor.cpp"
#include "modules/system/time/TimeModule.cpp"
#include "modules/tcp/pjlink/PJLinkModule.cpp"
#include "modules/tcp/pjlink/commands/PJLinkCommand.cpp"
#include "modules/tcp/pjlink/ui/PJLinkModuleUI.cpp"
#include "modules/tcp/tcpclient/TCPClientModule.cpp"
#include "modules/tcp/tcpclient/watchout/WatchoutModule.cpp"
#include "modules/tcp/tcpclient/watchout/commands/WatchoutCommand.cpp"
#include "modules/tcp/tcpserver/TCPServerConnectionManager.cpp"
#include "modules/tcp/tcpserver/TCPServerModule.cpp"
#include "modules/tcp/tcpserver/ui/TCPServerModuleUI.cpp"
#include "modules/udp/UDPModule.cpp"
#include "modules/websocket/WebSocketClientModule.cpp"
#include "modules/websocket/WebSocketServerModule.cpp"
#include "modules/websocket/ui/WebSocketServerModuleUI.cpp"

#include "modules/abletonlink/AbletonLinkModule.cpp"

#include "modules/mqtt/MQTTModule.cpp"
#include "modules/mqtt/commands/MQTTCommands.cpp"
#include "modules/mqtt/ui/MQTTModuleUI.cpp"

#include "modules/posistagenet/PosiStageNetModule.cpp"
#include "modules/posistagenet/commands/PosiStageNetCommand.cpp"
