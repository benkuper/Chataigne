/*
 ==============================================================================

 PresetManager.h
 Created: 25 Apr 2016 2:02:05pm
 Author:  bkupe

 ==============================================================================
 */

#ifndef PRESETMANAGER_H_INCLUDED
#define PRESETMANAGER_H_INCLUDED

#include "JuceHeader.h"//keep
class ControllableContainer;


// if adding option take care to change static function get num options
enum PresetChoice
{
	SaveCurrent = -3,
	SaveToNew = -2,
	ResetToDefault = -1,
	deleteStartId = 1000
};


class PresetManager
{
public:
	class PresetValue
	{
	public:
		PresetValue(const String &_controlAddress, var _value) : paramControlAddress(_controlAddress), presetValue(_value) {}
		String paramControlAddress;
		var presetValue;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetValue)
	};

	class Preset
	{
	public:
		Preset(const String & _name, String _filter) : filter(_filter), name(_name) {}

		String filter; //Used to filter which preset to propose depending on the object (specific nodes, vst, controller, etc.)
		String name;
		OwnedArray<PresetValue> presetValues;
		int presetId; //change each time the a preset list is created, but we don't care because ControllableContainer keeps the pointer to the Preset

		void addPresetValue(const String &controlAddress, var value);
		void addPresetValues(Array<PresetValue *> _presetValues);
		var getPresetValue(const String &targetControlAddress);

		void clear();

		var getJSONData();

		void loadJSONData(var data);

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Preset)
	};

	juce_DeclareSingleton(PresetManager, true)

		OwnedArray<Preset> presets;

	PresetManager();
	virtual ~PresetManager();

	Preset * addPresetFromControllableContainer(const String &name, const String & filter, ControllableContainer * container, bool recursive = false, bool includeNotExposed = false);
	Preset * getPreset(String filter, const String &name) const;
	void fillWithPresets(ComboBox * cb, const String & filter, bool showSaveCurrent = true) const;
	void removePresetForIdx(int idx);
	int getNumPresetForFilter(const String &) const;

	void clear();


	var getJSONData();
	void loadJSONData(var data);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetManager)
};


#endif  // PRESETMANAGER_H_INCLUDED
