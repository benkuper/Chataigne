/*
  ==============================================================================

    AutomationMappingLayerPanel.cpp
    Created: 25 Mar 2020 1:46:10pm
    Author:  bkupe

  ==============================================================================
*/

AutomationMappingLayerPanel::AutomationMappingLayerPanel(AutomationMappingLayer* layer) :
    MappingLayerPanel(layer),
    aml(layer)
{
    armUI.reset(aml->recorder.arm->createToggle());
    addAndMakeVisible(armUI.get());

    keyBT.reset(AssetManager::getInstance()->getBTDoubleImage(
        ImageCache::getFromMemory(BinaryData::keyframe_on_png, BinaryData::keyframe_on_pngSize),
        ImageCache::getFromMemory(BinaryData::keyframe_off_png, BinaryData::keyframe_off_pngSize)
    ));

    prevKeyBT.reset(AssetManager::getInstance()->getToggleBTImage(ImageCache::getFromMemory(BinaryData::prevkey_png, BinaryData::prevkey_pngSize)));
    nextKeyBT.reset(AssetManager::getInstance()->getToggleBTImage(ImageCache::getFromMemory(BinaryData::nextkey_png, BinaryData::nextkey_pngSize)));

    keyBT->addListener(this);
    prevKeyBT->addListener(this);
    nextKeyBT->addListener(this);

    addAndMakeVisible(keyBT.get());
    addAndMakeVisible(prevKeyBT.get());
    addAndMakeVisible(nextKeyBT.get());

    contentComponents.add(armUI.get());
}

AutomationMappingLayerPanel::~AutomationMappingLayerPanel()
{
}

void AutomationMappingLayerPanel::resizedInternalPanelContent(Rectangle<int>& r)
{
    MappingLayerPanel::resizedInternalPanelContent(r);
    r.removeFromTop(2);
    Rectangle<int> ar = r.removeFromTop(20);
    armUI->setBounds(ar.removeFromLeft(80).reduced(2));
    nextKeyBT->setBounds(ar.removeFromRight(20).reduced(2));
    keyBT->setBounds(ar.removeFromRight(20).reduced(2));
    prevKeyBT->setBounds(ar.removeFromRight(20).reduced(2));
}

void AutomationMappingLayerPanel::buttonClicked(Button* b)
{
    MappingLayerPanel::buttonClicked(b);

    if (b == keyBT.get())
    {
        aml->automation->insertKeyAt(aml->sequence->currentTime->floatValue());
    }
    else if (b == prevKeyBT.get())
    {
        if (AutomationKey* k = aml->automation->getKeyForPosition(aml->sequence->currentTime->floatValue(), false))
        {
            aml->sequence->setCurrentTime(aml->sequence->getPrevFrameTimeForTime(k->position->floatValue()), true, true);
        }
    }
    else if (b == nextKeyBT.get())
    {
        if (AutomationKey* k = aml->automation->getNextKeyForPosition(aml->sequence->currentTime->floatValue(), false))
        {
            aml->sequence->setCurrentTime(aml->sequence->getNextFrameTimeForTime(k->position->floatValue()), true, true);
        }
    }
}
