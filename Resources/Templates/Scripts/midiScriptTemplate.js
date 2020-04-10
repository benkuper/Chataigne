/* ********** MIDI MODULE SPECIFIC SCRIPTING ********************* */
/*

MIDI Modules have specific methods that can be used to send MIDI events such as noteOn, noteOff, controlChange and sysEx messages from Script.
If you want to send a MIDI event from this script, you can do the following :

local.sendNoteOn(1, 12, 127); //This will send a NoteOn Event on channel 1, pitch 12, velocity 127
local.sendNoteOff(1, 12); //This will send a NoteOff Event on chanenl 1, pitch 12
local.sendCC(3, 20, 65); //This will send a ControlChange on channel 3, number 20, value 65
local.sendSysEx(15,20,115,10); //This will send 4 bytes as a SysEx message
local.sendPitchWheel (3, 2000);
local.sendChannelPressure (1, 67);
local.sendAfterTouch (3, 20, 65);
*/

/*
You can intercept MIDI Events with the functions below
*/

function noteOnEvent(channel, pitch, velocity)
{
	script.log("Note on received "+channel+", "+pitch+", "+velocity);
}


function noteOffEvent(channel, pitch, velocity)
{
	script.log("Note off received "+channel+", "+pitch+", "+velocity);
}

function ccEvent(channel, number, value)
{
	script.log("ControlChange received "+channel+", "+number+", "+value);
}


function ccEvent(channel, number, value)
{
	script.log("ControlChange received "+channel+", "+number+", "+value);
}


function sysExEvent(data)
{
	script.log("Sysex Message received, "+data.length+" bytes :");
	for(var i=0; i < data.length; i++)
	{
		script.log(" > "+data[i]);
	}
}

function pitchWheelEvent(channel, value) 
{
	script.log("PitchWheel received "+channel+", "+value);
}

function channelPressureEvent(channel, value) 
{
	script.log("Channel Pressure received "+channel+", "+value);
}

function afterTouchEvent(channel, note, value) 
{
	script.log("After Touch received "+channel+", "+note+", "+value);
}