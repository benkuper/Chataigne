/*
  ==============================================================================

	StreamDeckPedal.cpp
	Created: 16 Nov 2026 10:00:00am
	Author:  bkupe

  ==============================================================================
*/

// Elgato doesn't publish a HID protocol page for the Pedal. Button report layout
// (report id 1, pedal states at bytes 4/5/6) cross-checked between two independent
// reverse-engineering efforts:
// https://github.com/pejalo/stream-deck-pedal (src/main.rs)
// https://github.com/abcminiuser/python-elgato-streamdeck (Devices/StreamDeckPedal.py),
// which also confirms the Pedal doesn't respond to the reset/brightness feature reports.

StreamDeckPedal::StreamDeckPedal(hid_device* device, String serialNumber) :
	StreamDeck(device, serialNumber, PEDAL, 3, 1, false, 0, 4)
{
}

StreamDeckPedal::~StreamDeckPedal()
{
}

void StreamDeckPedal::setColor(int row, int column, Colour color, bool highlight, const String& overlayText, int textSize)
{
	//The Pedal has no display, it is purely 3 momentary buttons
}

void StreamDeckPedal::setImage(int row, int column, Image image, bool highlight, const String& overlayText, int textSize)
{
	//The Pedal has no display, it is purely 3 momentary buttons
}

void StreamDeckPedal::setImage(int row, int column, Image image, Colour tint, bool highlight, const String& overlayText, int textSize)
{
	//The Pedal has no display, it is purely 3 momentary buttons
}

void StreamDeckPedal::sendButtonImageData(int row, int column, Image& img)
{
	//The Pedal has no display, it is purely 3 momentary buttons
}

void StreamDeckPedal::sendFeatureReport(const uint8_t* data, int length)
{
	//The Pedal ignores the reset and brightness feature reports (no display or LEDs to reset/dim)
}
