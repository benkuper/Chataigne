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
    TARGET_ARCH := -march=armv7
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DDEBUG=1" "-D_DEBUG=1" "-DVST_LOGGING=0" "-DJUCER_LINUX_MAKE_9C1B7400=1" "-DJUCE_APP_VERSION=1.8.0b38" "-DJUCE_APP_VERSION_HEX=0x10800" $(shell pkg-config --cflags alsa freetype2 libcurl webkit2gtk-4.0 gtk+-x11-3.0) -pthread -I../../JuceLibraryCode -I../../JUCE/modules -I../../Modules -I../../External/wiiuse -I../../External/serial/include -I../../External/servus/include -I../../External/dnssd/include -I../../External/sdl/include -I/usr/include/freetype2 -I../../External/hidapi/include -I../../External/libusb/include/libusb-1.0 -I../../External/joycon/include -I../../Source $(CPPFLAGS)
  JUCE_CPPFLAGS_APP :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0"
  JUCE_TARGET_APP := Chataigne

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O3 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++17 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L../../External/servus/lib/raspberry -L../../External/sdl/lib/raspberry -L../../External/joycon/lib/raspberry -L/usr/lib/arm-linux-gnueabihf -L../../Modules/juce_simpleweb/libs/Linux/armv7 $(shell pkg-config --libs alsa freetype2 libcurl) -fvisibility=hidden -Wl,-rpath,"lib" -Wl,--as-needed -lrt -ldl -lpthread -lGL -lssl -lcrypto -lbluetooth -lServus -lcurl -lSDL2 -lusb-1.0 -lhidapi-hidraw -lpthread -lJoyShockLibrary $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=armv7
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DNDEBUG=1" "-DVST_LOGGING=0" "-DJUCER_LINUX_MAKE_9C1B7400=1" "-DJUCE_APP_VERSION=1.8.0b38" "-DJUCE_APP_VERSION_HEX=0x10800" $(shell pkg-config --cflags alsa freetype2 libcurl webkit2gtk-4.0 gtk+-x11-3.0) -pthread -I../../JuceLibraryCode -I../../JUCE/modules -I../../Modules -I../../External/wiiuse -I../../External/serial/include -I../../External/servus/include -I../../External/dnssd/include -I../../External/sdl/include -I/usr/include/freetype2 -I../../External/hidapi/include -I../../External/libusb/include/libusb-1.0 -I../../External/joycon/include -I../../Source $(CPPFLAGS)
  JUCE_CPPFLAGS_APP :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_RTAS=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0"
  JUCE_TARGET_APP := Chataigne

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -O3 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++17 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L../../External/servus/lib/raspberry -L../../External/sdl/lib/raspberry -L../../External/joycon/lib/raspberry -L/usr/lib/arm-linux-gnueabihf -L../../Modules/juce_simpleweb/libs/Linux/armv7 $(shell pkg-config --libs alsa freetype2 libcurl) -fvisibility=hidden -Wl,-rpath,"lib" -Wl,--as-needed -lrt -ldl -lpthread -lGL -lssl -lcrypto -lbluetooth -lServus -lcurl -lSDL2 -lusb-1.0 -lhidapi-hidraw -lpthread -lJoyShockLibrary $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS_APP := \
  $(JUCE_OBJDIR)/GuiderIncludes_fc55989b.o \
  $(JUCE_OBJDIR)/MatomoAnalytics_a3cfd84d.o \
  $(JUCE_OBJDIR)/CommonIncludes_97d07afb.o \
  $(JUCE_OBJDIR)/LinkableParameterEditor_ed91bbd8.o \
  $(JUCE_OBJDIR)/ParameterLink_e54bbf1c.o \
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
  $(JUCE_OBJDIR)/CommandTemplateManager_6487023e.o \
  $(JUCE_OBJDIR)/CommandTemplateParameter_bfe2213a.o \
  $(JUCE_OBJDIR)/BaseCommand_2318dc9b.o \
  $(JUCE_OBJDIR)/BaseCommandHandler_4d357439.o \
  $(JUCE_OBJDIR)/CommandDefinition_7c3c255f.o \
  $(JUCE_OBJDIR)/CommandDefinitionManager_7c0c55d8.o \
  $(JUCE_OBJDIR)/CommandFactory_3a72e748.o \
  $(JUCE_OBJDIR)/e131_c3d02d0c.o \
  $(JUCE_OBJDIR)/ProcessorIncludes_efd64865.o \
  $(JUCE_OBJDIR)/ProcessorIncludes2_b12da37.o \
  $(JUCE_OBJDIR)/serial_82f97e54.o \
  $(JUCE_OBJDIR)/list_ports_linux_203b2613.o \
  $(JUCE_OBJDIR)/list_ports_osx_d93ade33.o \
  $(JUCE_OBJDIR)/list_ports_win_e6502c6b.o \
  $(JUCE_OBJDIR)/unix_d5f5c79d.o \
  $(JUCE_OBJDIR)/win_4c1bdbbb.o \
  $(JUCE_OBJDIR)/CustomVariablesIncludes_d7a15e29.o \
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
  $(JUCE_OBJDIR)/StateMachineIncludes_87fb5fdb.o \
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
  $(JUCE_OBJDIR)/include_juce_organicui2b_ff2481bd.o \
  $(JUCE_OBJDIR)/include_juce_organicui3_5ac6ddd0.o \
  $(JUCE_OBJDIR)/include_juce_organicui4_5ad4f551.o \
  $(JUCE_OBJDIR)/include_juce_osc_f3df604d.o \
  $(JUCE_OBJDIR)/include_juce_simpleweb_acec7570.o \
  $(JUCE_OBJDIR)/include_juce_simpleweb2_f0c24e8c.o \
  $(JUCE_OBJDIR)/include_juce_timeline_e6183ffd.o \

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

$(JUCE_OBJDIR)/e131_c3d02d0c.o: ../../Source/Common/DMX/device/sacn/e131.c
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling e131.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ProcessorIncludes_efd64865.o: ../../Source/Common/Processor/ProcessorIncludes.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ProcessorIncludes.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ProcessorIncludes2_b12da37.o: ../../Source/Common/Processor/ProcessorIncludes2.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ProcessorIncludes2.cpp"
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

$(JUCE_OBJDIR)/CustomVariablesIncludes_d7a15e29.o: ../../Source/CustomVariables/CustomVariablesIncludes.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomVariablesIncludes.cpp"
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

$(JUCE_OBJDIR)/StateMachineIncludes_87fb5fdb.o: ../../Source/StateMachine/StateMachineIncludes.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateMachineIncludes.cpp"
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

$(JUCE_OBJDIR)/include_juce_organicui2b_ff2481bd.o: ../../JuceLibraryCode/include_juce_organicui2b.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_organicui2b.cpp"
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

$(JUCE_OBJDIR)/include_juce_simpleweb2_f0c24e8c.o: ../../JuceLibraryCode/include_juce_simpleweb2.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_simpleweb2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_timeline_e6183ffd.o: ../../JuceLibraryCode/include_juce_timeline.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_timeline.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

clean:
	@echo Cleaning Chataigne
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping Chataigne
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

-include $(OBJECTS_APP:%.o=%.d)
