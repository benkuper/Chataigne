# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

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

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 $(shell pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I$(HOME)/JUCE/modules
  JUCE_CFLAGS += $(CFLAGS) $(JUCE_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  JUCE_CXXFLAGS += $(CXXFLAGS) $(JUCE_CFLAGS) -std=c++11
  JUCE_LDFLAGS += $(LDFLAGS) $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama) -lGL -ldl -lpthread -lrt 

  TARGET := Flap
  BLDCMD = $(CXX) -o $(JUCE_OUTDIR)/$(TARGET) $(OBJECTS) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 $(shell pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I$(HOME)/JUCE/modules
  JUCE_CFLAGS += $(CFLAGS) $(JUCE_CPPFLAGS) $(TARGET_ARCH) -O3
  JUCE_CXXFLAGS += $(CXXFLAGS) $(JUCE_CFLAGS) -std=c++11
  JUCE_LDFLAGS += $(LDFLAGS) $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -fvisibility=hidden $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama) -lGL -ldl -lpthread -lrt 

  TARGET := Flap
  BLDCMD = $(CXX) -o $(JUCE_OUTDIR)/$(TARGET) $(OBJECTS) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS := \
  $(JUCE_OBJDIR)/Outliner_324682f9.o \
  $(JUCE_OBJDIR)/ProgressNotifier_22cad7fc.o \
  $(JUCE_OBJDIR)/ProgressWindow_8ee8f986.o \
  $(JUCE_OBJDIR)/BoolParameter_60c297c0.o \
  $(JUCE_OBJDIR)/EnumParameter_6e309b09.o \
  $(JUCE_OBJDIR)/FloatParameter_67c285b6.o \
  $(JUCE_OBJDIR)/IntParameter_21cc1c03.o \
  $(JUCE_OBJDIR)/Parameter_73564c8a.o \
  $(JUCE_OBJDIR)/Point2DParameter_7a5034f0.o \
  $(JUCE_OBJDIR)/Point3DParameter_d42b9f31.o \
  $(JUCE_OBJDIR)/StringParameter_fe3ecef9.o \
  $(JUCE_OBJDIR)/TargetParameter_c17e2d79.o \
  $(JUCE_OBJDIR)/BetterStepper_8f8519a2.o \
  $(JUCE_OBJDIR)/BoolToggleUI_bb303d7b.o \
  $(JUCE_OBJDIR)/DoubleSliderUI_f128d6af.o \
  $(JUCE_OBJDIR)/EnumParameterUI_e576769d.o \
  $(JUCE_OBJDIR)/FloatSliderUI_d00e5892.o \
  $(JUCE_OBJDIR)/FloatStepperUI_bb9cf532.o \
  $(JUCE_OBJDIR)/IntSliderUI_a48244a5.o \
  $(JUCE_OBJDIR)/IntStepperUI_75a68b7f.o \
  $(JUCE_OBJDIR)/ParameterUI_37f5cbde.o \
  $(JUCE_OBJDIR)/StringParameterUI_aac76e8d.o \
  $(JUCE_OBJDIR)/TargetParameterUI_9ba92d0d.o \
  $(JUCE_OBJDIR)/TripleSliderUI_836dfbbc.o \
  $(JUCE_OBJDIR)/PresetChooser_96a32abd.o \
  $(JUCE_OBJDIR)/PresetManager_fc30642f.o \
  $(JUCE_OBJDIR)/GenericControllableContainerEditor_117ebfb7.o \
  $(JUCE_OBJDIR)/ControllableEditor_dd5c95df.o \
  $(JUCE_OBJDIR)/ControllableUI_c544de6.o \
  $(JUCE_OBJDIR)/TriggerBlinkUI_693175a1.o \
  $(JUCE_OBJDIR)/TriggerButtonUI_d1a8d47f.o \
  $(JUCE_OBJDIR)/TriggerUI_2ce661ad.o \
  $(JUCE_OBJDIR)/Controllable_d3fdb092.o \
  $(JUCE_OBJDIR)/ControllableContainer_f3a7c159.o \
  $(JUCE_OBJDIR)/ControllableContainerEditor_9832b566.o \
  $(JUCE_OBJDIR)/ControllableFactory_10ac8622.o \
  $(JUCE_OBJDIR)/ControllableHelpers_912c061d.o \
  $(JUCE_OBJDIR)/ControllableIndexedContainer_a91d6542.o \
  $(JUCE_OBJDIR)/Trigger_dbc00a19.o \
  $(JUCE_OBJDIR)/BaseCommand_529ec09b.o \
  $(JUCE_OBJDIR)/CommandFactory_88728348.o \
  $(JUCE_OBJDIR)/CommandChooserUI_aed1be8d.o \
  $(JUCE_OBJDIR)/BaseItem_6b33fe0d.o \
  $(JUCE_OBJDIR)/ConsequenceManagerUI_6e459487.o \
  $(JUCE_OBJDIR)/ConsequenceUI_adc498f8.o \
  $(JUCE_OBJDIR)/Consequence_78c99824.o \
  $(JUCE_OBJDIR)/ConsequenceManager_ed222b73.o \
  $(JUCE_OBJDIR)/BoolComparators_278bc7f4.o \
  $(JUCE_OBJDIR)/NumberComparators_7676c2b5.o \
  $(JUCE_OBJDIR)/Point2DComparators_1426d924.o \
  $(JUCE_OBJDIR)/Point3DComparators_64d0b725.o \
  $(JUCE_OBJDIR)/StringComparators_56d714ed.o \
  $(JUCE_OBJDIR)/BaseComparatorUI_36712464.o \
  $(JUCE_OBJDIR)/FloatComparatorUI_28ecccc7.o \
  $(JUCE_OBJDIR)/BaseComparator_fbba0e90.o \
  $(JUCE_OBJDIR)/ComparatorFactory_14bedeb5.o \
  $(JUCE_OBJDIR)/ConditionManagerUI_219422af.o \
  $(JUCE_OBJDIR)/ConditionUI_37f827d0.o \
  $(JUCE_OBJDIR)/Condition_dbc2fcfc.o \
  $(JUCE_OBJDIR)/ConditionManager_8b8e239b.o \
  $(JUCE_OBJDIR)/ActionEditor_5de4624c.o \
  $(JUCE_OBJDIR)/ActionManagerUI_82c1584c.o \
  $(JUCE_OBJDIR)/ActionUI_e36558d3.o \
  $(JUCE_OBJDIR)/Action_97be92bf.o \
  $(JUCE_OBJDIR)/ActionManager_9ed61c78.o \
  $(JUCE_OBJDIR)/MappingInputUI_8f4826b9.o \
  $(JUCE_OBJDIR)/MappingInput_19a7a225.o \
  $(JUCE_OBJDIR)/MappingOutputManagerUI_16c922db.o \
  $(JUCE_OBJDIR)/MappingOutputUI_6b97c124.o \
  $(JUCE_OBJDIR)/MappingOutput_7f60db50.o \
  $(JUCE_OBJDIR)/MappingOutputManager_681d3ec7.o \
  $(JUCE_OBJDIR)/MappingFilterManagerUI_447b69c4.o \
  $(JUCE_OBJDIR)/MappingFilterUI_f2877e5b.o \
  $(JUCE_OBJDIR)/MappingFilter_6ebc7a47.o \
  $(JUCE_OBJDIR)/MappingFilterManager_c0562bf0.o \
  $(JUCE_OBJDIR)/MappingManagerUI_a4d2f6dc.o \
  $(JUCE_OBJDIR)/MappingUI_56e9fc43.o \
  $(JUCE_OBJDIR)/Mapping_bdf5522f.o \
  $(JUCE_OBJDIR)/MappingManager_a2de1f08.o \
  $(JUCE_OBJDIR)/SequenceLayerFactory_ac14d863.o \
  $(JUCE_OBJDIR)/AudioLayerPanel_faa1fb8a.o \
  $(JUCE_OBJDIR)/AudioLayerTimeline_7cd71005.o \
  $(JUCE_OBJDIR)/AudioLayer_60c0cf24.o \
  $(JUCE_OBJDIR)/MappingLayerPanel_890eab82.o \
  $(JUCE_OBJDIR)/MappingLayerTimeline_96e8bd0d.o \
  $(JUCE_OBJDIR)/MappingLayer_50b4042c.o \
  $(JUCE_OBJDIR)/TriggerLayerPanel_72c06ec.o \
  $(JUCE_OBJDIR)/TriggerLayerTimeline_b139b163.o \
  $(JUCE_OBJDIR)/TimeTrigger_4a6ad70c.o \
  $(JUCE_OBJDIR)/TriggerLayer_1751ae82.o \
  $(JUCE_OBJDIR)/SequenceLayerTimelineManagerUI_a4660719.o \
  $(JUCE_OBJDIR)/SequenceLayerPanel_5928bd5d.o \
  $(JUCE_OBJDIR)/SequenceLayerTimeline_a0ac9952.o \
  $(JUCE_OBJDIR)/SequenceLayerPanelManagerUI_8a7a3f6e.o \
  $(JUCE_OBJDIR)/SequenceLayerManager_a9ccad86.o \
  $(JUCE_OBJDIR)/SequenceLayer_e986f571.o \
  $(JUCE_OBJDIR)/SequenceTimelineHeader_4a3b0458.o \
  $(JUCE_OBJDIR)/SequenceTimelineSeeker_db3332f0.o \
  $(JUCE_OBJDIR)/SequenceEditor_f0150517.o \
  $(JUCE_OBJDIR)/SequenceTimelineNavigationUI_8126f373.o \
  $(JUCE_OBJDIR)/SequenceTransportUI_5072a95d.o \
  $(JUCE_OBJDIR)/SequenceUI_6f55191e.o \
  $(JUCE_OBJDIR)/Sequence_b8a7e9ca.o \
  $(JUCE_OBJDIR)/SequenceManagerUI_cc91bda1.o \
  $(JUCE_OBJDIR)/TimeMachineView_76e28760.o \
  $(JUCE_OBJDIR)/SequenceManager_52b9d30d.o \
  $(JUCE_OBJDIR)/StateViewUI_7695054b.o \
  $(JUCE_OBJDIR)/StateUI_4c5e44c6.o \
  $(JUCE_OBJDIR)/State_21d7df72.o \
  $(JUCE_OBJDIR)/TransitionConnector_1e3508d9.o \
  $(JUCE_OBJDIR)/TransitionUI_3d0f2ab2.o \
  $(JUCE_OBJDIR)/Transition_3f21505e.o \
  $(JUCE_OBJDIR)/TransitionManager_82f9df9.o \
  $(JUCE_OBJDIR)/StateMachineView_e5001744.o \
  $(JUCE_OBJDIR)/StateManagerUI_e1b7a4f9.o \
  $(JUCE_OBJDIR)/StateManager_b261b065.o \
  $(JUCE_OBJDIR)/OutputCommand_7d3e356b.o \
  $(JUCE_OBJDIR)/OSCCommand_a034ed5.o \
  $(JUCE_OBJDIR)/OSCOutput_961b5a41.o \
  $(JUCE_OBJDIR)/BGloveOutput_fe42c581.o \
  $(JUCE_OBJDIR)/CustomOSCCommand_684cdda6.o \
  $(JUCE_OBJDIR)/CustomOSCOutput_b1ec2d50.o \
  $(JUCE_OBJDIR)/BentoOutput_52ac9bc8.o \
  $(JUCE_OBJDIR)/LiveOutput_48de9c16.o \
  $(JUCE_OBJDIR)/DLightOutput_b6ad71bc.o \
  $(JUCE_OBJDIR)/MilluminOutput_b125e8c3.o \
  $(JUCE_OBJDIR)/ResolumeClipCommand_489cbb6c.o \
  $(JUCE_OBJDIR)/ResolumeCompositionCommand_ea1ce09a.o \
  $(JUCE_OBJDIR)/ResolumeLayerCommand_b8990193.o \
  $(JUCE_OBJDIR)/ResolumeOutput_e3ef955a.o \
  $(JUCE_OBJDIR)/MIDIOutputModule_ed81e7f7.o \
  $(JUCE_OBJDIR)/SerialOutput_a577da1e.o \
  $(JUCE_OBJDIR)/OutputManagerUI_25a8df61.o \
  $(JUCE_OBJDIR)/OutputUI_81bc6f5e.o \
  $(JUCE_OBJDIR)/Output_dd6fd00a.o \
  $(JUCE_OBJDIR)/OutputFactory_d8f68faa.o \
  $(JUCE_OBJDIR)/OutputManager_d6ae64cd.o \
  $(JUCE_OBJDIR)/InputFactory_67bd3a69.o \
  $(JUCE_OBJDIR)/DMXInput_2920fd04.o \
  $(JUCE_OBJDIR)/MicrophoneInput_937e7d21.o \
  $(JUCE_OBJDIR)/LeapMotionInput_a9874f8d.o \
  $(JUCE_OBJDIR)/KinectV2Input_9ee97869.o \
  $(JUCE_OBJDIR)/KinectInput_f84ca505.o \
  $(JUCE_OBJDIR)/SerialInput_4c6e9197.o \
  $(JUCE_OBJDIR)/MIDIInputModule_266f02f6.o \
  $(JUCE_OBJDIR)/MyoInput_f847b050.o \
  $(JUCE_OBJDIR)/BentoInput_d625f2ad.o \
  $(JUCE_OBJDIR)/OSCInput_f1190114.o \
  $(JUCE_OBJDIR)/OSCInputUI_4fdd85e8.o \
  $(JUCE_OBJDIR)/CustomOSCInput_9f6a1025.o \
  $(JUCE_OBJDIR)/AugmentaInput_a0307c1b.o \
  $(JUCE_OBJDIR)/BGloveInput_fcb72dd4.o \
  $(JUCE_OBJDIR)/LiveInput_30abb09f.o \
  $(JUCE_OBJDIR)/DLightInput_3c788e39.o \
  $(JUCE_OBJDIR)/MilluminInput_97219a52.o \
  $(JUCE_OBJDIR)/ResolumeInput_56b47edb.o \
  $(JUCE_OBJDIR)/InputManagerUI_1d6fd660.o \
  $(JUCE_OBJDIR)/InputUI_f317c93f.o \
  $(JUCE_OBJDIR)/Input_3ea2e2b.o \
  $(JUCE_OBJDIR)/InputManager_65750f8c.o \
  $(JUCE_OBJDIR)/InspectableContent_25389b34.o \
  $(JUCE_OBJDIR)/Inspectable_484e00cf.o \
  $(JUCE_OBJDIR)/InspectableContentComponent_c06df73.o \
  $(JUCE_OBJDIR)/Inspector_d6237cd8.o \
  $(JUCE_OBJDIR)/InspectorEditor_dded95a5.o \
  $(JUCE_OBJDIR)/CustomEditor_3b5461c7.o \
  $(JUCE_OBJDIR)/FlapLogger_f90e106e.o \
  $(JUCE_OBJDIR)/FlapLoggerUI_2ecc26c2.o \
  $(JUCE_OBJDIR)/LookAndFeelOO_b6d1a33f.o \
  $(JUCE_OBJDIR)/AssetManager_d79828a6.o \
  $(JUCE_OBJDIR)/ShapeShifter_bf2b9b57.o \
  $(JUCE_OBJDIR)/ShapeShifterContainer_69052434.o \
  $(JUCE_OBJDIR)/ShapeShifterContent_505997ac.o \
  $(JUCE_OBJDIR)/ShapeShifterFactory_6c2ae3bd.o \
  $(JUCE_OBJDIR)/ShapeShifterManager_69e2b8e0.o \
  $(JUCE_OBJDIR)/ShapeShifterPanel_821dd737.o \
  $(JUCE_OBJDIR)/ShapeShifterPanelHeader_8b4f9de4.o \
  $(JUCE_OBJDIR)/ShapeShifterPanelTab_a4791be8.o \
  $(JUCE_OBJDIR)/ShapeShifterWindow_e5e759e7.o \
  $(JUCE_OBJDIR)/GapGrabber_4d565216.o \
  $(JUCE_OBJDIR)/Engine_7f3228cb.o \
  $(JUCE_OBJDIR)/EngineFileDocument_abfb722.o \
  $(JUCE_OBJDIR)/Main_90ebc5c2.o \
  $(JUCE_OBJDIR)/MainComponent_a6ffb4a5.o \
  $(JUCE_OBJDIR)/MainComponentCommands_b1179f4d.o \
  $(JUCE_OBJDIR)/BinaryData_ce4232d4.o \
  $(JUCE_OBJDIR)/juce_audio_basics_6b797ca1.o \
  $(JUCE_OBJDIR)/juce_audio_devices_a742c38b.o \
  $(JUCE_OBJDIR)/juce_audio_formats_5a29c68a.o \
  $(JUCE_OBJDIR)/juce_audio_processors_dea3173d.o \
  $(JUCE_OBJDIR)/juce_core_75b14332.o \
  $(JUCE_OBJDIR)/juce_cryptography_6de2ebff.o \
  $(JUCE_OBJDIR)/juce_data_structures_72d3da2c.o \
  $(JUCE_OBJDIR)/juce_events_d2be882c.o \
  $(JUCE_OBJDIR)/juce_graphics_9c18891e.o \
  $(JUCE_OBJDIR)/juce_gui_basics_8a6da59c.o \
  $(JUCE_OBJDIR)/juce_gui_extra_4a026f23.o \
  $(JUCE_OBJDIR)/juce_opengl_cd70b4c2.o \
  $(JUCE_OBJDIR)/juce_osc_be4cee16.o \
  $(JUCE_OBJDIR)/juce_video_f128c512.o \

.PHONY: clean

$(JUCE_OUTDIR)/$(TARGET): check-pkg-config $(OBJECTS) $(RESOURCES)
	@echo Linking Flap
	-@mkdir -p $(JUCE_BINDIR)
	-@mkdir -p $(JUCE_LIBDIR)
	-@mkdir -p $(JUCE_OUTDIR)
	@$(BLDCMD)

check-pkg-config:
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa freetype2 libcurl x11 xext xinerama

clean:
	@echo Cleaning Flap
	@$(CLEANCMD)

strip:
	@echo Stripping Flap
	-@$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

$(JUCE_OBJDIR)/Outliner_324682f9.o: ../../Source/Outliner.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Outliner.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ProgressNotifier_22cad7fc.o: ../../Source/ProgressNotifier.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ProgressNotifier.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ProgressWindow_8ee8f986.o: ../../Source/ProgressWindow.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ProgressWindow.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BoolParameter_60c297c0.o: ../../Source/BoolParameter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BoolParameter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/EnumParameter_6e309b09.o: ../../Source/EnumParameter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling EnumParameter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FloatParameter_67c285b6.o: ../../Source/FloatParameter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FloatParameter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/IntParameter_21cc1c03.o: ../../Source/IntParameter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IntParameter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Parameter_73564c8a.o: ../../Source/Parameter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Parameter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Point2DParameter_7a5034f0.o: ../../Source/Point2DParameter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Point2DParameter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Point3DParameter_d42b9f31.o: ../../Source/Point3DParameter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Point3DParameter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StringParameter_fe3ecef9.o: ../../Source/StringParameter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StringParameter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TargetParameter_c17e2d79.o: ../../Source/TargetParameter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TargetParameter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BetterStepper_8f8519a2.o: ../../Source/BetterStepper.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BetterStepper.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BoolToggleUI_bb303d7b.o: ../../Source/BoolToggleUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BoolToggleUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DoubleSliderUI_f128d6af.o: ../../Source/DoubleSliderUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DoubleSliderUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/EnumParameterUI_e576769d.o: ../../Source/EnumParameterUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling EnumParameterUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FloatSliderUI_d00e5892.o: ../../Source/FloatSliderUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FloatSliderUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FloatStepperUI_bb9cf532.o: ../../Source/FloatStepperUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FloatStepperUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/IntSliderUI_a48244a5.o: ../../Source/IntSliderUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IntSliderUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/IntStepperUI_75a68b7f.o: ../../Source/IntStepperUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IntStepperUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ParameterUI_37f5cbde.o: ../../Source/ParameterUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ParameterUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StringParameterUI_aac76e8d.o: ../../Source/StringParameterUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StringParameterUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TargetParameterUI_9ba92d0d.o: ../../Source/TargetParameterUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TargetParameterUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TripleSliderUI_836dfbbc.o: ../../Source/TripleSliderUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TripleSliderUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PresetChooser_96a32abd.o: ../../Source/PresetChooser.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PresetChooser.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PresetManager_fc30642f.o: ../../Source/PresetManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PresetManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GenericControllableContainerEditor_117ebfb7.o: ../../Source/GenericControllableContainerEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GenericControllableContainerEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControllableEditor_dd5c95df.o: ../../Source/ControllableEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControllableEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControllableUI_c544de6.o: ../../Source/ControllableUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControllableUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TriggerBlinkUI_693175a1.o: ../../Source/TriggerBlinkUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TriggerBlinkUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TriggerButtonUI_d1a8d47f.o: ../../Source/TriggerButtonUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TriggerButtonUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TriggerUI_2ce661ad.o: ../../Source/TriggerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TriggerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Controllable_d3fdb092.o: ../../Source/Controllable.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Controllable.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControllableContainer_f3a7c159.o: ../../Source/ControllableContainer.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControllableContainer.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControllableContainerEditor_9832b566.o: ../../Source/ControllableContainerEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControllableContainerEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControllableFactory_10ac8622.o: ../../Source/ControllableFactory.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControllableFactory.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControllableHelpers_912c061d.o: ../../Source/ControllableHelpers.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControllableHelpers.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ControllableIndexedContainer_a91d6542.o: ../../Source/ControllableIndexedContainer.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ControllableIndexedContainer.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Trigger_dbc00a19.o: ../../Source/Trigger.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Trigger.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BaseCommand_529ec09b.o: ../../Source/BaseCommand.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BaseCommand.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CommandFactory_88728348.o: ../../Source/CommandFactory.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CommandFactory.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CommandChooserUI_aed1be8d.o: ../../Source/CommandChooserUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CommandChooserUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BaseItem_6b33fe0d.o: ../../Source/BaseItem.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BaseItem.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConsequenceManagerUI_6e459487.o: ../../Source/ConsequenceManagerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConsequenceManagerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConsequenceUI_adc498f8.o: ../../Source/ConsequenceUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConsequenceUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Consequence_78c99824.o: ../../Source/Consequence.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Consequence.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConsequenceManager_ed222b73.o: ../../Source/ConsequenceManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConsequenceManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BoolComparators_278bc7f4.o: ../../Source/BoolComparators.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BoolComparators.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NumberComparators_7676c2b5.o: ../../Source/NumberComparators.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NumberComparators.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Point2DComparators_1426d924.o: ../../Source/Point2DComparators.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Point2DComparators.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Point3DComparators_64d0b725.o: ../../Source/Point3DComparators.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Point3DComparators.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StringComparators_56d714ed.o: ../../Source/StringComparators.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StringComparators.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BaseComparatorUI_36712464.o: ../../Source/BaseComparatorUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BaseComparatorUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FloatComparatorUI_28ecccc7.o: ../../Source/FloatComparatorUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FloatComparatorUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BaseComparator_fbba0e90.o: ../../Source/BaseComparator.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BaseComparator.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ComparatorFactory_14bedeb5.o: ../../Source/ComparatorFactory.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ComparatorFactory.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConditionManagerUI_219422af.o: ../../Source/ConditionManagerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConditionManagerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConditionUI_37f827d0.o: ../../Source/ConditionUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConditionUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Condition_dbc2fcfc.o: ../../Source/Condition.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Condition.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConditionManager_8b8e239b.o: ../../Source/ConditionManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConditionManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ActionEditor_5de4624c.o: ../../Source/ActionEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ActionEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ActionManagerUI_82c1584c.o: ../../Source/ActionManagerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ActionManagerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ActionUI_e36558d3.o: ../../Source/ActionUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ActionUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Action_97be92bf.o: ../../Source/Action.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Action.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ActionManager_9ed61c78.o: ../../Source/ActionManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ActionManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingInputUI_8f4826b9.o: ../../Source/MappingInputUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingInputUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingInput_19a7a225.o: ../../Source/MappingInput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingInput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingOutputManagerUI_16c922db.o: ../../Source/MappingOutputManagerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingOutputManagerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingOutputUI_6b97c124.o: ../../Source/MappingOutputUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingOutputUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingOutput_7f60db50.o: ../../Source/MappingOutput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingOutput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingOutputManager_681d3ec7.o: ../../Source/MappingOutputManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingOutputManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingFilterManagerUI_447b69c4.o: ../../Source/MappingFilterManagerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingFilterManagerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingFilterUI_f2877e5b.o: ../../Source/MappingFilterUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingFilterUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingFilter_6ebc7a47.o: ../../Source/MappingFilter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingFilter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingFilterManager_c0562bf0.o: ../../Source/MappingFilterManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingFilterManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingManagerUI_a4d2f6dc.o: ../../Source/MappingManagerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingManagerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingUI_56e9fc43.o: ../../Source/MappingUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Mapping_bdf5522f.o: ../../Source/Mapping.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Mapping.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingManager_a2de1f08.o: ../../Source/MappingManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceLayerFactory_ac14d863.o: ../../Source/SequenceLayerFactory.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceLayerFactory.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioLayerPanel_faa1fb8a.o: ../../Source/AudioLayerPanel.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioLayerPanel.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioLayerTimeline_7cd71005.o: ../../Source/AudioLayerTimeline.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioLayerTimeline.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioLayer_60c0cf24.o: ../../Source/AudioLayer.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioLayer.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingLayerPanel_890eab82.o: ../../Source/MappingLayerPanel.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingLayerPanel.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingLayerTimeline_96e8bd0d.o: ../../Source/MappingLayerTimeline.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingLayerTimeline.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MappingLayer_50b4042c.o: ../../Source/MappingLayer.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MappingLayer.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TriggerLayerPanel_72c06ec.o: ../../Source/TriggerLayerPanel.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TriggerLayerPanel.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TriggerLayerTimeline_b139b163.o: ../../Source/TriggerLayerTimeline.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TriggerLayerTimeline.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeTrigger_4a6ad70c.o: ../../Source/TimeTrigger.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeTrigger.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TriggerLayer_1751ae82.o: ../../Source/TriggerLayer.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TriggerLayer.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceLayerTimelineManagerUI_a4660719.o: ../../Source/SequenceLayerTimelineManagerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceLayerTimelineManagerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceLayerPanel_5928bd5d.o: ../../Source/SequenceLayerPanel.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceLayerPanel.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceLayerTimeline_a0ac9952.o: ../../Source/SequenceLayerTimeline.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceLayerTimeline.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceLayerPanelManagerUI_8a7a3f6e.o: ../../Source/SequenceLayerPanelManagerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceLayerPanelManagerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceLayerManager_a9ccad86.o: ../../Source/SequenceLayerManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceLayerManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceLayer_e986f571.o: ../../Source/SequenceLayer.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceLayer.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceTimelineHeader_4a3b0458.o: ../../Source/SequenceTimelineHeader.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceTimelineHeader.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceTimelineSeeker_db3332f0.o: ../../Source/SequenceTimelineSeeker.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceTimelineSeeker.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceEditor_f0150517.o: ../../Source/SequenceEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceTimelineNavigationUI_8126f373.o: ../../Source/SequenceTimelineNavigationUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceTimelineNavigationUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceTransportUI_5072a95d.o: ../../Source/SequenceTransportUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceTransportUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceUI_6f55191e.o: ../../Source/SequenceUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Sequence_b8a7e9ca.o: ../../Source/Sequence.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Sequence.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceManagerUI_cc91bda1.o: ../../Source/SequenceManagerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceManagerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TimeMachineView_76e28760.o: ../../Source/TimeMachineView.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TimeMachineView.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SequenceManager_52b9d30d.o: ../../Source/SequenceManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SequenceManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StateViewUI_7695054b.o: ../../Source/StateViewUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateViewUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StateUI_4c5e44c6.o: ../../Source/StateUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/State_21d7df72.o: ../../Source/State.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling State.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TransitionConnector_1e3508d9.o: ../../Source/TransitionConnector.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TransitionConnector.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TransitionUI_3d0f2ab2.o: ../../Source/TransitionUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TransitionUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Transition_3f21505e.o: ../../Source/Transition.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Transition.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TransitionManager_82f9df9.o: ../../Source/TransitionManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TransitionManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StateMachineView_e5001744.o: ../../Source/StateMachineView.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateMachineView.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StateManagerUI_e1b7a4f9.o: ../../Source/StateManagerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateManagerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StateManager_b261b065.o: ../../Source/StateManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StateManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OutputCommand_7d3e356b.o: ../../Source/OutputCommand.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OutputCommand.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OSCCommand_a034ed5.o: ../../Source/OSCCommand.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OSCCommand.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OSCOutput_961b5a41.o: ../../Source/OSCOutput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OSCOutput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BGloveOutput_fe42c581.o: ../../Source/BGloveOutput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BGloveOutput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomOSCCommand_684cdda6.o: ../../Source/CustomOSCCommand.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomOSCCommand.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomOSCOutput_b1ec2d50.o: ../../Source/CustomOSCOutput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomOSCOutput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BentoOutput_52ac9bc8.o: ../../Source/BentoOutput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BentoOutput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LiveOutput_48de9c16.o: ../../Source/LiveOutput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LiveOutput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DLightOutput_b6ad71bc.o: ../../Source/DLightOutput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DLightOutput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MilluminOutput_b125e8c3.o: ../../Source/MilluminOutput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MilluminOutput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ResolumeClipCommand_489cbb6c.o: ../../Source/ResolumeClipCommand.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ResolumeClipCommand.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ResolumeCompositionCommand_ea1ce09a.o: ../../Source/ResolumeCompositionCommand.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ResolumeCompositionCommand.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ResolumeLayerCommand_b8990193.o: ../../Source/ResolumeLayerCommand.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ResolumeLayerCommand.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ResolumeOutput_e3ef955a.o: ../../Source/ResolumeOutput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ResolumeOutput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MIDIOutputModule_ed81e7f7.o: ../../Source/MIDIOutputModule.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MIDIOutputModule.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SerialOutput_a577da1e.o: ../../Source/SerialOutput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SerialOutput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OutputManagerUI_25a8df61.o: ../../Source/OutputManagerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OutputManagerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OutputUI_81bc6f5e.o: ../../Source/OutputUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OutputUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Output_dd6fd00a.o: ../../Source/Output.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Output.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OutputFactory_d8f68faa.o: ../../Source/OutputFactory.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OutputFactory.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OutputManager_d6ae64cd.o: ../../Source/OutputManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OutputManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InputFactory_67bd3a69.o: ../../Source/InputFactory.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InputFactory.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DMXInput_2920fd04.o: ../../Source/DMXInput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DMXInput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MicrophoneInput_937e7d21.o: ../../Source/MicrophoneInput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MicrophoneInput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LeapMotionInput_a9874f8d.o: ../../Source/LeapMotionInput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LeapMotionInput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/KinectV2Input_9ee97869.o: ../../Source/KinectV2Input.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling KinectV2Input.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/KinectInput_f84ca505.o: ../../Source/KinectInput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling KinectInput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SerialInput_4c6e9197.o: ../../Source/SerialInput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SerialInput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MIDIInputModule_266f02f6.o: ../../Source/MIDIInputModule.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MIDIInputModule.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MyoInput_f847b050.o: ../../Source/MyoInput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MyoInput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BentoInput_d625f2ad.o: ../../Source/BentoInput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BentoInput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OSCInput_f1190114.o: ../../Source/OSCInput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OSCInput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OSCInputUI_4fdd85e8.o: ../../Source/OSCInputUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OSCInputUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomOSCInput_9f6a1025.o: ../../Source/CustomOSCInput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomOSCInput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AugmentaInput_a0307c1b.o: ../../Source/AugmentaInput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AugmentaInput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BGloveInput_fcb72dd4.o: ../../Source/BGloveInput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BGloveInput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LiveInput_30abb09f.o: ../../Source/LiveInput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LiveInput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DLightInput_3c788e39.o: ../../Source/DLightInput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DLightInput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MilluminInput_97219a52.o: ../../Source/MilluminInput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MilluminInput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ResolumeInput_56b47edb.o: ../../Source/ResolumeInput.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ResolumeInput.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InputManagerUI_1d6fd660.o: ../../Source/InputManagerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InputManagerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InputUI_f317c93f.o: ../../Source/InputUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InputUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Input_3ea2e2b.o: ../../Source/Input.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Input.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InputManager_65750f8c.o: ../../Source/InputManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InputManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InspectableContent_25389b34.o: ../../Source/InspectableContent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InspectableContent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Inspectable_484e00cf.o: ../../Source/Inspectable.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Inspectable.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InspectableContentComponent_c06df73.o: ../../Source/InspectableContentComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InspectableContentComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Inspector_d6237cd8.o: ../../Source/Inspector.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Inspector.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InspectorEditor_dded95a5.o: ../../Source/InspectorEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InspectorEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CustomEditor_3b5461c7.o: ../../Source/CustomEditor.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CustomEditor.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FlapLogger_f90e106e.o: ../../Source/FlapLogger.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FlapLogger.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FlapLoggerUI_2ecc26c2.o: ../../Source/FlapLoggerUI.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FlapLoggerUI.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LookAndFeelOO_b6d1a33f.o: ../../Source/LookAndFeelOO.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LookAndFeelOO.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AssetManager_d79828a6.o: ../../Source/AssetManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AssetManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifter_bf2b9b57.o: ../../Source/ShapeShifter.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifter.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifterContainer_69052434.o: ../../Source/ShapeShifterContainer.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifterContainer.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifterContent_505997ac.o: ../../Source/ShapeShifterContent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifterContent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifterFactory_6c2ae3bd.o: ../../Source/ShapeShifterFactory.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifterFactory.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifterManager_69e2b8e0.o: ../../Source/ShapeShifterManager.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifterManager.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifterPanel_821dd737.o: ../../Source/ShapeShifterPanel.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifterPanel.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifterPanelHeader_8b4f9de4.o: ../../Source/ShapeShifterPanelHeader.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifterPanelHeader.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifterPanelTab_a4791be8.o: ../../Source/ShapeShifterPanelTab.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifterPanelTab.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ShapeShifterWindow_e5e759e7.o: ../../Source/ShapeShifterWindow.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ShapeShifterWindow.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GapGrabber_4d565216.o: ../../Source/GapGrabber.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GapGrabber.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Engine_7f3228cb.o: ../../Source/Engine.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Engine.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/EngineFileDocument_abfb722.o: ../../Source/EngineFileDocument.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling EngineFileDocument.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Main.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainComponent_a6ffb4a5.o: ../../Source/MainComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainComponentCommands_b1179f4d.o: ../../Source/MainComponentCommands.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainComponentCommands.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_basics_6b797ca1.o: ../../JuceLibraryCode/juce_audio_basics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_devices_a742c38b.o: ../../JuceLibraryCode/juce_audio_devices.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_formats_5a29c68a.o: ../../JuceLibraryCode/juce_audio_formats.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_processors_dea3173d.o: ../../JuceLibraryCode/juce_audio_processors.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_core_75b14332.o: ../../JuceLibraryCode/juce_core.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_cryptography_6de2ebff.o: ../../JuceLibraryCode/juce_cryptography.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_data_structures_72d3da2c.o: ../../JuceLibraryCode/juce_data_structures.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_events_d2be882c.o: ../../JuceLibraryCode/juce_events.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_graphics_9c18891e.o: ../../JuceLibraryCode/juce_graphics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_gui_basics_8a6da59c.o: ../../JuceLibraryCode/juce_gui_basics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_gui_extra_4a026f23.o: ../../JuceLibraryCode/juce_gui_extra.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_opengl_cd70b4c2.o: ../../JuceLibraryCode/juce_opengl.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_osc_be4cee16.o: ../../JuceLibraryCode/juce_osc.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_osc.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_video_f128c512.o: ../../JuceLibraryCode/juce_video.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_video.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
