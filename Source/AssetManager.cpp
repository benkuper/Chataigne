#include "AssetManager.h"

juce_ImplementSingleton(AssetManager);

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{


}

Image AssetManager::getGridImage()
{
	return ImageCache::getFromMemory(BinaryData::removeBT_png, BinaryData::removeBT_pngSize);
}

ImageButton * AssetManager::getRemoveBT()
{
	Image removeImage = ImageCache::getFromMemory(BinaryData::removeBT_png, BinaryData::removeBT_pngSize);
	return getSetupBTImage(removeImage);
}

ImageButton * AssetManager::getConfigBT()
{
	Image removeImage = ImageCache::getFromMemory(BinaryData::configBT_png, BinaryData::configBT_pngSize);
	return getSetupBTImage(removeImage);
}

ImageButton * AssetManager::getTargetBT()
{
	Image removeImage = ImageCache::getFromMemory(BinaryData::target_png, BinaryData::target_pngSize);
	return getSetupBTImage(removeImage);
}

ImageButton * AssetManager::getSetupBTImage(const Image & image)
{
	ImageButton * removeBT = new ImageButton();
	removeBT->setImages(false, true, true, image,
		0.7f, Colours::transparentBlack,
		image, 1.0f, Colours::transparentBlack,
		image, 1.0f, Colours::white.withAlpha(.7f),
		0.5f);
	return removeBT;
}
