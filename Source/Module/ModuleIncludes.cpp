/*
  ==============================================================================

    ModuleIncludes.cpp
    Created: 10 Mar 2021 9:51:52am
    Author:  bkupe

  ==============================================================================
*/

#include "ModuleIncludes.h"

#include "MainIncludes.h"

//include here to avoid loop-include
#include "ui/ModuleEditor.h"

#include "Common/Command/BaseCommandHandler.h"
#include "Common/Command/ui/BaseCommandHandlerEditor.h"
#include "ModuleCommandTester.h" 

#include "ui/ModuleCommandTesterEditor.h"



#include "Module.cpp"

#include "ModuleCommandTester.cpp"


//include those here to avoid conflict with SDL lib
#include "modules/controller/gamepad/GamepadModule.h"
#include "modules/controller/joycon/JoyConModule.h"
//#include "modules/controller/joystick/JoystickModule.h"

#include "ModuleFactory.cpp"

#include "ModuleManager.cpp"

#include "Community/CommunityModuleInfo.cpp"
#include "Community/CommunityModuleManager.cpp"
#include "Community/ui/CommunityModuleInfoEditor.cpp"

#include "Routing/ModuleRouter.cpp"
#include "Routing/ModuleRouterController.cpp"
#include "Routing/ModuleRouterManager.cpp"
#include "Routing/ModuleRouterValue.cpp"
#include "Routing/ui/ModuleRouterPanelView.cpp"
#include "Routing/ui/ModuleRouterValueEditor.cpp"
#include "Routing/ui/ModuleRouterView.cpp"
#include "modules/audio/AudioModule.cpp"
#include "modules/audio/analysis/FFTAnalyzer.cpp"
#include "modules/audio/analysis/FFTAnalyzerManager.cpp"
#include "modules/audio/analysis/ui/FFTAnalyzerEditor.cpp"
#include "modules/audio/analysis/ui/FFTAnalyzerManagerEditor.cpp"
#include "modules/audio/commands/PlayAudioFileCommand.cpp"
#include "modules/audio/ui/AudioModuleHardwareEditor.cpp"


#include "modules/common/commands/scriptcallback/ScriptCallbackCommand.cpp"
#include "modules/common/commands/scriptcommands/ScriptCommand.cpp"
#include "modules/common/streaming/NetworkStreamingModule.cpp"
#include "modules/common/streaming/StreamingModule.cpp"
#include "modules/common/streaming/commands/SendStreamRawDataCommand.cpp"
#include "modules/common/streaming/commands/SendStreamStringCommand.cpp"
#include "modules/common/streaming/commands/SendStreamStringValuesCommand.cpp"
#include "modules/common/streaming/commands/SendStreamValuesCommand.cpp"
#include "modules/common/streaming/commands/StreamingCommand.cpp"
#include "modules/common/ui/EnablingNetworkControllableContainerEditor.cpp"
#include "modules/controller/gamepad/GamepadModule.cpp"
#include "modules/controller/joycon/JoyConModule.cpp"
#include "modules/controller/keyboard/KeyboardModule.cpp"
#include "modules/controller/keyboard/WindowsHooker.cpp"
#include "modules/controller/keyboard/commands/KeyboardModuleCommands.cpp"
#include "modules/controller/kinect/KinectV2Module.cpp"
#include "modules/controller/mouse/MouseModule.cpp"
#include "modules/controller/mouse/commands/MouseModuleCommands.cpp"
#include "modules/controller/streamdeck/StreamDeck.cpp"
#include "modules/controller/streamdeck/StreamDeckManager.cpp"
#include "modules/controller/streamdeck/StreamDeckModule.cpp"
#include "modules/controller/streamdeck/commands/StreamDeckCommand.cpp"
#include "modules/controller/streamdeck/models/StreamDeckMini.cpp"
#include "modules/controller/streamdeck/models/StreamDeckV1.cpp"
#include "modules/controller/streamdeck/models/StreamDeckV2.cpp"
#include "modules/controller/streamdeck/models/StreamDeckXL.cpp"

#include "modules/controller/loupedeck/LoupedeckShape.cpp"
#include "modules/controller/loupedeck/LoupedeckShapeManager.cpp"
#include "modules/controller/loupedeck/LoupedeckModule.cpp"
#include "modules/controller/loupedeck/commands/LoupedeckCommands.cpp"

#include "modules/controller/wiimote/WiimoteManager.cpp"
#include "modules/controller/wiimote/WiimoteModule.cpp"
#include "modules/controller/wiimote/WiimotePairUtil.cpp"
#include "modules/customvariables/CustomVariablesModule.cpp"
#include "modules/customvariables/commands/CustomVariablesModuleCommands.cpp"

#include "ui/ModuleChooserUI.cpp"
#include "ui/ModuleCommandTesterEditor.cpp"
#include "ui/ModuleEditor.cpp"
#include "ui/ModuleManagerUI.cpp"
#include "ui/ModuleUI.cpp"


