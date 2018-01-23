/*
  ==============================================================================

    TimeModule.h
    Created: 4 Jan 2018 5:02:32pm
    Author:  Ben

  ==============================================================================
*/

#pragma once
#include "Module.h"

class TimeModule :
	public Module,
	public Timer
{
public: 
	TimeModule(const String &name = "Time");
	~TimeModule();

	enum Month { JANUARY, FEBRUARY, MARCH, APRIL, MAY, JUNE, JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER};
	enum Day { MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY };

	IntParameter * year;
	EnumParameter * monthName;
	IntParameter * month;
	IntParameter * monthDay;
	EnumParameter * weekDayName;
	IntParameter * weekDay;
	IntParameter * hour;
	IntParameter * minutes;
	IntParameter * seconds;
	FloatParameter * dayTime;

	virtual String getDefaultTypeString() const override { return "Time"; }
	static TimeModule * create() { return new TimeModule(); }

	// Inherited via Timer
	virtual void timerCallback() override;
};