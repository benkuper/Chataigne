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
  CONFIG=Debug
endif

JUCE_ARCH_LABEL := $(shell uname -m)

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -m64
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DDEBUG=1" "-D_DEBUG=1" "-DVST_LOGGING=0" "-DJUCER_LINUX_MAKE_6D53C8B4=1" "-DJUCE_APP_VERSION=1.8.0b17" "-DJUCE_APP_VERSION_HEX=0x10800" $(shell pkg-config --cflags alsa freetype2 libcurl webkit2gtk-4.0 gtk+-x11-3.0) -pthread -I../../JuceLibraryCode -I../../JUCE/modules -I../../Modules -I../../External/wiiuse -I../../External/serial/include -I../../External/servus/include -I../../External/dnssd/include -I../../External/sdl/include -I../../External/hidapi/include -I../../External/libusb/include/libusb-1.0 -I../../Source $(CPPFLAGS)
  JUCE_CPPFLAGS_APP :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0"
  JUCE_TARGET_APP := Chataigne

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O3 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++17 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L../../External/servus/lib/linux -L../../External/sdl/lib/linux -L/usr/lib/x86_64-linux-gnu/ $(shell pkg-config --libs alsa freetype2 libcurl) -fvisibility=hidden -Wl,-rpath,"lib" -Wl,--as-needed -lrt -ldl -lpthread -lGL -lbluetooth -lServus -lcurl -lSDL2 -lusb-1.0 -lhidapi-hidraw $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -m64
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DNDEBUG=1" "-DVST_LOGGING=0" "-DJUCER_LINUX_MAKE_6D53C8B4=1" "-DJUCE_APP_VERSION=1.8.0b17" "-DJUCE_APP_VERSION_HEX=0x10800" $(shell pkg-config --cflags alsa freetype2 libcurl webkit2gtk-4.0 gtk+-x11-3.0) -pthread -I../../JuceLibraryCode -I../../JUCE/modules -I../../Modules -I../../External/wiiuse -I../../External/serial/include -I../../External/servus/include -I../../External/dnssd/include -I../../External/sdl/include -I../../External/hidapi/include -I../../External/libusb/include/libusb-1.0 -I../../Source $(CPPFLAGS)
  JUCE_CPPFLAGS_APP :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0"
  JUCE_TARGET_APP := Chataigne

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -O3 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++17 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L../../External/servus/lib/linux -L../../External/sdl/lib/linux -L/usr/lib/x86_64-linux-gnu/ $(shell pkg-config --libs alsa freetype2 libcurl) -fvisibility=hidden -Wl,-rpath,"lib" -Wl,--as-needed -lrt -ldl -lpthread -lGL -lbluetooth -lServus -lcurl -lSDL2 -lusb-1.0 -lhidapi-hidraw $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS_APP := \
  $(JUCE_OBJDIR)/GuiderIncludes_fc55989b.o \
  $(JUCE_OBJDIR)/MatomoAnalytics_a3cfd84d.o \
  $(JUCE_OBJDIR)/CommonIncludes_97d07afb.o \
  $(JUCE_OBJDIR)/LinkableParameterEditor_ed91bbd8.o \
  $(JUCE_OBJDIR)/ParameterLink_e54bbf1c.o \
  $(JUCE_OBJDIR)/ZeroconfManager_68129d03.o \
  $(JUCE_OBJDIR)/InputDeviceHelpers_f77a7a0.o \
  $(JUCE_OBJDIR)/InputSystemManager_a791f74f.o \
  $(JUCE_OBJDIR)/CustomValuesCommandArgumentEditor_97209c24.o \
  $(JUCE_OBJDIR)/CustomValuesCommandArgumentManagerEditor_dc11806d.o \
  $(JUCE_OBJDIR)/CustomValuesCommandArgument_cf882c82.o \
  $(JUCE_OBJDIR)/CustomValuesCommandArgumentManager_77439155.o \
  $(JUCE_OBJDIR)/BaseCommandEditor_eeffa8d5.o \
  $(JUCE_OBJDIR)/BaseCommandHandlerEditor_d6f421f9.o \
  $(JUCE_OBJDIR)/CommandChooserUI_dcc0e740.o \
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
  $(JUCE_OBJDIR)/DMXArtNetDevice_5a61b42b.o \
  $(JUCE_OBJDIR)/DMXDevice_462f8cd1.o \
  $(JUCE_OBJDIR)/DMXEnttecProDevice_e054588b.o \
  $(JUCE_OBJDIR)/DMXOpenUSBDevice_ad4fdeff.o \
  $(JUCE_OBJDIR)/DMXSerialDevice_44a8d965.o \
  $(JUCE_OBJDIR)/DMXManager_b583f7e3.o \
  $(JUCE_OBJDIR)/MIDIDeviceChooser_959781bc.o \
  $(JUCE_OBJDIR)/MIDIDeviceParameterUI_bf16f57e.o \
  $(JUCE_OBJDIR)/MIDIDevice_3c053f6a.o \
  $(JUCE_OBJDIR)/MIDIDeviceParameter_f25cf8a9.o \
  $(JUCE_OBJDIR)/MIDIManager_a5993663.o \
  $(JUCE_OBJDIR)/MTCReceiver_b0ff4722.o \
  $(JUCE_OBJDIR)/MTCSender_7b4a4be8.o \
  $(JUCE_OBJDIR)/ActivationCondition_54e1de99.o \
  $(JUCE_OBJDIR)/ConditionGroup_1310754d.o \
  $(JUCE_OBJDIR)/BoolComparators_c6fc0805.o \
  $(JUCE_OBJDIR)/EnumComparator_227e028f.o \
  $(JUCE_OBJDIR)/NumberComparators_fad74286.o \
  $(JUCE_OBJDIR)/Point2DComparators_1bd65373.o \
  $(JUCE_OBJDIR)/Point3DComparators_6c803174.o \
  $(JUCE_OBJDIR)/StringComparators_db3794be.o \
  $(JUCE_OBJDIR)/BaseComparatorUI_2cae5cda.o \
  $(JUCE_OBJDIR)/BaseComparator_ebfc54ed.o \
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
  $(JUCE_OBJDIR)/ConditionFilter_63f5eed7.o \
  $(JUCE_OBJDIR)/CropFilter_98bb03c6.o \
  $(JUCE_OBJDIR)/CurveMapFilter_1384ae63.o \
  $(JUCE_OBJDIR)/DampingFilter_5e3ffd46.o \
  $(JUCE_OBJDIR)/InverseFilter_3972b9f4.o \
  $(JUCE_OBJDIR)/LagFilter_b09a4156.o \
  $(JUCE_OBJDIR)/MathFilter_14041a3e.o \
  $(JUCE_OBJDIR)/SimpleRemapFilter_f05c323b.o \
  $(JUCE_OBJDIR)/SimpleSmoothFilter_24b6ca36.o \
  $(JUCE_OBJDIR)/ConversionParamValueLink_c54f9565.o \
  $(JUCE_OBJDIR)/ConversionConnector_71e92e4d.o \
  $(JUCE_OBJDIR)/ConversionFilterEditor_b0e8298f.o \
  $(JUCE_OBJDIR)/ConversionParamValueLinkUI_5b19779c.o \
  $(JUCE_OBJDIR)/ConversionSourceParameterUI_48851562.o \
  $(JUCE_OBJDIR)/ConvertedParameterManagerEditor_9c713179.o \
  $(JUCE_OBJDIR)/ConvertedParameterEditor_8c18c098.o \
  $(JUCE_OBJDIR)/ConversionFilter_a4c97adf.o \
  $(JUCE_OBJDIR)/SimpleConversionFilters_e6d8fd2c.o \
  $(JUCE_OBJDIR)/ConvertedParameter_b1b41968.o \
  $(JUCE_OBJDIR)/ConvertedParameterManager_c86475af.o \
  $(JUCE_OBJDIR)/ColorShiftFilter_9fafa455.o \
  $(JUCE_OBJDIR)/ScriptFilter_49f85c55.o \
  $(JUCE_OBJDIR)/MappingInputEditor_3aad72e6.o \
  $(JUCE_OBJDIR)/MappingInput_d2e57b44.o \
  $(JUCE_OBJDIR)/MappingInputManager_b5968453.o \
  $(JUCE_OBJDIR)/MappingOutputManagerEditor_c1da629b.o \
  $(JUCE_OBJDIR)/MappingOutput_feb174a4.o \
  $(JUCE_OBJDIR)/MappingOutputManager_b8813ef3.o \
  $(JUCE_OBJDIR)/MappingUI_939edaca.o \
  $(JUCE_OBJDIR)/Mapping_78a2fa15.o \
  $(JUCE_OBJDIR)/MultiplexListEditor_e887104a.o \
  $(JUCE_OBJDIR)/MultiplexList_4959b89e.o \
  $(JUCE_OBJDIR)/MultiplexListManager_17d6db9.o \
  $(JUCE_OBJDIR)/MultiplexUI_6808bd0a.o \
  $(JUCE_OBJDIR)/Multiplex_737cd189.o \
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
  $(JUCE_OBJDIR)/CVPresetMorphUI_d35c56f1.o \
  $(JUCE_OBJDIR)/MorpherViewUI_10801555.o \
  $(JUCE_OBJDIR)/MorphTargetUI_9b510ad4.o \
  $(JUCE_OBJDIR)/Morpher_50d8fcbd.o \
  $(JUCE_OBJDIR)/MorphTarget_55e8c641.o \
  $(JUCE_OBJDIR)/CVPresetEditor_83210cfc.o \
  $(JUCE_OBJDIR)/CVPreset_3c4a8f1a.o \
  $(JUCE_OBJDIR)/CVPresetManager_6d2193bd.o \
  $(JUCE_OBJDIR)/CVGroupManagerUI_95d59108.o \
  $(JUCE_OBJDIR)/CustomVariablesIncludes_d7a15e29.o \
  $(JUCE_OBJDIR)/CVGroup_5e5064a4.o \
  $(JUCE_OBJDIR)/CVGroupManager_45344ef3.o \
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
  $(JUCE_OBJDIR)/command_20531fa2.o \
  $(JUCE_OBJDIR)/pigpio_c63733db.o \
  $(JUCE_OBJDIR)/ModuleIncludes_95e047db.o \
  $(JUCE_OBJDIR)/ModuleIncludes2_2648c981.o \
  $(JUCE_OBJDIR)/StateUI_cf33c402.o \
  $(JUCE_OBJDIR)/StateViewUI_87e08687.o \
  $(JUCE_OBJDIR)/State_6ea1d8c3.o \
  $(JUCE_OBJDIR)/StateTransitionManagerUI_2535d416.o \
  $(JUCE_OBJDIR)/StateTransitionUI_2782c49.o \
  $(JUCE_OBJDIR)/StateTransition_772f2d26.o \
  $(JUCE_OBJDIR)/StateTransitionManager_c1fd5031.o \
  $(JUCE_OBJDIR)/StateMachineView_e5e08766.o \
  $(JUCE_OBJDIR)/StateManagerUI_a2d9b59b.o \
  $(JUCE_OBJDIR)/StateMachineIncludes_87fb5fdb.o \
  $(JUCE_OBJDIR)/StateManager_ec4bbd6.o \
  $(JUCE_OBJDIR)/TimeMachineIncludes_8f797311.o \
  $(JUCE_OBJDIR)/AboutWindow_970e3d2b.o \
  $(JUCE_OBJDIR)/ChataigneAssetManager_b7ab91a9.o \
  $(JUCE_OBJDIR)/WelcomeScreen_e38192fc.o \
  $(JUCE_OBJDIR)/ChataigneEngine_2d424321.o \
  $(JUCE_OBJDIR)/Main_90ebc5c2.o \
  $(JUCE_OBJDIR)/MainComponent_a6ffb4a5.o \
  $(JUCE_OBJDIR)/MainComponentCommands_b1179f4d.o \
  $(JUCE_OBJDIR)/BinaryData_ce4232d4.o \
  $(JUCE_OBJDIR)/include_juce_audio_basics_8a4e984a.o \
  $(JUCE_OBJDIR)/include_juce_audio_devices_63111d02.o \
  $(JUCE_OBJDIR)/include_juce_audio_formats_15f82001.o \
  $(JUCE_OBJDIR)/include_juce_audio_processors_10c03666.o \
  $(JUCE_OBJDIR)/include_juce_audio_utils_9f9fb2d6.o \
  $(JUCE_OBJDIR)/include_juce_core_f26d17db.o \
  $(JUCE_OBJDIR)/include_juce_cryptography_8cb807a8.o \
  $(JUCE_OBJDIR)/include_juce_data_structures_7471b1e3.o \
  $(JUCE_OBJDIR)/include_juce_dsp_aeb2060f.o \
  $(JUCE_OBJDIR)/include_juce_events_fd7d695.o \
  $(JUCE_OBJDIR)/include_juce_graphics_f817e147.o \
  $(JUCE_OBJDIR)/include_juce_gui_basics_e3f79785.o \
  $(JUCE_OBJDIR)/include_juce_gui_extra_6dee1c1a.o \
  $(JUCE_OBJDIR)/include_juce_opengl_a8a032b.o \
  $(JUCE_OBJDIR)/include_juce_organicui_2c36794d.o \
  $(JUCE_OBJDIR)/include_juce_organicui2_5ab8c64f.o \
  $(JUCE_OBJDIR)/include_juce_organicui3_5ac6ddd0.o \
  $(JUCE_OBJDIR)/include_juce_organicui4_5ad4f551.o \
  $(JUCE_OBJDIR)/include_juce_osc_f3df604d.o \
  $(JUCE_OBJDIR)/include_juce_simpleweb_acec7570.o \
  $(JUCE_OBJDIR)/include_juce_timeline_e6183ffd.o \
  $(JUCE_OBJDIR)/include_juce_video_be78589.o \

