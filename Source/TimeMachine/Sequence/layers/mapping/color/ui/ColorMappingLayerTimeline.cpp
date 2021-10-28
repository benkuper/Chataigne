/*
  ==============================================================================

	ColorMappingLayerTimeline.cpp
	Created: 25 Mar 2020 1:45:28pm
	Author:  bkupe

  ==============================================================================
*/

ColorMappingLayerTimeline::ColorMappingLayerTimeline(ColorMappingLayer* layer) :
	MappingLayerTimeline(layer, new GradientColorManagerUI(&layer->colorManager)),
	cml(layer)
{
	colorManagerUI = ((GradientColorManagerUI*)mainComponent.get());
	std::function<void(Array<float>*)> func = std::bind(&ColorMappingLayer::getSequenceSnapTimesForManager, layer, std::placeholders::_1);
	colorManagerUI->getSnapTimesFunc = func;
	updateMiniModeUI();
}

ColorMappingLayerTimeline::~ColorMappingLayerTimeline()
{
}

void ColorMappingLayerTimeline::updateContent()
{
	colorManagerUI->setViewRange(item->sequence->viewStartTime->floatValue(), item->sequence->viewEndTime->floatValue());
}

void ColorMappingLayerTimeline::updateMiniModeUI()
{
	colorManagerUI->setMiniMode(item->miniMode->boolValue());
}

void ColorMappingLayerTimeline::addSelectableComponentsAndInspectables(Array<Component*>& selectables, Array<Inspectable*>& inspectables)
{
	if (colorManagerUI == nullptr) return;
	colorManagerUI->addSelectableComponentsAndInspectables(selectables, inspectables);
}
