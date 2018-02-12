/*
  ==============================================================================

    MIDIDeviceChooser.h
    Created: 20 Dec 2016 12:35:11pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MIDIDEVICECHOOSER_H_INCLUDED
#define MIDIDEVICECHOOSER_H_INCLUDED

#include "MIDIManager.h"

class MIDIDeviceChooser :
	public Component,
	public ComboBox::Listener,
	public MIDIManager::Listener
{
public:
	MIDIDeviceChooser(bool showInputs = true, bool showOutputs = true);
	~MIDIDeviceChooser();

	bool showInputs;
	//Label inputLabel;
	ComboBox inputBox;

	bool showOutputs;
	//Label outputLabel;
	ComboBox outputBox;

	MIDIInputDevice * currentInputDevice;
	MIDIOutputDevice * currentOutputDevice;

	void resized() override;

	void setGhostValues(const String &inValue, const String &outValue);

	void updateInputComboBox();
	void updateOutputComboBox();

	void setSelectedInputDevice(MIDIInputDevice * );
	void setSelectedInputDevice(const String &deviceName);
	void setSelectedOutputDevice(MIDIOutputDevice *);
	void setSelectedOutputDevice(const String &deviceName);

	virtual void comboBoxChanged(ComboBox * ccb) override;

	virtual void midiDeviceInAdded(MIDIInputDevice *) override;
	virtual void midiDeviceOutAdded(MIDIOutputDevice *) override;
	virtual void midiDeviceInRemoved(MIDIInputDevice *) override;
	virtual void midiDeviceOutRemoved(MIDIOutputDevice *) override;

	class  ChooserListener
	{
	public:
		/** Destructor. */
		virtual ~ChooserListener() {}
		virtual void midiDeviceInSelected(MIDIInputDevice * /*input*/) {}
		virtual void midiDeviceOutSelected(MIDIOutputDevice * /*output*/) {}
	};

	ListenerList<ChooserListener> chooserListeners;
	void addMIDIChooserListener(ChooserListener* newListener) { chooserListeners.add(newListener); }
	void removeMIDIChooserListener(ChooserListener* listener) { chooserListeners.remove(listener); }

};

#endif  // MIDIDEVICECHOOSER_H_INCLUDED