.PHONY: clean all strip

all : $(JUCE_OUTDIR)/$(JUCE_TARGET_APP)

$(JUCE_OUTDIR)/$(JUCE_TARGET_APP) : $(OBJECTS_APP) $(RESOURCES)
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa freetype2 libcurl
	@echo Linking "Chataigne - App"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_APP) $(OBJECTS_APP) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_APP) $(RESOURCES) $(TARGET_ARCH)

$(JUCE_OBJDIR)/GuiderIncludes_fc55989b.o: ../../Source/Guider/GuiderIncludes.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GuiderIncludes.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MatomoAnalytics_a3cfd84d.o: ../../Source/Analytics/MatomoAnalytics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MatomoAnalytics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CommonIncludes_97d07afb.o: ../../Source/Common/CommonIncludes.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CommonIncludes.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LinkableParameterEditor_ed91bbd8.o: ../../Source/Common/ParameterLink/ui/LinkableParameterEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LinkableParameterEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ParameterLink_e54bbf1c.o: ../../Source/Common/ParameterLink/ParameterLink.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ParameterLink.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ZeroconfManager_68129d03.o: ../../Source/Common/Zeroconf/ZeroconfManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ZeroconfManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InputDeviceHelpers_f77a7a0.o: ../../Source/Common/InputSystem/InputDeviceHelpers.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InputDeviceHelpers.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InputSystemManager_a791f74f.o: ../../Source/Common/InputSystem/InputSystemManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InputSystemManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomValuesCommandArgumentEditor_97209c24.o: ../../Source/Common/Command/CustomValues/ui/CustomValuesCommandArgumentEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomValuesCommandArgumentEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomValuesCommandArgumentManagerEditor_dc11806d.o: ../../Source/Common/Command/CustomValues/ui/CustomValuesCommandArgumentManagerEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomValuesCommandArgumentManagerEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomValuesCommandArgument_cf882c82.o: ../../Source/Common/Command/CustomValues/CustomValuesCommandArgument.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomValuesCommandArgument.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomValuesCommandArgumentManager_77439155.o: ../../Source/Common/Command/CustomValues/CustomValuesCommandArgumentManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomValuesCommandArgumentManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BaseCommandEditor_eeffa8d5.o: ../../Source/Common/Command/ui/BaseCommandEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BaseCommandEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BaseCommandHandlerEditor_d6f421f9.o: ../../Source/Common/Command/ui/BaseCommandHandlerEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BaseCommandHandlerEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CommandChooserUI_dcc0e740.o: ../../Source/Common/Command/ui/CommandChooserUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CommandChooserUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

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

