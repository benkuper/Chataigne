/*
  ==============================================================================

	ModuleIncludes2.cpp
	Created: 10 Mar 2021 12:02:16pm
	Author:  bkupe

  ==============================================================================
*/

#include "ModuleIncludes.h"
#include "MainIncludes.h"

#include "modules/dmx/DMXModule.cpp"
#include "modules/dmx/commands/DMXCommand.cpp"
#include "modules/dmx/ui/DMXModuleUI.cpp"
#include "modules/generators/metronome/MetronomeModule.cpp"
#include "modules/generators/signal/SignalModule.cpp"
#include "modules/generic/ChataigneGenericModule.cpp"
#include "modules/generic/commands/ChataigneLogCommand.cpp"
#include "modules/generic/commands/GenericAppCommand.cpp"
#include "modules/generic/commands/GenericScriptCommand.cpp"
#include "modules/generic/commands/ChataigneDashboardCommand.cpp"

#include "modules/common/commands/generic/GenericControllableCommand.cpp"
#include "modules/common/commands/osc/OSCCommand.cpp"
#include "modules/common/commands/osc/CustomOSCCommand.cpp"

#include "modules/gpio/GPIOModule.cpp"

#include "modules/gpio/commands/GPIOCommands.cpp"
#include "modules/http/HTTPModule.cpp"
#include "modules/http/commands/HTTPCommand.cpp"
#include "modules/midi/MIDIClock.cpp"
#include "modules/midi/MIDIModule.cpp"
#include "modules/midi/commands/MIDICommands.cpp"
#include "modules/multiplex/MultiplexModule.cpp"
#include "modules/multiplex/commands/MultiplexCommands.cpp"
#include "modules/osc/OSCModule.cpp"
#include "modules/osc/custom/CustomOSCModule.cpp"
#include "modules/osc/dlight/DLightModule.cpp"
#include "modules/osc/heavym/HeavyMModule.cpp"
#include "modules/osc/live/LiveOSCModule.cpp"
#include "modules/osc/live/commands/LiveOSCCommandBase.cpp"
#include "modules/osc/millumin/MilluminModule.cpp"
#include "modules/osc/powerpoint/PowerpointModule.cpp"
#include "modules/osc/qlab/QLabModule.cpp"
#include "modules/osc/qlab/commands/QLabWorkspaceCommand.cpp"
#include "modules/osc/reaper/ReaperModule.cpp"
#include "modules/osc/reaper/commands/ReaperTimeCommand.cpp"
#include "modules/osc/resolume/ResolumeModule.cpp"
#include "modules/osc/resolume/commands/ResolumeBaseCommand.cpp"
#include "modules/osc/resolume/commands/ResolumeClipCommand.cpp"
#include "modules/osc/resolume/commands/ResolumeFXCommand.cpp"
#include "modules/osc/resolume/commands/ui/ResolumeBaseCommandEditor.cpp"

#include "modules/ble/BLEModule.cpp"

#include "modules/osc/ui/OSCOutputEditor.cpp"
#include "modules/oscquery/MadMapperModule.cpp"

#include "modules/oscquery/generic/GenericOSCQueryModule.cpp"
#include "modules/oscquery/generic/ui/OSCQueryModuleEditor.cpp"

#include "modules/sequence/SequenceModule.cpp"
#include "modules/sequence/commands/SequenceAudioCommand.cpp"
#include "modules/sequence/commands/SequenceCommand.cpp"

