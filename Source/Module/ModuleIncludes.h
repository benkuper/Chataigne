/*
  ==============================================================================

    ModuleIncludes.h
    Created: 10 Mar 2021 9:51:51am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"


#include "Module.h"
#include "ModuleFactory.h"
#include "ModuleManager.h"

#include "Common/CommonIncludes.h"
#include "Common/Command/CommandIncludes.h"

#include "Community/CommunityModuleInfo.h"
#include "Community/CommunityModuleManager.h"
#include "Community/ui/CommunityModuleInfoEditor.h"

#include "Routing/ModuleRouterValue.h"
#include "Routing/ModuleRouter.h"
#include "Routing/ModuleRouterController.h"
#include "Routing/ModuleRouterManager.h"



#include "modules/audio/libs/pitch/PitchDetector.h"
#include "modules/audio/libs/pitch/PitchMPM.h"
#include "modules/audio/libs/pitch/PitchYIN.h"

#include "modules/audio/analysis/FFTAnalyzer.h"
#include "modules/audio/analysis/FFTAnalyzerManager.h"

#include "modules/audio/AudioModule.h"

#include "modules/audio/analysis/ui/FFTAnalyzerEditor.h"
#include "modules/audio/analysis/ui/FFTAnalyzerManagerEditor.h"


#include "modules/audio/commands/PlayAudioFileCommand.h"
#include "modules/audio/ui/AudioModuleHardwareEditor.h"


#include "modules/common/commands/scriptcallback/ScriptCallbackCommand.h"
#include "modules/common/commands/scriptcommands/ScriptCommand.h"

#include "modules/common/streaming/StreamingModule.h"
#include "modules/common/streaming/NetworkStreamingModule.h"

#include "modules/common/commands/generic/GenericControllableCommand.h"
#include "modules/common/osc/IOSCSenderModule.h"
#include "modules/common/commands/osc/OSCCommand.h"
#include "modules/common/commands/osc/CustomOSCCommand.h"

#include "modules/common/streaming/commands/StreamingCommand.h"
#include "modules/common/streaming/commands/SendStreamValuesCommand.h"
#include "modules/common/streaming/commands/SendStreamRawDataCommand.h"
#include "modules/common/streaming/commands/SendStreamStringCommand.h"
#include "modules/common/streaming/commands/SendStreamStringValuesCommand.h"

#include "modules/common/ui/EnablingNetworkControllableContainerEditor.h"


#include "modules/udp/UDPModule.h"
#include "modules/websocket/WebSocketClientModule.h"
#include "modules/websocket/WebSocketServerModule.h"

//#include "modules/controller/keyboard/KeyboardMacFunctions.h" //with #if JUCE_MAC inside KeyboardModule.cpp
#include "modules/controller/keyboard/WindowsHooker.h"
#include "modules/controller/keyboard/KeyboardModule.h"
#include "modules/controller/keyboard/commands/KeyboardModuleCommands.h"

#include "modules/controller/kinect/KinectV2Module.h"

//#include "modules/controller/mouse/MouseMacFunctions.h"  //with #if JUCE_MAC inside MouseModule.cpp
#include "modules/controller/mouse/MouseModule.h"
#include "modules/controller/mouse/commands/MouseModuleCommands.h"

#include "modules/controller/streamdeck/StreamDeck.h"
#include "modules/controller/streamdeck/StreamDeckManager.h"
#include "modules/controller/streamdeck/StreamDeckModule.h"
#include "modules/controller/streamdeck/commands/StreamDeckCommand.h"
#include "modules/controller/streamdeck/models/StreamDeckMini.h"
#include "modules/controller/streamdeck/models/StreamDeckV1.h"
#include "modules/controller/streamdeck/models/StreamDeckV2.h"
#include "modules/controller/streamdeck/models/StreamDeckXL.h"


#include "modules/controller/wiimote/WiimotePairUtil.h"
#include "modules/controller/wiimote/WiimoteManager.h"
#include "modules/controller/wiimote/WiimoteModule.h"

#include "modules/customvariables/CustomVariablesModule.h"
#include "modules/customvariables/commands/CustomVariablesModuleCommands.h"

#include "modules/dmx/DMXModule.h"
#include "modules/dmx/commands/DMXCommand.h"
#include "modules/dmx/ui/DMXModuleUI.h"

#include "modules/generators/metronome/MetronomeModule.h"

#include "modules/generators/signal/SignalModule.h"

#include "modules/abletonlink/AbletonLinkModule.h"

#include "modules/generic/ChataigneGenericModule.h"
#include "modules/generic/commands/ChataigneLogCommand.h"
#include "modules/generic/commands/GenericAppCommand.h"
#include "modules/generic/commands/GenericScriptCommand.h"
#include "modules/generic/commands/ChataigneDashboardCommand.h"

#include "modules/gpio/GPIOModule.h"
#include "modules/gpio/commands/GPIOCommands.h"

#include "modules/http/HTTPModule.h"
#include "modules/http/commands/HTTPCommand.h"

#include "modules/midi/MIDIClock.h"
#include "modules/midi/MIDIModule.h"
#include "modules/midi/commands/MIDICommands.h"

#include "modules/multiplex/MultiplexModule.h"
#include "modules/multiplex/commands/MultiplexCommands.h"

#include "modules/osc/OSCModule.h"

#include "modules/osc/custom/CustomOSCModule.h"
#include "modules/osc/dlight/DLightModule.h"


#include "modules/osc/heavym/HeavyMModule.h"

#include "modules/osc/live/LiveOSCModule.h"
#include "modules/osc/live/commands/LiveOSCCommandBase.h"

#include "modules/osc/millumin/MilluminModule.h"

#include "modules/osc/powerpoint/PowerpointModule.h"

#include "modules/osc/qlab/QLabModule.h"
#include "modules/osc/qlab/commands/QLabWorkspaceCommand.h"

#include "modules/osc/reaper/ReaperModule.h"
#include "modules/osc/reaper/commands/ReaperTimeCommand.h"

#include "modules/osc/resolume/ResolumeModule.h"
#include "modules/osc/resolume/commands/ResolumeBaseCommand.h"
#include "modules/osc/resolume/commands/ResolumeClipCommand.h"
#include "modules/osc/resolume/commands/ResolumeFXCommand.h"
#include "modules/osc/resolume/commands/ui/ResolumeBaseCommandEditor.h"

#include "modules/osc/ui/OSCOutputEditor.h"

#include "modules/oscquery/generic/GenericOSCQueryModule.h"
#include "modules/oscquery/generic/ui/OSCQueryModuleEditor.h"

#include "modules/oscquery/MadMapperModule.h"

#include "modules/sequence/commands/SequenceCommand.h"
#include "modules/sequence/commands/SequenceAudioCommand.h"
#include "modules/sequence/SequenceModule.h"

#include "modules/serial/SerialModule.h"
#include "modules/ble/BLEModule.h"

#include "modules/controller/loupedeck/LoupedeckShape.h"
#include "modules/controller/loupedeck/LoupedeckShapeManager.h"
#include "modules/controller/loupedeck/LoupedeckModule.h"
#include "modules/controller/loupedeck/commands/LoupedeckCommands.h"

#include "modules/custom/empty/EmptyModule.h"

#include "modules/state/commands/StateCommand.h"
#include "modules/state/StateModule.h"

#include "modules/system/os/OSModule.h"
#include "modules/system/os/commands/OSExecCommand.h"
#include "modules/system/os/commands/OSPowerCommand.h"
#include "modules/system/os/commands/OSWindowCommand.h"
#include "modules/system/os/commands/WakeOnLanCommand.h"
#include "modules/system/os/commands/ui/WakeOnLanCommandEditor.h"
#include "modules/system/time/TimeModule.h"

#include "modules/tcp/tcpclient/TCPClientModule.h"
#include "modules/tcp/pjlink/PJLinkModule.h"
#include "modules/tcp/pjlink/commands/PJLinkCommand.h"
#include "modules/tcp/pjlink/ui/PJLinkModuleUI.h"
#include "modules/tcp/tcpclient/watchout/WatchoutModule.h"
#include "modules/tcp/tcpclient/watchout/commands/WatchoutCommand.h"
#include "modules/tcp/tcpserver/TCPServerConnectionManager.h"
#include "modules/tcp/tcpserver/TCPServerModule.h"

#include "modules/mqtt/MQTTModule.h"
#include "modules/mqtt/commands/MQTTCommands.h"
#include "modules/mqtt/ui/MQTTModuleUI.h"

#include "modules/posistagenet/PosiStageNetModule.h"
#include "modules/posistagenet/commands/PosiStageNetCommand.h"

#include "ui/ModuleChooserUI.h"
#include "ui/ModuleUI.h"
#include "ui/ModuleManagerUI.h"

#include "Routing/ui/ModuleRouterValueEditor.h"
#include "Routing/ui/ModuleRouterView.h"
#include "Routing/ui/ModuleRouterPanelView.h"

#include "modules/tcp/tcpserver/ui/TCPServerModuleUI.h"
#include "modules/websocket/ui/WebSocketServerModuleUI.h"



