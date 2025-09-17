/*
  ==============================================================================

    CVPresetMorphUI.h
    Created: 11 Jun 2019 10:26:13am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class CVPresetMorphUI :
	public BaseItemMinimalUI<CVPreset>
{
public:
	CVPresetMorphUI(CVPreset * preset);
	~CVPresetMorphUI();

	void paint(Graphics& g) override;
};