$(JUCE_OBJDIR)/DMXManager_b583f7e3.o: ../../Source/Common/DMX/DMXManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DMXManager.cpp"
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

$(JUCE_OBJDIR)/MTCReceiver_b0ff4722.o: ../../Source/Common/MIDI/MTCReceiver.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MTCReceiver.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MTCSender_7b4a4be8.o: ../../Source/Common/MIDI/MTCSender.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MTCSender.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ActivationCondition_54e1de99.o: ../../Source/Common/Processor/Action/Condition/conditions/ActivationCondition/ActivationCondition.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ActivationCondition.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConditionGroup_1310754d.o: ../../Source/Common/Processor/Action/Condition/conditions/ConditionGroup/ConditionGroup.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConditionGroup.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BoolComparators_c6fc0805.o: ../../Source/Common/Processor/Action/Condition/conditions/StandardCondition/Comparator/comparators/BoolComparators.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BoolComparators.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/EnumComparator_227e028f.o: ../../Source/Common/Processor/Action/Condition/conditions/StandardCondition/Comparator/comparators/EnumComparator.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling EnumComparator.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NumberComparators_fad74286.o: ../../Source/Common/Processor/Action/Condition/conditions/StandardCondition/Comparator/comparators/NumberComparators.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NumberComparators.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Point2DComparators_1bd65373.o: ../../Source/Common/Processor/Action/Condition/conditions/StandardCondition/Comparator/comparators/Point2DComparators.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Point2DComparators.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Point3DComparators_6c803174.o: ../../Source/Common/Processor/Action/Condition/conditions/StandardCondition/Comparator/comparators/Point3DComparators.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Point3DComparators.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StringComparators_db3794be.o: ../../Source/Common/Processor/Action/Condition/conditions/StandardCondition/Comparator/comparators/StringComparators.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StringComparators.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BaseComparatorUI_2cae5cda.o: ../../Source/Common/Processor/Action/Condition/conditions/StandardCondition/Comparator/ui/BaseComparatorUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BaseComparatorUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BaseComparator_ebfc54ed.o: ../../Source/Common/Processor/Action/Condition/conditions/StandardCondition/Comparator/BaseComparator.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BaseComparator.cpp"
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

