/*
  ==============================================================================

    CVGroup.h
    Created: 15 Feb 2018 3:49:35pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Preset/CVPresetManager.h"

class CVGroup :
	public BaseItem
{
public:
	CVGroup(const String &name = "Group");
	~CVGroup();

	GenericControllableManager values;
	CVPresetManager pm;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;
};