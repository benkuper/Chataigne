/*
  ==============================================================================

    StreamDeckMini.cpp
    Created: 14 Feb 2020 12:29:01pm
    Author:  bkupe

  ==============================================================================
*/

#define ICON_SIZE 72
#define ICON_PIXELS ICON_SIZE*ICON_SIZE
#define ICON_BYTES ICON_PIXELS * 3

#define PACKET1_HEADER_SIZE 70
#define PACKET1_PIXELS 2583   
#define PACKET1_PIXELS_BYTES PACKET1_PIXELS * 3   

#define PACKET2_HEADER_SIZE 16
#define PACKET2_PIXELS 2601  
#define PACKET2_PIXELS_BYTES PACKET2_PIXELS * 3   

#define PACKET_SIZE 8191

StreamDeckMini::StreamDeckMini(hid_device* device, String serialNumber) :
	StreamDeck(device, serialNumber, STANDARD_V1, 3, 2, false, 72, 1)
{
	resetData.add(0x0B, 0x63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	brightnessData.add(0x05, 0x55, 0xAA, 0xD1, 0x01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);


	page1Header.add(
		0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x4D, 0xF6,
		0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00,
		0x48, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x3C, 0x00, 0x00, 0x13, 0x0E, 0x00,
		0x00, 0x13, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	);

	page2Header.add(0x02, 0x01, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

}

StreamDeckMini::~StreamDeckMini()
{
}

void StreamDeckMini::sendButtonImageData(int row, int column, Image& img)
{
	writeLock.enter();

	int buttonID = row * column + column;

	Image::BitmapData bitmapData(img, Image::BitmapData::ReadWriteMode::readOnly);

	MemoryOutputStream block;

	page1Header.set(5, buttonID + 1);
	page2Header.set(5, buttonID + 1);

	MemoryBlock packet1;
	packet1.ensureSize(PACKET_SIZE, true);
	packet1.copyFrom(page1Header.getRawDataPointer(), 0, PACKET1_HEADER_SIZE);
	packet1.copyFrom(bitmapData.data, page1Header.size(), PACKET1_PIXELS_BYTES);

	MemoryBlock packet2;
	packet2.ensureSize(PACKET_SIZE, true);
	packet2.copyFrom(page2Header.getRawDataPointer(), 0, PACKET2_HEADER_SIZE);
	packet2.copyFrom(bitmapData.data + PACKET1_PIXELS_BYTES, PACKET2_HEADER_SIZE, PACKET2_PIXELS_BYTES);

	if (device != nullptr)
	{
		try {
			hid_write(device, (unsigned char*)packet1.getData(), PACKET_SIZE);
			hid_write(device, (unsigned char*)packet2.getData(), PACKET_SIZE);
		}
		catch (std::exception e)
		{
			NLOGERROR("StreamDeck", "Error write image to device");
		}
	}

	writeLock.exit();
}