$(JUCE_OBJDIR)/ConditionFilter_63f5eed7.o: ../../Source/Common/Processor/Mapping/Filter/filters/condition/ConditionFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConditionFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CropFilter_98bb03c6.o: ../../Source/Common/Processor/Mapping/Filter/filters/number/CropFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CropFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CurveMapFilter_1384ae63.o: ../../Source/Common/Processor/Mapping/Filter/filters/number/CurveMapFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CurveMapFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DampingFilter_5e3ffd46.o: ../../Source/Common/Processor/Mapping/Filter/filters/number/DampingFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DampingFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InverseFilter_3972b9f4.o: ../../Source/Common/Processor/Mapping/Filter/filters/number/InverseFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InverseFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LagFilter_b09a4156.o: ../../Source/Common/Processor/Mapping/Filter/filters/number/LagFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LagFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MathFilter_14041a3e.o: ../../Source/Common/Processor/Mapping/Filter/filters/number/MathFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MathFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SimpleRemapFilter_f05c323b.o: ../../Source/Common/Processor/Mapping/Filter/filters/number/SimpleRemapFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SimpleRemapFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SimpleSmoothFilter_24b6ca36.o: ../../Source/Common/Processor/Mapping/Filter/filters/number/SimpleSmoothFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SimpleSmoothFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConversionParamValueLink_c54f9565.o: ../../Source/Common/Processor/Mapping/Filter/filters/conversion/ConversionParamValueLink.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConversionParamValueLink.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConversionConnector_71e92e4d.o: ../../Source/Common/Processor/Mapping/Filter/filters/conversion/ui/ConversionConnector.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConversionConnector.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConversionFilterEditor_b0e8298f.o: ../../Source/Common/Processor/Mapping/Filter/filters/conversion/ui/ConversionFilterEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConversionFilterEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConversionParamValueLinkUI_5b19779c.o: ../../Source/Common/Processor/Mapping/Filter/filters/conversion/ui/ConversionParamValueLinkUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConversionParamValueLinkUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConversionSourceParameterUI_48851562.o: ../../Source/Common/Processor/Mapping/Filter/filters/conversion/ui/ConversionSourceParameterUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConversionSourceParameterUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConvertedParameterManagerEditor_9c713179.o: ../../Source/Common/Processor/Mapping/Filter/filters/conversion/ui/ConvertedParameterManagerEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConvertedParameterManagerEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConvertedParameterEditor_8c18c098.o: ../../Source/Common/Processor/Mapping/Filter/filters/conversion/ui/ConvertedParameterEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConvertedParameterEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConversionFilter_a4c97adf.o: ../../Source/Common/Processor/Mapping/Filter/filters/conversion/ConversionFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConversionFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SimpleConversionFilters_e6d8fd2c.o: ../../Source/Common/Processor/Mapping/Filter/filters/conversion/SimpleConversionFilters.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SimpleConversionFilters.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConvertedParameter_b1b41968.o: ../../Source/Common/Processor/Mapping/Filter/filters/conversion/ConvertedParameter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConvertedParameter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConvertedParameterManager_c86475af.o: ../../Source/Common/Processor/Mapping/Filter/filters/conversion/ConvertedParameterManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConvertedParameterManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ColorShiftFilter_9fafa455.o: ../../Source/Common/Processor/Mapping/Filter/filters/color/ColorShiftFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ColorShiftFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScriptFilter_49f85c55.o: ../../Source/Common/Processor/Mapping/Filter/filters/ScriptFilter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScriptFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingInputEditor_3aad72e6.o: ../../Source/Common/Processor/Mapping/Input/ui/MappingInputEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingInputEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingInput_d2e57b44.o: ../../Source/Common/Processor/Mapping/Input/MappingInput.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingInput.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingInputManager_b5968453.o: ../../Source/Common/Processor/Mapping/Input/MappingInputManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingInputManager.cpp"
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

