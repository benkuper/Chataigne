/*
  ==============================================================================

    CVPresetManager.h
    Created: 22 Feb 2018 4:10:48pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class CVGroup;

class CVPresetManager :
	public Manager<CVPreset>
{
public:
	CVPresetManager(CVGroup * group);
	~CVPresetManager();

	CVGroup * group;
	CVPreset * createItem() override;
};