/*
  ==============================================================================

    CVPresetMorphUI.h
    Created: 11 Jun 2019 10:26:13am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "MorphTargetUI.h"
#include "../../CVPreset.h"

class CVPresetMorphUI :
	public BaseItemMinimalUI<CVPreset>
{
public:
	CVPresetMorphUI(CVPreset * preset);
	~CVPresetMorphUI();

	void paint(Graphics& g) override;
};