$(JUCE_OBJDIR)/MultiplexListEditor_e887104a.o: ../../Source/Common/Processor/Multiplex/List/ui/MultiplexListEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MultiplexListEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MultiplexList_4959b89e.o: ../../Source/Common/Processor/Multiplex/List/MultiplexList.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MultiplexList.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MultiplexListManager_17d6db9.o: ../../Source/Common/Processor/Multiplex/List/MultiplexListManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MultiplexListManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MultiplexUI_6808bd0a.o: ../../Source/Common/Processor/Multiplex/ui/MultiplexUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MultiplexUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Multiplex_737cd189.o: ../../Source/Common/Processor/Multiplex/Multiplex.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Multiplex.cpp"
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

$(JUCE_OBJDIR)/CVPresetMorphUI_d35c56f1.o: ../../Source/CustomVariables/Preset/Morpher/ui/CVPresetMorphUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CVPresetMorphUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MorpherViewUI_10801555.o: ../../Source/CustomVariables/Preset/Morpher/ui/MorpherViewUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MorpherViewUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MorphTargetUI_9b510ad4.o: ../../Source/CustomVariables/Preset/Morpher/ui/MorphTargetUI.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MorphTargetUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Morpher_50d8fcbd.o: ../../Source/CustomVariables/Preset/Morpher/Morpher.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Morpher.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MorphTarget_55e8c641.o: ../../Source/CustomVariables/Preset/Morpher/MorphTarget.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MorphTarget.cpp"
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

