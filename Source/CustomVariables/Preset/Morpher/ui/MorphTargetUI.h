/*
  ==============================================================================

    MorphTargetUI.h
    Created: 11 Jun 2019 10:26:55am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class MorphTargetUI :
	public BaseItemMinimalUI<MorphTarget>
{
public:
	MorphTargetUI(MorphTarget* mt);
	~MorphTargetUI();

	void paint(Graphics& g) override;
};