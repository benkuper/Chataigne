# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# build with "V=1" for verbose builds
ifeq ($(V), 1)
V_AT =
else
V_AT = @
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef STRIP
  STRIP=strip
endif

ifndef AR
  AR=ar
endif

ifndef CONFIG
  CONFIG=Release
endif

JUCE_ARCH_LABEL := $(shell uname -m)

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -m64
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=1.4.6 -DJUCE_APP_VERSION_HEX=0x10406 $(shell pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I$(HOME)/JUCE/modules -I../../../JUCE\ Modules -I../../External/wiiuse -I../../External/serial/include -I../../External/servus/include -I../../External/dnssd/include -I../../External/ossia/include -I../../Source $(CPPFLAGS)
  JUCE_CPPFLAGS_APP := -DJucePlugin_Build_VST=0 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=0
  JUCE_TARGET_APP := Chataigne

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -O3 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++14 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L../../External/servus/lib/linux -L../../External/ossia/lib/linux $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama) -fvisibility=hidden -Wl,-rpath,"lib" -lGL -ldl -lpthread -lrt -lbluetooth -lServus $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -m64
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=1.4.6 -DJUCE_APP_VERSION_HEX=0x10406 $(shell pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I$(HOME)/JUCE/modules -I../../../JUCE\ Modules -I../../External/wiiuse -I../../External/serial/include -I../../External/servus/include -I../../External/dnssd/include -I../../External/ossia/include -I../../Source $(CPPFLAGS)
  JUCE_CPPFLAGS_APP := -DJucePlugin_Build_VST=0 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=0
  JUCE_TARGET_APP := Chataigne

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++14 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L../../External/servus/lib/linux -L../../External/ossia/lib/linux $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama) -Wl,-rpath,"lib" -lGL -ldl -lpthread -lrt -lbluetooth -lServus $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS_APP := \
  $(JUCE_OBJDIR)/CommandTemplateEditor_5399b8ad.o \
  $(JUCE_OBJDIR)/CommandTemplateManagerUI_dc305a4b.o \
  $(JUCE_OBJDIR)/CommandTemplateParameterEditor_a7196f80.o \
  $(JUCE_OBJDIR)/CommandTemplate_5181ffb9.o \
  $(JUCE_OBJDIR)/CommandTemplateFactory_66cf2d1b.o \
  $(JUCE_OBJDIR)/CommandTemplateManager_6487023e.o \
  $(JUCE_OBJDIR)/CommandTemplateParameter_bfe2213a.o \
  $(JUCE_OBJDIR)/BaseCommand_2318dc9b.o \
  $(JUCE_OBJDIR)/BaseCommandHandler_4d357439.o \
  $(JUCE_OBJDIR)/CommandDefinition_7c3c255f.o \
  $(JUCE_OBJDIR)/CommandDefinitionManager_7c0c55d8.o \
  $(JUCE_OBJDIR)/CommandFactory_3a72e748.o \
  $(JUCE_OBJDIR)/BaseCommandHandlerEditor_d6f421f9.o \
  $(JUCE_OBJDIR)/CommandChooserUI_dcc0e740.o \
  $(JUCE_OBJDIR)/DMXArtNetDevice_5a61b42b.o \
  $(JUCE_OBJDIR)/DMXDevice_462f8cd1.o \
  $(JUCE_OBJDIR)/DMXEnttecProDevice_e054588b.o \
  $(JUCE_OBJDIR)/DMXOpenUSBDevice_ad4fdeff.o \
  $(JUCE_OBJDIR)/DMXSerialDevice_44a8d965.o \
  $(JUCE_OBJDIR)/DMXDashboard_2d5d2a45.o \
  $(JUCE_OBJDIR)/DMXStateManagerUI_7bd1c2a9.o \
  $(JUCE_OBJDIR)/DMXStateUI_a98d2d16.o \
  $(JUCE_OBJDIR)/DMXManager_b583f7e3.o \
  $(JUCE_OBJDIR)/DMXState_b401a707.o \
  $(JUCE_OBJDIR)/DMXStateManager_90092730.o \
  $(JUCE_OBJDIR)/MIDIDeviceChooser_959781bc.o \
  $(JUCE_OBJDIR)/MIDIDeviceParameterUI_bf16f57e.o \
  $(JUCE_OBJDIR)/MIDIDevice_3c053f6a.o \
  $(JUCE_OBJDIR)/MIDIDeviceParameter_f25cf8a9.o \
  $(JUCE_OBJDIR)/MIDIManager_a5993663.o \
  $(JUCE_OBJDIR)/PluginManager_69ed7623.o \
  $(JUCE_OBJDIR)/BoolComparators_9c68289d.o \
  $(JUCE_OBJDIR)/EnumComparator_a53f6ef7.o \
  $(JUCE_OBJDIR)/NumberComparators_25bd9d1e.o \
  $(JUCE_OBJDIR)/Point2DComparators_4dbb4bdb.o \
  $(JUCE_OBJDIR)/Point3DComparators_9e6529dc.o \
  $(JUCE_OBJDIR)/StringComparators_61def56.o \
  $(JUCE_OBJDIR)/BaseComparatorUI_a3309572.o \
  $(JUCE_OBJDIR)/BaseComparator_c76f1d55.o \
  $(JUCE_OBJDIR)/ActivationCondition_54e1de99.o \
  $(JUCE_OBJDIR)/ConditionGroup_1310754d.o \
  $(JUCE_OBJDIR)/StandardConditionEditor_d7753ad.o \
  $(JUCE_OBJDIR)/StandardCondition_9fb607cb.o \
  $(JUCE_OBJDIR)/ScriptCondition_eadfe96f.o \
  $(JUCE_OBJDIR)/ConditionEditor_15d65f42.o \
  $(JUCE_OBJDIR)/ConditionManagerEditor_94d9f8f.o \
  $(JUCE_OBJDIR)/Condition_d04eabf0.o \
  $(JUCE_OBJDIR)/ConditionManager_83c17a27.o \
  $(JUCE_OBJDIR)/Consequence_a3851740.o \
  $(JUCE_OBJDIR)/ConsequenceManager_5beab4d7.o \
  $(JUCE_OBJDIR)/ActionUI_5ace508e.o \
  $(JUCE_OBJDIR)/Action_6a925b7.o \
  $(JUCE_OBJDIR)/MappingFilter_806fb652.o \
  $(JUCE_OBJDIR)/MappingFilterManager_2c745d85.o \
  $(JUCE_OBJDIR)/MappingFilterEditor_f7157696.o \
  $(JUCE_OBJDIR)/ConversionFilters_b963b15d.o \
  $(JUCE_OBJDIR)/CropFilter_481b2f1a.o \
  $(JUCE_OBJDIR)/CurveMapFilter_75770fb7.o \
  $(JUCE_OBJDIR)/InverseFilter_e1c4de20.o \
  $(JUCE_OBJDIR)/LagFilter_219d5b82.o \
  $(JUCE_OBJDIR)/OffsetFilter_3759d61d.o \
  $(JUCE_OBJDIR)/ScriptFilter_49f85c55.o \
  $(JUCE_OBJDIR)/SimpleRemapFilter_1d686067.o \
  $(JUCE_OBJDIR)/SimpleSmoothFilter_9930618a.o \
  $(JUCE_OBJDIR)/MappingInputEditor_3aad72e6.o \
  $(JUCE_OBJDIR)/MappingInput_d2e57b44.o \
  $(JUCE_OBJDIR)/MappingOutputManagerEditor_c1da629b.o \
  $(JUCE_OBJDIR)/MappingOutput_feb174a4.o \
  $(JUCE_OBJDIR)/MappingOutputManager_b8813ef3.o \
  $(JUCE_OBJDIR)/MappingUI_939edaca.o \
  $(JUCE_OBJDIR)/Mapping_78a2fa15.o \
  $(JUCE_OBJDIR)/ProcessorManagerUI_f8839312.o \
  $(JUCE_OBJDIR)/ProcessorUI_b6bd10cd.o \
  $(JUCE_OBJDIR)/Processor_e39625fa.o \
  $(JUCE_OBJDIR)/ProcessorManager_e0c880dd.o \
  $(JUCE_OBJDIR)/cobs_e77e740f.o \
  $(JUCE_OBJDIR)/serial_82f97e54.o \
  $(JUCE_OBJDIR)/list_ports_linux_203b2613.o \
  $(JUCE_OBJDIR)/list_ports_osx_d93ade33.o \
  $(JUCE_OBJDIR)/list_ports_win_e6502c6b.o \
  $(JUCE_OBJDIR)/unix_d5f5c79d.o \
  $(JUCE_OBJDIR)/win_4c1bdbbb.o \
  $(JUCE_OBJDIR)/SerialDevice_e404848a.o \
  $(JUCE_OBJDIR)/SerialDeviceParameter_2de97789.o \
  $(JUCE_OBJDIR)/SerialManager_fd829543.o \
  $(JUCE_OBJDIR)/CVPresetEditor_83210cfc.o \
  $(JUCE_OBJDIR)/CVPreset_3c4a8f1a.o \
  $(JUCE_OBJDIR)/CVPresetManager_6d2193bd.o \
  $(JUCE_OBJDIR)/CVGroupManagerUI_95d59108.o \
  $(JUCE_OBJDIR)/CVGroup_5e5064a4.o \
  $(JUCE_OBJDIR)/CVGroupManager_45344ef3.o \
  $(JUCE_OBJDIR)/CustomVariablesModuleCommands_edf5eec8.o \
  $(JUCE_OBJDIR)/CustomVariablesModule_cdc46d67.o \
  $(JUCE_OBJDIR)/AudioModuleHardwareEditor_e9c8a821.o \
  $(JUCE_OBJDIR)/AudioModule_84057627.o \
  $(JUCE_OBJDIR)/CustomValuesCommandArgumentEditor_1cc8b6de.o \
  $(JUCE_OBJDIR)/CustomValuesCommandArgumentManagerEditor_8192c073.o \
  $(JUCE_OBJDIR)/CustomValuesCommand_2f0f022b.o \
  $(JUCE_OBJDIR)/CustomValuesCommandArgument_96a63888.o \
  $(JUCE_OBJDIR)/CustomValuesCommandArgumentManager_fc7128f.o \
  $(JUCE_OBJDIR)/SendStreamRawDataCommand_ca89c0e3.o \
  $(JUCE_OBJDIR)/SendStreamStringCommand_d6c0655a.o \
  $(JUCE_OBJDIR)/SendStreamStringValuesCommand_814cbb58.o \
  $(JUCE_OBJDIR)/SendStreamValuesCommand_7fe403a9.o \
  $(JUCE_OBJDIR)/StreamingCommand_19b7802b.o \
  $(JUCE_OBJDIR)/NetworkStreamingModule_a6ae39cf.o \
  $(JUCE_OBJDIR)/StreamingModule_8e8f50a9.o \
  $(JUCE_OBJDIR)/EnablingNetworkControllableContainerEditor_5d799d9a.o \
  $(JUCE_OBJDIR)/GamepadManager_dea1568b.o \
  $(JUCE_OBJDIR)/GamepadModule_fc770758.o \
  $(JUCE_OBJDIR)/HIDModule_c3f9a29c.o \
  $(JUCE_OBJDIR)/JoyConModule_1410c7f4.o \
  $(JUCE_OBJDIR)/KeyboardModule_965e79f4.o \
  $(JUCE_OBJDIR)/KinectV2Module_99832bf0.o \
  $(JUCE_OBJDIR)/MyoDevice_cb8c0306.o \
  $(JUCE_OBJDIR)/MyoManager_6eae647.o \
  $(JUCE_OBJDIR)/MyoModule_691e901c.o \
  $(JUCE_OBJDIR)/os_mac_a23c8a30.o \
  $(JUCE_OBJDIR)/classic_e456a893.o \
  $(JUCE_OBJDIR)/dynamics_a64483fd.o \
  $(JUCE_OBJDIR)/events_e669bcc2.o \
  $(JUCE_OBJDIR)/guitar_hero_3_a0003144.o \
  $(JUCE_OBJDIR)/io_7ae9338f.o \
  $(JUCE_OBJDIR)/ir_7ae93ed2.o \
  $(JUCE_OBJDIR)/motion_plus_4f38fa44.o \
  $(JUCE_OBJDIR)/nunchuk_7e0009c3.o \
  $(JUCE_OBJDIR)/os_nix_4c40caab.o \
  $(JUCE_OBJDIR)/os_win_4cbf78aa.o \
  $(JUCE_OBJDIR)/util_7a46226b.o \
  $(JUCE_OBJDIR)/wiiboard_b3f0e298.o \
  $(JUCE_OBJDIR)/wiiuse_8bc34039.o \
  $(JUCE_OBJDIR)/WiimoteManager_c0441739.o \
  $(JUCE_OBJDIR)/WiimoteModule_144278ea.o \
  $(JUCE_OBJDIR)/WiimotePairUtil_320e063a.o \
  $(JUCE_OBJDIR)/DMXCommand_23f829a1.o \
  $(JUCE_OBJDIR)/DMXModule_c47853d9.o \
  $(JUCE_OBJDIR)/MetronomeModule_6453731e.o \
  $(JUCE_OBJDIR)/SignalModule_9a14db38.o \
  $(JUCE_OBJDIR)/ChataigneLogCommand_8373a622.o \
  $(JUCE_OBJDIR)/GenericControllableCommand_caf6da58.o \
  $(JUCE_OBJDIR)/ChataigneGenericModule_d6802829.o \
  $(JUCE_OBJDIR)/MIDICommands_121216e.o \
  $(JUCE_OBJDIR)/LaunchpadModule_654c7bb.o \
  $(JUCE_OBJDIR)/MIDIModule_d3422ef9.o \
  $(JUCE_OBJDIR)/OSCCommand_779cc3a1.o \
  $(JUCE_OBJDIR)/UserOSCCommandModelEditor_64a71933.o \
  $(JUCE_OBJDIR)/UserOSCCommandModelManagerUI_3fe3e405.o \
  $(JUCE_OBJDIR)/UserOSCCommandModelUI_66d9b3a.o \
  $(JUCE_OBJDIR)/UserOSCCommandModel_399f4bed.o \
  $(JUCE_OBJDIR)/UserOSCCommandModelManager_84d9538a.o \
  $(JUCE_OBJDIR)/UserOSCCommandEditor_26442000.o \
  $(JUCE_OBJDIR)/UserOSCCommand_7c414100.o \
  $(JUCE_OBJDIR)/CustomOSCCommand_61f60882.o \
  $(JUCE_OBJDIR)/CustomOSCModule_1d5ea018.o \
  $(JUCE_OBJDIR)/DLightModule_37bf0649.o \
  $(JUCE_OBJDIR)/LiveOSCCommandBase_2512fc69.o \
  $(JUCE_OBJDIR)/LiveOSCSceneCommand_fb53785c.o \
  $(JUCE_OBJDIR)/LiveOSCModule_ae0bbb38.o \
  $(JUCE_OBJDIR)/MilluminModule_e03eaa89.o \
  $(JUCE_OBJDIR)/ReaperTimeCommandUI_18f099cb.o \
  $(JUCE_OBJDIR)/ReaperTimeCommand_2fc352d6.o \
  $(JUCE_OBJDIR)/ReaperModule_95c95bc9.o \
  $(JUCE_OBJDIR)/ResolumeBaseCommandEditor_c6593aa0.o \
  $(JUCE_OBJDIR)/ResolumeBaseCommand_9ab5c8d0.o \
  $(JUCE_OBJDIR)/ResolumeClipCommand_ac9cb6f1.o \
  $(JUCE_OBJDIR)/ResolumeFXCommand_82bf808f.o \
  $(JUCE_OBJDIR)/ResolumeModule_61b1e469.o \
  $(JUCE_OBJDIR)/OSCModule_5d38d7b9.o \
  $(JUCE_OBJDIR)/SequenceCommand_5137c44f.o \
  $(JUCE_OBJDIR)/SequenceModule_6db3e719.o \
  $(JUCE_OBJDIR)/SerialModule_a9d8bfb9.o \
  $(JUCE_OBJDIR)/StateCommand_1e7fa1.o \
  $(JUCE_OBJDIR)/StateModule_898183dd.o \
  $(JUCE_OBJDIR)/OSExecCommand_1adb1a06.o \
  $(JUCE_OBJDIR)/OSPowerCommand_8563ee44.o \
  $(JUCE_OBJDIR)/WakeOnLanCommand_f0bfe78f.o \
  $(JUCE_OBJDIR)/OSModule_9cf844c7.o \
  $(JUCE_OBJDIR)/TimeModule_26aecc47.o \
  $(JUCE_OBJDIR)/PJLinkModule_528c0f0b.o \
  $(JUCE_OBJDIR)/VLCModule_e43b1167.o \
  $(JUCE_OBJDIR)/WatchoutCommand_1abb5d65.o \
  $(JUCE_OBJDIR)/WatchoutModule_630ef32b.o \
  $(JUCE_OBJDIR)/TCPModule_24a2937d.o \
  $(JUCE_OBJDIR)/UDPModule_4d12fb3d.o \
  $(JUCE_OBJDIR)/ModuleRouterPanelView_3de9a86.o \
  $(JUCE_OBJDIR)/ModuleRouterValueEditor_7fd5857b.o \
  $(JUCE_OBJDIR)/ModuleRouterView_be67d052.o \
  $(JUCE_OBJDIR)/ModuleRouter_b990f562.o \
  $(JUCE_OBJDIR)/ModuleRouterManager_84510c75.o \
  $(JUCE_OBJDIR)/ModuleRouterValue_107f14f9.o \
  $(JUCE_OBJDIR)/ModuleChooserUI_a3310ecc.o \
  $(JUCE_OBJDIR)/ModuleEditor_e010c1a.o \
  $(JUCE_OBJDIR)/ModuleManagerUI_da55b3be.o \
  $(JUCE_OBJDIR)/ModuleUI_510b19a1.o \
  $(JUCE_OBJDIR)/Module_14b9fb70.o \
  $(JUCE_OBJDIR)/ModuleFactory_f0ade584.o \
  $(JUCE_OBJDIR)/ModuleManager_ee65baa7.o \
  $(JUCE_OBJDIR)/StateUI_cf33c402.o \
  $(JUCE_OBJDIR)/StateViewUI_87e08687.o \
  $(JUCE_OBJDIR)/State_6ea1d8c3.o \
  $(JUCE_OBJDIR)/StateTransitionManagerUI_2535d416.o \
  $(JUCE_OBJDIR)/StateTransitionUI_2782c49.o \
  $(JUCE_OBJDIR)/StateTransition_772f2d26.o \
  $(JUCE_OBJDIR)/StateTransitionManager_c1fd5031.o \
  $(JUCE_OBJDIR)/StateMachineView_e5e08766.o \
  $(JUCE_OBJDIR)/StateManagerUI_a2d9b59b.o \
  $(JUCE_OBJDIR)/StateManager_ec4bbd6.o \
  $(JUCE_OBJDIR)/TimeCueManagerUI_b2d29c4a.o \
  $(JUCE_OBJDIR)/TimeCueUI_c10d7695.o \
  $(JUCE_OBJDIR)/TimeCue_a13ab31a.o \
  $(JUCE_OBJDIR)/TimeCueManager_4480efbd.o \
  $(JUCE_OBJDIR)/AudioLayerClipManagerUI_9973a193.o \
  $(JUCE_OBJDIR)/AudioLayerClipUI_e31de16c.o \
  $(JUCE_OBJDIR)/AudioLayerPanel_51233d86.o \
  $(JUCE_OBJDIR)/AudioLayerTimeline_28b4bc89.o \
  $(JUCE_OBJDIR)/AudioLayer_c91e7f33.o \
  $(JUCE_OBJDIR)/AudioLayerClip_604aefa3.o \
  $(JUCE_OBJDIR)/AudioLayerClipManager_d1763714.o \
  $(JUCE_OBJDIR)/TimeColorManagerUI_d8674ab7.o \
  $(JUCE_OBJDIR)/TimeColorUI_4c8e78c8.o \
  $(JUCE_OBJDIR)/TimeColor_f2e47aa7.o \
  $(JUCE_OBJDIR)/TimeColorManager_b39eff90.o \
  $(JUCE_OBJDIR)/MappingLayerEditor_949073f5.o \
  $(JUCE_OBJDIR)/MappingLayerPanel_f16bd606.o \
  $(JUCE_OBJDIR)/MappingLayerTimeline_88bf5409.o \
  $(JUCE_OBJDIR)/MappingLayer_461e6c03.o \
  $(JUCE_OBJDIR)/TimeTriggerManagerUI_3a8b4b85.o \
  $(JUCE_OBJDIR)/TimeTriggerMultiTransformer_e11cdfa6.o \
  $(JUCE_OBJDIR)/TimeTriggerUI_1103a3ba.o \
  $(JUCE_OBJDIR)/TriggerLayerPanel_42a36fc6.o \
  $(JUCE_OBJDIR)/TriggerLayerTimeline_de0c6249.o \
  $(JUCE_OBJDIR)/TimeTrigger_4aecc13f.o \
  $(JUCE_OBJDIR)/TimeTriggerManager_28e9df8.o \
  $(JUCE_OBJDIR)/TriggerLayer_270d0aaf.o \
  $(JUCE_OBJDIR)/SequenceLayerPanel_b503e081.o \
  $(JUCE_OBJDIR)/SequenceLayerPanelManagerUI_b9ca64ca.o \
  $(JUCE_OBJDIR)/SequenceLayerTimeline_2eaf3ae.o \
  $(JUCE_OBJDIR)/SequenceLayerTimelineManagerUI_8411943d.o \
  $(JUCE_OBJDIR)/SequenceLayer_c9c02da2.o \
  $(JUCE_OBJDIR)/SequenceLayerFactory_eda63712.o \
  $(JUCE_OBJDIR)/SequenceLayerManager_eb5e0c35.o \
  $(JUCE_OBJDIR)/SequenceEditor_88b31cd9.o \
  $(JUCE_OBJDIR)/SequenceTimelineHeader_e6ae3a1a.o \
  $(JUCE_OBJDIR)/SequenceTimelineNavigationUI_7c174fb5.o \
  $(JUCE_OBJDIR)/SequenceTimelineSeeker_77a668b2.o \
  $(JUCE_OBJDIR)/SequenceTransportUI_f5171adb.o \
  $(JUCE_OBJDIR)/SequenceUI_d0a961e0.o \
  $(JUCE_OBJDIR)/Sequence_b81dca9b.o \
  $(JUCE_OBJDIR)/SequenceManagerUI_68ff9d23.o \
  $(JUCE_OBJDIR)/TimeMachineView_84e65f62.o \
  $(JUCE_OBJDIR)/SequenceManager_b4b96c18.o \
  $(JUCE_OBJDIR)/AboutWindow_970e3d2b.o \
  $(JUCE_OBJDIR)/ChataigneAssetManager_b7ab91a9.o \
  $(JUCE_OBJDIR)/ChataigneEngine_2d424321.o \
  $(JUCE_OBJDIR)/Main_90ebc5c2.o \
  $(JUCE_OBJDIR)/MainComponent_a6ffb4a5.o \
  $(JUCE_OBJDIR)/MainComponentCommands_b1179f4d.o \
  $(JUCE_OBJDIR)/CrashHandler_65ca18ac.o \
  $(JUCE_OBJDIR)/BinaryData_ce4232d4.o \
  $(JUCE_OBJDIR)/include_juce_analytics_f8e9fa94.o \
  $(JUCE_OBJDIR)/include_juce_audio_basics_8a4e984a.o \
  $(JUCE_OBJDIR)/include_juce_audio_devices_63111d02.o \
  $(JUCE_OBJDIR)/include_juce_audio_formats_15f82001.o \
  $(JUCE_OBJDIR)/include_juce_audio_processors_10c03666.o \
  $(JUCE_OBJDIR)/include_juce_audio_utils_9f9fb2d6.o \
  $(JUCE_OBJDIR)/include_juce_core_f26d17db.o \
  $(JUCE_OBJDIR)/include_juce_cryptography_8cb807a8.o \
  $(JUCE_OBJDIR)/include_juce_data_structures_7471b1e3.o \
  $(JUCE_OBJDIR)/include_juce_events_fd7d695.o \
  $(JUCE_OBJDIR)/include_juce_graphics_f817e147.o \
  $(JUCE_OBJDIR)/include_juce_gui_basics_e3f79785.o \
  $(JUCE_OBJDIR)/include_juce_gui_extra_6dee1c1a.o \
  $(JUCE_OBJDIR)/include_juce_opengl_a8a032b.o \
  $(JUCE_OBJDIR)/include_juce_organicui_2c36794d.o \
  $(JUCE_OBJDIR)/include_juce_osc_f3df604d.o \
  $(JUCE_OBJDIR)/include_juce_video_be78589.o \

