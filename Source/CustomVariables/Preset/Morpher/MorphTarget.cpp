/*
  ==============================================================================

    MorphTarget.cpp
    Created: 11 Jun 2019 2:11:04pm
    Author:  bkupe

  ==============================================================================
*/

#include "MorphTarget.h"

MorphTarget::MorphTarget(StringRef name) :
	BaseItem(name)
{
	showInspectorOnSelect = false;

	weight = addFloatParameter("Weight", "Weight of this preset in a non-free control mode", 0, 0, 1);
	weight->hideInEditor = true;

	//position = addPoint2DParameter("Position", "Position in a 2D interpolation control mode, such as Voronoi or Gradient Band");
	attraction = addFloatParameter("Attraction", "Attraction in a 2D interpolation control mode with physics", 0, 0, 1);
	attraction->hideInEditor = true;

	Random r(Time::getApproximateMillisecondCounter());
	targetColor = addColorParameter("Color", "The color of the handle in a 2D interpolation editor", Colour::fromHSV(r.nextFloat(), 1, 1, 1));
	targetColor->hideInEditor = true;
}

MorphTarget::~MorphTarget()
{
}
