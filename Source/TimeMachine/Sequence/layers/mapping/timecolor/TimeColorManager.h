/*
  ==============================================================================

    TimeColorManager.h
    Created: 11 Apr 2017 11:40:41am
    Author:  Ben

  ==============================================================================
*/

#ifndef TIMECOLORMANAGER_H_INCLUDED
#define TIMECOLORMANAGER_H_INCLUDED

#include "TimeColor.h"

class TimeColorComparator
{
public:
	int compareElements(TimeColor * t1, TimeColor * t2)
	{
		if (t1->position->floatValue() < t2->position->floatValue()) return -1;
		else if (t1->position->floatValue() > t2->position->floatValue()) return 1;
		return 0;
	}
};

class TimeColorManager :
	public BaseManager<TimeColor>
{
public:
	TimeColorManager(float maxPosition = 10);
	~TimeColorManager();

	ColorParameter * currentColor;
	SpinLock gradientLock;

	FloatParameter * position;
	FloatParameter * length;

	bool allowKeysOutside;
	
	void setLength(float val, bool stretch = false, bool stickToEnd = false);

	static TimeColorComparator comparator;

	Colour getColorForPosition(const float & time) const;
	
	//void rebuildGradient();
	
	TimeColor * addColorAt(float time, Colour color);
	TimeColor * getItemAt(float time, bool getNearestPreviousKeyIfNotFound = false) const;

    void addItemInternal(TimeColor * item, var data) override;
	void removeItemInternal(TimeColor * item) override;
	
	void reorderItems() override;

	void onContainerParameterChanged(Parameter *) override;
	void onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;

	void loadJSONDataInternal(var data) override;

	class TimeColorManagerListener
	{
	public:
		virtual ~TimeColorManagerListener() {}
		virtual void gradientUpdated() {}
	};

	ListenerList<TimeColorManagerListener> colorManagerListeners;
	void addColorManagerListener(TimeColorManagerListener* newListener) { colorManagerListeners.add(newListener); }
	void removeColorManagerListener(TimeColorManagerListener* listener) { colorManagerListeners.remove(listener); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeColorManager)
};




#endif  // TIMECOLORMANAGER_H_INCLUDED