$(JUCE_OBJDIR)/CustomVariablesIncludes_d7a15e29.o: ../../Source/CustomVariables/CustomVariablesIncludes.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomVariablesIncludes.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CVGroup_5e5064a4.o: ../../Source/CustomVariables/CVGroup.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CVGroup.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CVGroupManager_45344ef3.o: ../../Source/CustomVariables/CVGroupManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CVGroupManager.cpp"
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

$(JUCE_OBJDIR)/command_20531fa2.o: ../../Source/Module/modules/gpio/pigpio/command.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling command.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/pigpio_c63733db.o: ../../Source/Module/modules/gpio/pigpio/pigpio.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling pigpio.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModuleIncludes_95e047db.o: ../../Source/Module/ModuleIncludes.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModuleIncludes.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModuleIncludes2_2648c981.o: ../../Source/Module/ModuleIncludes2.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ModuleIncludes2.cpp"
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

$(JUCE_OBJDIR)/StateMachineIncludes_87fb5fdb.o: ../../Source/StateMachine/StateMachineIncludes.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateMachineIncludes.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StateManager_ec4bbd6.o: ../../Source/StateMachine/StateManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeMachineIncludes_8f797311.o: ../../Source/TimeMachine/TimeMachineIncludes.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeMachineIncludes.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AboutWindow_970e3d2b.o: ../../Source/UI/AboutWindow.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AboutWindow.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ChataigneAssetManager_b7ab91a9.o: ../../Source/UI/ChataigneAssetManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ChataigneAssetManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WelcomeScreen_e38192fc.o: ../../Source/UI/WelcomeScreen.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WelcomeScreen.cpp"
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

$(JUCE_OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BinaryData.cpp"
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

$(JUCE_OBJDIR)/include_juce_dsp_aeb2060f.o: ../../JuceLibraryCode/include_juce_dsp.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_dsp.cpp"
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

$(JUCE_OBJDIR)/include_juce_organicui2_5ab8c64f.o: ../../JuceLibraryCode/include_juce_organicui2.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_organicui2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_organicui3_5ac6ddd0.o: ../../JuceLibraryCode/include_juce_organicui3.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_organicui3.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_organicui4_5ad4f551.o: ../../JuceLibraryCode/include_juce_organicui4.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_organicui4.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_osc_f3df604d.o: ../../JuceLibraryCode/include_juce_osc.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_osc.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_simpleweb_acec7570.o: ../../JuceLibraryCode/include_juce_simpleweb.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_simpleweb.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_timeline_e6183ffd.o: ../../JuceLibraryCode/include_juce_timeline.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_timeline.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_video_be78589.o: ../../JuceLibraryCode/include_juce_video.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_video.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

clean:
	@echo Cleaning Chataigne
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping Chataigne
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

-include $(OBJECTS_APP:%.o=%.d)