.PHONY: clean all

all : $(JUCE_OUTDIR)/$(JUCE_TARGET_APP)

$(JUCE_OUTDIR)/$(JUCE_TARGET_APP) : check-pkg-config $(OBJECTS_APP) $(RESOURCES)
	@echo Linking "Chataigne - App"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_APP) $(OBJECTS_APP) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_APP) $(RESOURCES) $(TARGET_ARCH)

$(JUCE_OBJDIR)/CommandTemplateEditor_5399b8ad.o: ../../Source/Common/Command/Template/ui/CommandTemplateEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CommandTemplateEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CommandTemplateManagerUI_dc305a4b.o: ../../Source/Common/Command/Template/ui/CommandTemplateManagerUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CommandTemplateManagerUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CommandTemplateParameterEditor_a7196f80.o: ../../Source/Common/Command/Template/ui/CommandTemplateParameterEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CommandTemplateParameterEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CommandTemplate_5181ffb9.o: ../../Source/Common/Command/Template/CommandTemplate.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CommandTemplate.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CommandTemplateFactory_66cf2d1b.o: ../../Source/Common/Command/Template/CommandTemplateFactory.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CommandTemplateFactory.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CommandTemplateManager_6487023e.o: ../../Source/Common/Command/Template/CommandTemplateManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CommandTemplateManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CommandTemplateParameter_bfe2213a.o: ../../Source/Common/Command/Template/CommandTemplateParameter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CommandTemplateParameter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BaseCommand_2318dc9b.o: ../../Source/Common/Command/BaseCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BaseCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BaseCommandHandler_4d357439.o: ../../Source/Common/Command/BaseCommandHandler.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BaseCommandHandler.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CommandDefinition_7c3c255f.o: ../../Source/Common/Command/CommandDefinition.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CommandDefinition.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CommandDefinitionManager_7c0c55d8.o: ../../Source/Common/Command/CommandDefinitionManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CommandDefinitionManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CommandFactory_3a72e748.o: ../../Source/Common/Command/CommandFactory.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CommandFactory.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BaseCommandHandlerEditor_d6f421f9.o: ../../Source/Common/Command/ui/BaseCommandHandlerEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BaseCommandHandlerEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CommandChooserUI_dcc0e740.o: ../../Source/Common/Command/ui/CommandChooserUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CommandChooserUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DMXArtNetDevice_5a61b42b.o: ../../Source/Common/DMX/device/DMXArtNetDevice.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DMXArtNetDevice.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DMXDevice_462f8cd1.o: ../../Source/Common/DMX/device/DMXDevice.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DMXDevice.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DMXEnttecProDevice_e054588b.o: ../../Source/Common/DMX/device/DMXEnttecProDevice.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DMXEnttecProDevice.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DMXOpenUSBDevice_ad4fdeff.o: ../../Source/Common/DMX/device/DMXOpenUSBDevice.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DMXOpenUSBDevice.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DMXSerialDevice_44a8d965.o: ../../Source/Common/DMX/device/DMXSerialDevice.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DMXSerialDevice.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DMXDashboard_2d5d2a45.o: ../../Source/Common/DMX/ui/DMXDashboard.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DMXDashboard.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DMXStateManagerUI_7bd1c2a9.o: ../../Source/Common/DMX/ui/DMXStateManagerUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DMXStateManagerUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DMXStateUI_a98d2d16.o: ../../Source/Common/DMX/ui/DMXStateUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DMXStateUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DMXManager_b583f7e3.o: ../../Source/Common/DMX/DMXManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DMXManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DMXState_b401a707.o: ../../Source/Common/DMX/DMXState.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DMXState.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DMXStateManager_90092730.o: ../../Source/Common/DMX/DMXStateManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DMXStateManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MIDIDeviceChooser_959781bc.o: ../../Source/Common/MIDI/ui/MIDIDeviceChooser.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MIDIDeviceChooser.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MIDIDeviceParameterUI_bf16f57e.o: ../../Source/Common/MIDI/ui/MIDIDeviceParameterUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MIDIDeviceParameterUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MIDIDevice_3c053f6a.o: ../../Source/Common/MIDI/MIDIDevice.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MIDIDevice.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MIDIDeviceParameter_f25cf8a9.o: ../../Source/Common/MIDI/MIDIDeviceParameter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MIDIDeviceParameter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MIDIManager_a5993663.o: ../../Source/Common/MIDI/MIDIManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MIDIManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PluginManager_69ed7623.o: ../../Source/Common/Plugin/PluginManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PluginManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BoolComparators_9c68289d.o: ../../Source/Common/Processor/Action/Condition/Comparator/comparators/BoolComparators.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BoolComparators.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/EnumComparator_a53f6ef7.o: ../../Source/Common/Processor/Action/Condition/Comparator/comparators/EnumComparator.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling EnumComparator.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NumberComparators_25bd9d1e.o: ../../Source/Common/Processor/Action/Condition/Comparator/comparators/NumberComparators.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NumberComparators.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Point2DComparators_4dbb4bdb.o: ../../Source/Common/Processor/Action/Condition/Comparator/comparators/Point2DComparators.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Point2DComparators.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Point3DComparators_9e6529dc.o: ../../Source/Common/Processor/Action/Condition/Comparator/comparators/Point3DComparators.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Point3DComparators.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StringComparators_61def56.o: ../../Source/Common/Processor/Action/Condition/Comparator/comparators/StringComparators.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StringComparators.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BaseComparatorUI_a3309572.o: ../../Source/Common/Processor/Action/Condition/Comparator/ui/BaseComparatorUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BaseComparatorUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BaseComparator_c76f1d55.o: ../../Source/Common/Processor/Action/Condition/Comparator/BaseComparator.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BaseComparator.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ActivationCondition_54e1de99.o: ../../Source/Common/Processor/Action/Condition/conditions/ActivationCondition/ActivationCondition.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ActivationCondition.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConditionGroup_1310754d.o: ../../Source/Common/Processor/Action/Condition/conditions/ConditionGroup/ConditionGroup.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConditionGroup.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StandardConditionEditor_d7753ad.o: ../../Source/Common/Processor/Action/Condition/conditions/StandardCondition/ui/StandardConditionEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StandardConditionEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StandardCondition_9fb607cb.o: ../../Source/Common/Processor/Action/Condition/conditions/StandardCondition/StandardCondition.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StandardCondition.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScriptCondition_eadfe96f.o: ../../Source/Common/Processor/Action/Condition/conditions/ScriptCondition/ScriptCondition.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScriptCondition.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConditionEditor_15d65f42.o: ../../Source/Common/Processor/Action/Condition/ui/ConditionEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConditionEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConditionManagerEditor_94d9f8f.o: ../../Source/Common/Processor/Action/Condition/ui/ConditionManagerEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConditionManagerEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Condition_d04eabf0.o: ../../Source/Common/Processor/Action/Condition/Condition.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Condition.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConditionManager_83c17a27.o: ../../Source/Common/Processor/Action/Condition/ConditionManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConditionManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Consequence_a3851740.o: ../../Source/Common/Processor/Action/Consequence/Consequence.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Consequence.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConsequenceManager_5beab4d7.o: ../../Source/Common/Processor/Action/Consequence/ConsequenceManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConsequenceManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ActionUI_5ace508e.o: ../../Source/Common/Processor/Action/ui/ActionUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ActionUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Action_6a925b7.o: ../../Source/Common/Processor/Action/Action.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Action.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingFilter_806fb652.o: ../../Source/Common/Processor/Mapping/Filter/MappingFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingFilterManager_2c745d85.o: ../../Source/Common/Processor/Mapping/Filter/MappingFilterManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingFilterManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingFilterEditor_f7157696.o: ../../Source/Common/Processor/Mapping/Filter/ui/MappingFilterEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingFilterEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConversionFilters_b963b15d.o: ../../Source/Common/Processor/Mapping/Filter/filters/ConversionFilters.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConversionFilters.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CropFilter_481b2f1a.o: ../../Source/Common/Processor/Mapping/Filter/filters/CropFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CropFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CurveMapFilter_75770fb7.o: ../../Source/Common/Processor/Mapping/Filter/filters/CurveMapFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CurveMapFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InverseFilter_e1c4de20.o: ../../Source/Common/Processor/Mapping/Filter/filters/InverseFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InverseFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LagFilter_219d5b82.o: ../../Source/Common/Processor/Mapping/Filter/filters/LagFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LagFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OffsetFilter_3759d61d.o: ../../Source/Common/Processor/Mapping/Filter/filters/OffsetFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OffsetFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScriptFilter_49f85c55.o: ../../Source/Common/Processor/Mapping/Filter/filters/ScriptFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScriptFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SimpleRemapFilter_1d686067.o: ../../Source/Common/Processor/Mapping/Filter/filters/SimpleRemapFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SimpleRemapFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SimpleSmoothFilter_9930618a.o: ../../Source/Common/Processor/Mapping/Filter/filters/SimpleSmoothFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SimpleSmoothFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingInputEditor_3aad72e6.o: ../../Source/Common/Processor/Mapping/Input/ui/MappingInputEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingInputEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingInput_d2e57b44.o: ../../Source/Common/Processor/Mapping/Input/MappingInput.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingInput.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingOutputManagerEditor_c1da629b.o: ../../Source/Common/Processor/Mapping/Output/ui/MappingOutputManagerEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingOutputManagerEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingOutput_feb174a4.o: ../../Source/Common/Processor/Mapping/Output/MappingOutput.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingOutput.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingOutputManager_b8813ef3.o: ../../Source/Common/Processor/Mapping/Output/MappingOutputManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingOutputManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingUI_939edaca.o: ../../Source/Common/Processor/Mapping/ui/MappingUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Mapping_78a2fa15.o: ../../Source/Common/Processor/Mapping/Mapping.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Mapping.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ProcessorManagerUI_f8839312.o: ../../Source/Common/Processor/ui/ProcessorManagerUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ProcessorManagerUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ProcessorUI_b6bd10cd.o: ../../Source/Common/Processor/ui/ProcessorUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ProcessorUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Processor_e39625fa.o: ../../Source/Common/Processor/Processor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Processor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ProcessorManager_e0c880dd.o: ../../Source/Common/Processor/ProcessorManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ProcessorManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/cobs_e77e740f.o: ../../Source/Common/Serial/lib/cobs/cobs.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling cobs.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/serial_82f97e54.o: ../../Source/Common/Serial/lib/serial/serial.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling serial.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/list_ports_linux_203b2613.o: ../../Source/Common/Serial/lib/serial/impl/list_ports/list_ports_linux.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling list_ports_linux.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/list_ports_osx_d93ade33.o: ../../Source/Common/Serial/lib/serial/impl/list_ports/list_ports_osx.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling list_ports_osx.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/list_ports_win_e6502c6b.o: ../../Source/Common/Serial/lib/serial/impl/list_ports/list_ports_win.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling list_ports_win.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/unix_d5f5c79d.o: ../../Source/Common/Serial/lib/serial/impl/unix.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling unix.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/win_4c1bdbbb.o: ../../Source/Common/Serial/lib/serial/impl/win.cc
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling win.cc"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SerialDevice_e404848a.o: ../../Source/Common/Serial/SerialDevice.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SerialDevice.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SerialDeviceParameter_2de97789.o: ../../Source/Common/Serial/SerialDeviceParameter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SerialDeviceParameter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SerialManager_fd829543.o: ../../Source/Common/Serial/SerialManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SerialManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CVPresetEditor_83210cfc.o: ../../Source/CustomVariables/Preset/ui/CVPresetEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CVPresetEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CVPreset_3c4a8f1a.o: ../../Source/CustomVariables/Preset/CVPreset.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CVPreset.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CVPresetManager_6d2193bd.o: ../../Source/CustomVariables/Preset/CVPresetManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CVPresetManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CVGroupManagerUI_95d59108.o: ../../Source/CustomVariables/ui/CVGroupManagerUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CVGroupManagerUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CVGroup_5e5064a4.o: ../../Source/CustomVariables/CVGroup.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CVGroup.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CVGroupManager_45344ef3.o: ../../Source/CustomVariables/CVGroupManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CVGroupManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomVariablesModuleCommands_edf5eec8.o: ../../Source/Module/modules/customvariables/commands/CustomVariablesModuleCommands.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomVariablesModuleCommands.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomVariablesModule_cdc46d67.o: ../../Source/Module/modules/customvariables/CustomVariablesModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomVariablesModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioModuleHardwareEditor_e9c8a821.o: ../../Source/Module/modules/audio/ui/AudioModuleHardwareEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioModuleHardwareEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioModule_84057627.o: ../../Source/Module/modules/audio/AudioModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomValuesCommandArgumentEditor_1cc8b6de.o: ../../Source/Module/modules/common/commands/customvalues/ui/CustomValuesCommandArgumentEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomValuesCommandArgumentEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomValuesCommandArgumentManagerEditor_8192c073.o: ../../Source/Module/modules/common/commands/customvalues/ui/CustomValuesCommandArgumentManagerEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomValuesCommandArgumentManagerEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomValuesCommand_2f0f022b.o: ../../Source/Module/modules/common/commands/customvalues/CustomValuesCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomValuesCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomValuesCommandArgument_96a63888.o: ../../Source/Module/modules/common/commands/customvalues/CustomValuesCommandArgument.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomValuesCommandArgument.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomValuesCommandArgumentManager_fc7128f.o: ../../Source/Module/modules/common/commands/customvalues/CustomValuesCommandArgumentManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomValuesCommandArgumentManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SendStreamRawDataCommand_ca89c0e3.o: ../../Source/Module/modules/common/streaming/commands/SendStreamRawDataCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SendStreamRawDataCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SendStreamStringCommand_d6c0655a.o: ../../Source/Module/modules/common/streaming/commands/SendStreamStringCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SendStreamStringCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SendStreamStringValuesCommand_814cbb58.o: ../../Source/Module/modules/common/streaming/commands/SendStreamStringValuesCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SendStreamStringValuesCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SendStreamValuesCommand_7fe403a9.o: ../../Source/Module/modules/common/streaming/commands/SendStreamValuesCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SendStreamValuesCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StreamingCommand_19b7802b.o: ../../Source/Module/modules/common/streaming/commands/StreamingCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StreamingCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NetworkStreamingModule_a6ae39cf.o: ../../Source/Module/modules/common/streaming/NetworkStreamingModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NetworkStreamingModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StreamingModule_8e8f50a9.o: ../../Source/Module/modules/common/streaming/StreamingModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StreamingModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/EnablingNetworkControllableContainerEditor_5d799d9a.o: ../../Source/Module/modules/common/ui/EnablingNetworkControllableContainerEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling EnablingNetworkControllableContainerEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GamepadManager_dea1568b.o: ../../Source/Module/modules/controller/gamepad/GamepadManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GamepadManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GamepadModule_fc770758.o: ../../Source/Module/modules/controller/gamepad/GamepadModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GamepadModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/HIDModule_c3f9a29c.o: ../../Source/Module/modules/controller/hid/HIDModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling HIDModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/JoyConModule_1410c7f4.o: ../../Source/Module/modules/controller/joycon/JoyConModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JoyConModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/KeyboardModule_965e79f4.o: ../../Source/Module/modules/controller/keyboard/KeyboardModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling KeyboardModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/KinectV2Module_99832bf0.o: ../../Source/Module/modules/controller/kinect/KinectV2Module.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling KinectV2Module.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MyoDevice_cb8c0306.o: ../../Source/Module/modules/controller/myo/MyoDevice.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MyoDevice.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MyoManager_6eae647.o: ../../Source/Module/modules/controller/myo/MyoManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MyoManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MyoModule_691e901c.o: ../../Source/Module/modules/controller/myo/MyoModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MyoModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/os_mac_a23c8a30.o: ../../Source/Module/modules/controller/wiimote/lib/wiiuse/os_mac/os_mac.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling os_mac.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/classic_e456a893.o: ../../Source/Module/modules/controller/wiimote/lib/wiiuse/classic.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling classic.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/dynamics_a64483fd.o: ../../Source/Module/modules/controller/wiimote/lib/wiiuse/dynamics.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling dynamics.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/events_e669bcc2.o: ../../Source/Module/modules/controller/wiimote/lib/wiiuse/events.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling events.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/guitar_hero_3_a0003144.o: ../../Source/Module/modules/controller/wiimote/lib/wiiuse/guitar_hero_3.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling guitar_hero_3.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/io_7ae9338f.o: ../../Source/Module/modules/controller/wiimote/lib/wiiuse/io.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling io.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ir_7ae93ed2.o: ../../Source/Module/modules/controller/wiimote/lib/wiiuse/ir.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ir.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/motion_plus_4f38fa44.o: ../../Source/Module/modules/controller/wiimote/lib/wiiuse/motion_plus.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling motion_plus.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/nunchuk_7e0009c3.o: ../../Source/Module/modules/controller/wiimote/lib/wiiuse/nunchuk.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling nunchuk.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/os_nix_4c40caab.o: ../../Source/Module/modules/controller/wiimote/lib/wiiuse/os_nix.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling os_nix.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/os_win_4cbf78aa.o: ../../Source/Module/modules/controller/wiimote/lib/wiiuse/os_win.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling os_win.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/util_7a46226b.o: ../../Source/Module/modules/controller/wiimote/lib/wiiuse/util.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling util.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/wiiboard_b3f0e298.o: ../../Source/Module/modules/controller/wiimote/lib/wiiuse/wiiboard.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling wiiboard.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/wiiuse_8bc34039.o: ../../Source/Module/modules/controller/wiimote/lib/wiiuse/wiiuse.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling wiiuse.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WiimoteManager_c0441739.o: ../../Source/Module/modules/controller/wiimote/WiimoteManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WiimoteManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WiimoteModule_144278ea.o: ../../Source/Module/modules/controller/wiimote/WiimoteModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WiimoteModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WiimotePairUtil_320e063a.o: ../../Source/Module/modules/controller/wiimote/WiimotePairUtil.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WiimotePairUtil.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DMXCommand_23f829a1.o: ../../Source/Module/modules/dmx/commands/DMXCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DMXCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DMXModule_c47853d9.o: ../../Source/Module/modules/dmx/DMXModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DMXModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MetronomeModule_6453731e.o: ../../Source/Module/modules/generators/metronome/MetronomeModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MetronomeModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SignalModule_9a14db38.o: ../../Source/Module/modules/generators/signal/SignalModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SignalModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ChataigneLogCommand_8373a622.o: ../../Source/Module/modules/generic/commands/ChataigneLogCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ChataigneLogCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GenericControllableCommand_caf6da58.o: ../../Source/Module/modules/generic/commands/GenericControllableCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GenericControllableCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ChataigneGenericModule_d6802829.o: ../../Source/Module/modules/generic/ChataigneGenericModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ChataigneGenericModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MIDICommands_121216e.o: ../../Source/Module/modules/midi/commands/MIDICommands.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MIDICommands.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LaunchpadModule_654c7bb.o: ../../Source/Module/modules/midi/Launchpad/LaunchpadModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LaunchpadModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MIDIModule_d3422ef9.o: ../../Source/Module/modules/midi/MIDIModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MIDIModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OSCCommand_779cc3a1.o: ../../Source/Module/modules/osc/commands/OSCCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OSCCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/UserOSCCommandModelEditor_64a71933.o: ../../Source/Module/modules/osc/custom/commands/user/model/ui/UserOSCCommandModelEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling UserOSCCommandModelEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/UserOSCCommandModelManagerUI_3fe3e405.o: ../../Source/Module/modules/osc/custom/commands/user/model/ui/UserOSCCommandModelManagerUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling UserOSCCommandModelManagerUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/UserOSCCommandModelUI_66d9b3a.o: ../../Source/Module/modules/osc/custom/commands/user/model/ui/UserOSCCommandModelUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling UserOSCCommandModelUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/UserOSCCommandModel_399f4bed.o: ../../Source/Module/modules/osc/custom/commands/user/model/UserOSCCommandModel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling UserOSCCommandModel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/UserOSCCommandModelManager_84d9538a.o: ../../Source/Module/modules/osc/custom/commands/user/model/UserOSCCommandModelManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling UserOSCCommandModelManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/UserOSCCommandEditor_26442000.o: ../../Source/Module/modules/osc/custom/commands/user/ui/UserOSCCommandEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling UserOSCCommandEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/UserOSCCommand_7c414100.o: ../../Source/Module/modules/osc/custom/commands/user/UserOSCCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling UserOSCCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomOSCCommand_61f60882.o: ../../Source/Module/modules/osc/custom/commands/CustomOSCCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomOSCCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomOSCModule_1d5ea018.o: ../../Source/Module/modules/osc/custom/CustomOSCModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomOSCModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DLightModule_37bf0649.o: ../../Source/Module/modules/osc/dlight/DLightModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DLightModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LiveOSCCommandBase_2512fc69.o: ../../Source/Module/modules/osc/live/commands/LiveOSCCommandBase.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LiveOSCCommandBase.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LiveOSCSceneCommand_fb53785c.o: ../../Source/Module/modules/osc/live/commands/LiveOSCSceneCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LiveOSCSceneCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LiveOSCModule_ae0bbb38.o: ../../Source/Module/modules/osc/live/LiveOSCModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LiveOSCModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MilluminModule_e03eaa89.o: ../../Source/Module/modules/osc/millumin/MilluminModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MilluminModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ReaperTimeCommandUI_18f099cb.o: ../../Source/Module/modules/osc/reaper/commands/ui/ReaperTimeCommandUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ReaperTimeCommandUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ReaperTimeCommand_2fc352d6.o: ../../Source/Module/modules/osc/reaper/commands/ReaperTimeCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ReaperTimeCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ReaperModule_95c95bc9.o: ../../Source/Module/modules/osc/reaper/ReaperModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ReaperModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ResolumeBaseCommandEditor_c6593aa0.o: ../../Source/Module/modules/osc/resolume/commands/ui/ResolumeBaseCommandEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ResolumeBaseCommandEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ResolumeBaseCommand_9ab5c8d0.o: ../../Source/Module/modules/osc/resolume/commands/ResolumeBaseCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ResolumeBaseCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ResolumeClipCommand_ac9cb6f1.o: ../../Source/Module/modules/osc/resolume/commands/ResolumeClipCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ResolumeClipCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ResolumeFXCommand_82bf808f.o: ../../Source/Module/modules/osc/resolume/commands/ResolumeFXCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ResolumeFXCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ResolumeModule_61b1e469.o: ../../Source/Module/modules/osc/resolume/ResolumeModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ResolumeModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OSCModule_5d38d7b9.o: ../../Source/Module/modules/osc/OSCModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OSCModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceCommand_5137c44f.o: ../../Source/Module/modules/sequence/commands/SequenceCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceModule_6db3e719.o: ../../Source/Module/modules/sequence/SequenceModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SerialModule_a9d8bfb9.o: ../../Source/Module/modules/serial/SerialModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SerialModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StateCommand_1e7fa1.o: ../../Source/Module/modules/state/commands/StateCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StateModule_898183dd.o: ../../Source/Module/modules/state/StateModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OSExecCommand_1adb1a06.o: ../../Source/Module/modules/system/os/commands/OSExecCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OSExecCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OSPowerCommand_8563ee44.o: ../../Source/Module/modules/system/os/commands/OSPowerCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OSPowerCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WakeOnLanCommand_f0bfe78f.o: ../../Source/Module/modules/system/os/commands/WakeOnLanCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WakeOnLanCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OSModule_9cf844c7.o: ../../Source/Module/modules/system/os/OSModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OSModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeModule_26aecc47.o: ../../Source/Module/modules/system/time/TimeModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PJLinkModule_528c0f0b.o: ../../Source/Module/modules/tcp/pjlink/PJLinkModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PJLinkModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/VLCModule_e43b1167.o: ../../Source/Module/modules/tcp/vlc/VLCModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling VLCModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WatchoutCommand_1abb5d65.o: ../../Source/Module/modules/tcp/watchout/commands/WatchoutCommand.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WatchoutCommand.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WatchoutModule_630ef32b.o: ../../Source/Module/modules/tcp/watchout/WatchoutModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WatchoutModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TCPModule_24a2937d.o: ../../Source/Module/modules/tcp/TCPModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TCPModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/UDPModule_4d12fb3d.o: ../../Source/Module/modules/udp/UDPModule.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling UDPModule.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModuleRouterPanelView_3de9a86.o: ../../Source/Module/Routing/ui/ModuleRouterPanelView.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModuleRouterPanelView.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModuleRouterValueEditor_7fd5857b.o: ../../Source/Module/Routing/ui/ModuleRouterValueEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModuleRouterValueEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModuleRouterView_be67d052.o: ../../Source/Module/Routing/ui/ModuleRouterView.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModuleRouterView.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModuleRouter_b990f562.o: ../../Source/Module/Routing/ModuleRouter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModuleRouter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModuleRouterManager_84510c75.o: ../../Source/Module/Routing/ModuleRouterManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModuleRouterManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModuleRouterValue_107f14f9.o: ../../Source/Module/Routing/ModuleRouterValue.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModuleRouterValue.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModuleChooserUI_a3310ecc.o: ../../Source/Module/ui/ModuleChooserUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModuleChooserUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModuleEditor_e010c1a.o: ../../Source/Module/ui/ModuleEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModuleEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModuleManagerUI_da55b3be.o: ../../Source/Module/ui/ModuleManagerUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModuleManagerUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModuleUI_510b19a1.o: ../../Source/Module/ui/ModuleUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModuleUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Module_14b9fb70.o: ../../Source/Module/Module.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Module.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModuleFactory_f0ade584.o: ../../Source/Module/ModuleFactory.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModuleFactory.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModuleManager_ee65baa7.o: ../../Source/Module/ModuleManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModuleManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StateUI_cf33c402.o: ../../Source/StateMachine/State/ui/StateUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StateViewUI_87e08687.o: ../../Source/StateMachine/State/ui/StateViewUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateViewUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/State_6ea1d8c3.o: ../../Source/StateMachine/State/State.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling State.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StateTransitionManagerUI_2535d416.o: ../../Source/StateMachine/Transition/ui/StateTransitionManagerUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateTransitionManagerUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StateTransitionUI_2782c49.o: ../../Source/StateMachine/Transition/ui/StateTransitionUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateTransitionUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StateTransition_772f2d26.o: ../../Source/StateMachine/Transition/StateTransition.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateTransition.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StateTransitionManager_c1fd5031.o: ../../Source/StateMachine/Transition/StateTransitionManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateTransitionManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StateMachineView_e5e08766.o: ../../Source/StateMachine/ui/StateMachineView.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateMachineView.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StateManagerUI_a2d9b59b.o: ../../Source/StateMachine/ui/StateManagerUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateManagerUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StateManager_ec4bbd6.o: ../../Source/StateMachine/StateManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeCueManagerUI_b2d29c4a.o: ../../Source/TimeMachine/Sequence/Cue/ui/TimeCueManagerUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeCueManagerUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeCueUI_c10d7695.o: ../../Source/TimeMachine/Sequence/Cue/ui/TimeCueUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeCueUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeCue_a13ab31a.o: ../../Source/TimeMachine/Sequence/Cue/TimeCue.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeCue.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeCueManager_4480efbd.o: ../../Source/TimeMachine/Sequence/Cue/TimeCueManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeCueManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioLayerClipManagerUI_9973a193.o: ../../Source/TimeMachine/Sequence/Layer/layers/audio/ui/AudioLayerClipManagerUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioLayerClipManagerUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioLayerClipUI_e31de16c.o: ../../Source/TimeMachine/Sequence/Layer/layers/audio/ui/AudioLayerClipUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioLayerClipUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioLayerPanel_51233d86.o: ../../Source/TimeMachine/Sequence/Layer/layers/audio/ui/AudioLayerPanel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioLayerPanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioLayerTimeline_28b4bc89.o: ../../Source/TimeMachine/Sequence/Layer/layers/audio/ui/AudioLayerTimeline.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioLayerTimeline.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioLayer_c91e7f33.o: ../../Source/TimeMachine/Sequence/Layer/layers/audio/AudioLayer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioLayer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioLayerClip_604aefa3.o: ../../Source/TimeMachine/Sequence/Layer/layers/audio/AudioLayerClip.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioLayerClip.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioLayerClipManager_d1763714.o: ../../Source/TimeMachine/Sequence/Layer/layers/audio/AudioLayerClipManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioLayerClipManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeColorManagerUI_d8674ab7.o: ../../Source/TimeMachine/Sequence/Layer/layers/mapping/timecolor/ui/TimeColorManagerUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeColorManagerUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeColorUI_4c8e78c8.o: ../../Source/TimeMachine/Sequence/Layer/layers/mapping/timecolor/ui/TimeColorUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeColorUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeColor_f2e47aa7.o: ../../Source/TimeMachine/Sequence/Layer/layers/mapping/timecolor/TimeColor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeColor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeColorManager_b39eff90.o: ../../Source/TimeMachine/Sequence/Layer/layers/mapping/timecolor/TimeColorManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeColorManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingLayerEditor_949073f5.o: ../../Source/TimeMachine/Sequence/Layer/layers/mapping/ui/MappingLayerEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingLayerEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingLayerPanel_f16bd606.o: ../../Source/TimeMachine/Sequence/Layer/layers/mapping/ui/MappingLayerPanel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingLayerPanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingLayerTimeline_88bf5409.o: ../../Source/TimeMachine/Sequence/Layer/layers/mapping/ui/MappingLayerTimeline.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingLayerTimeline.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingLayer_461e6c03.o: ../../Source/TimeMachine/Sequence/Layer/layers/mapping/MappingLayer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingLayer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeTriggerManagerUI_3a8b4b85.o: ../../Source/TimeMachine/Sequence/Layer/layers/trigger/ui/TimeTriggerManagerUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeTriggerManagerUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeTriggerMultiTransformer_e11cdfa6.o: ../../Source/TimeMachine/Sequence/Layer/layers/trigger/ui/TimeTriggerMultiTransformer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeTriggerMultiTransformer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeTriggerUI_1103a3ba.o: ../../Source/TimeMachine/Sequence/Layer/layers/trigger/ui/TimeTriggerUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeTriggerUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TriggerLayerPanel_42a36fc6.o: ../../Source/TimeMachine/Sequence/Layer/layers/trigger/ui/TriggerLayerPanel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TriggerLayerPanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TriggerLayerTimeline_de0c6249.o: ../../Source/TimeMachine/Sequence/Layer/layers/trigger/ui/TriggerLayerTimeline.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TriggerLayerTimeline.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeTrigger_4aecc13f.o: ../../Source/TimeMachine/Sequence/Layer/layers/trigger/TimeTrigger.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeTrigger.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeTriggerManager_28e9df8.o: ../../Source/TimeMachine/Sequence/Layer/layers/trigger/TimeTriggerManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeTriggerManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TriggerLayer_270d0aaf.o: ../../Source/TimeMachine/Sequence/Layer/layers/trigger/TriggerLayer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TriggerLayer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceLayerPanel_b503e081.o: ../../Source/TimeMachine/Sequence/Layer/ui/SequenceLayerPanel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceLayerPanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceLayerPanelManagerUI_b9ca64ca.o: ../../Source/TimeMachine/Sequence/Layer/ui/SequenceLayerPanelManagerUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceLayerPanelManagerUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceLayerTimeline_2eaf3ae.o: ../../Source/TimeMachine/Sequence/Layer/ui/SequenceLayerTimeline.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceLayerTimeline.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceLayerTimelineManagerUI_8411943d.o: ../../Source/TimeMachine/Sequence/Layer/ui/SequenceLayerTimelineManagerUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceLayerTimelineManagerUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceLayer_c9c02da2.o: ../../Source/TimeMachine/Sequence/Layer/SequenceLayer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceLayer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceLayerFactory_eda63712.o: ../../Source/TimeMachine/Sequence/Layer/SequenceLayerFactory.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceLayerFactory.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceLayerManager_eb5e0c35.o: ../../Source/TimeMachine/Sequence/Layer/SequenceLayerManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceLayerManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceEditor_88b31cd9.o: ../../Source/TimeMachine/Sequence/ui/SequenceEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceTimelineHeader_e6ae3a1a.o: ../../Source/TimeMachine/Sequence/ui/SequenceTimelineHeader.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceTimelineHeader.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceTimelineNavigationUI_7c174fb5.o: ../../Source/TimeMachine/Sequence/ui/SequenceTimelineNavigationUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceTimelineNavigationUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceTimelineSeeker_77a668b2.o: ../../Source/TimeMachine/Sequence/ui/SequenceTimelineSeeker.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceTimelineSeeker.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceTransportUI_f5171adb.o: ../../Source/TimeMachine/Sequence/ui/SequenceTransportUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceTransportUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceUI_d0a961e0.o: ../../Source/TimeMachine/Sequence/ui/SequenceUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Sequence_b81dca9b.o: ../../Source/TimeMachine/Sequence/Sequence.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Sequence.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceManagerUI_68ff9d23.o: ../../Source/TimeMachine/ui/SequenceManagerUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceManagerUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeMachineView_84e65f62.o: ../../Source/TimeMachine/ui/TimeMachineView.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeMachineView.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceManager_b4b96c18.o: ../../Source/TimeMachine/SequenceManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AboutWindow_970e3d2b.o: ../../Source/UI/AboutWindow.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AboutWindow.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ChataigneAssetManager_b7ab91a9.o: ../../Source/UI/ChataigneAssetManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ChataigneAssetManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ChataigneEngine_2d424321.o: ../../Source/ChataigneEngine.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ChataigneEngine.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Main.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainComponent_a6ffb4a5.o: ../../Source/MainComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainComponentCommands_b1179f4d.o: ../../Source/MainComponentCommands.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainComponentCommands.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CrashHandler_65ca18ac.o: ../../Source/CrashHandler.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CrashHandler.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_analytics_f8e9fa94.o: ../../JuceLibraryCode/include_juce_analytics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_analytics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_basics_8a4e984a.o: ../../JuceLibraryCode/include_juce_audio_basics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_devices_63111d02.o: ../../JuceLibraryCode/include_juce_audio_devices.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_devices.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_formats_15f82001.o: ../../JuceLibraryCode/include_juce_audio_formats.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_formats.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_processors_10c03666.o: ../../JuceLibraryCode/include_juce_audio_processors.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_processors.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_utils_9f9fb2d6.o: ../../JuceLibraryCode/include_juce_audio_utils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_audio_utils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_core_f26d17db.o: ../../JuceLibraryCode/include_juce_core.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_core.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_cryptography_8cb807a8.o: ../../JuceLibraryCode/include_juce_cryptography.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_cryptography.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_data_structures_7471b1e3.o: ../../JuceLibraryCode/include_juce_data_structures.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_data_structures.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_events_fd7d695.o: ../../JuceLibraryCode/include_juce_events.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_events.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_graphics_f817e147.o: ../../JuceLibraryCode/include_juce_graphics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_graphics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_basics_e3f79785.o: ../../JuceLibraryCode/include_juce_gui_basics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_gui_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_extra_6dee1c1a.o: ../../JuceLibraryCode/include_juce_gui_extra.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_gui_extra.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_opengl_a8a032b.o: ../../JuceLibraryCode/include_juce_opengl.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_opengl.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_organicui_2c36794d.o: ../../JuceLibraryCode/include_juce_organicui.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_organicui.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_osc_f3df604d.o: ../../JuceLibraryCode/include_juce_osc.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_osc.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_video_be78589.o: ../../JuceLibraryCode/include_juce_video.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_video.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

check-pkg-config:
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa freetype2 libcurl x11 xext xinerama

clean:
	@echo Cleaning Chataigne
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping Chataigne
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

-include $(OBJECTS_APP:%.o=%.d)
