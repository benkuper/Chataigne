/*
  ==============================================================================

    TimeModule.cpp
    Created: 4 Jan 2018 5:02:32pm
    Author:  Ben

  ==============================================================================
*/

#include "TimeModule.h"

TimeModule::TimeModule(const String & name) :
	Module(name)
{
	setupIOConfiguration(false, false);

	year = valuesCC.addIntParameter("Year", "Current year", 2000, 2000, 5000);
	monthName = valuesCC.addEnumParameter("Month name", "Name of the current month");
	monthName->addOption("January", JANUARY)->addOption("February", FEBRUARY)->addOption("March", MARCH)->addOption("April", APRIL)->addOption("May", MAY)->addOption("June", JUNE)->addOption("July", JULY)->addOption("August", AUGUST)->addOption("September", SEPTEMBER)->addOption("October", OCTOBER)->addOption("November", NOVEMBER)->addOption("December", DECEMBER);
	month = valuesCC.addIntParameter("Month", "Number of the month (1 = January, 12 = December)", 1, 1, 12);
	monthDay = valuesCC.addIntParameter("Month day", "Day relative to the current month (1 > 31)", 1, 1, 31);
	weekDayName = valuesCC.addEnumParameter("Week Day Name", "Name of the day in the week");
	weekDayName->addOption("Monday", MONDAY)->addOption("Tuesday", TUESDAY)->addOption("Wednesday", WEDNESDAY)->addOption("Thursday", THURSDAY)->addOption("Friday", FRIDAY)->addOption("Saturday", SATURDAY)->addOption("Sunday", SUNDAY);
	weekDay = valuesCC.addIntParameter("Week day", "Day relative to the current week (Monday = 1, Sunday = 7)", 1, 1, 7);

	hour = valuesCC.addIntParameter("Hour", "Current hour relative to the day (0 > 23)", 0, 0, 23);
	minutes = valuesCC.addIntParameter("Minutes", "Current minute relative to the current hour (0 > 59)", 0, 0, 59);
	seconds = valuesCC.addIntParameter("Seconds", "Current second relative to the current minute (0 > 59)", 0, 0, 59);
	
	moduleParams.hideInEditor = true;
	for (auto &c : valuesCC.controllables) c->isControllableFeedbackOnly = true;

	startTimer(1000);
	timerCallback(); //force one
}

TimeModule::~TimeModule()
{

}

void TimeModule::timerCallback()
{
	Time time = Time::getCurrentTime();
	year->setValue(time.getYear());
	monthName->setValueWithData((Month)time.getMonth());
	month->setValue(time.getMonth() + 1);
	monthDay->setValue(time.getDayOfMonth());
	int wDay = (time.getDayOfWeek() +6) % 7;
	weekDayName->setValueWithData((Day)wDay);
	weekDay->setValue(wDay + 1);
	hour->setValue(time.getHours());
	minutes->setValue(time.getMinutes());
	seconds->setValue(time.getSeconds());
}
