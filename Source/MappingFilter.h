/*
  ==============================================================================

    MappingFilter.h
    Created: 28 Oct 2016 8:08:53pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGFILTER_H_INCLUDED
#define MAPPINGFILTER_H_INCLUDED


#include "BaseItem.h"

class MappingFilter :
	public BaseItem
{
public:
	MappingFilter(const String &name = "MappingFilter");
	virtual ~MappingFilter();

	Parameter * sourceParam;
	WeakReference<Parameter> filteredParameter;
	ControllableContainer filterParams;

	bool autoSetRange; //if true, will check at process if ranges are differents between source and filtered, and if so, will reassign

	void setupSource(Parameter * source);
	virtual Parameter * setupParameterInternal(Parameter * source);

	void controllableFeedbackUpdate(ControllableContainer *, Controllable * p) override;
	virtual void filterParamChanged(Parameter * ) {};

	Parameter * process(Parameter * source);
	virtual void processInternal() {}

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	InspectableEditor * getEditor(bool isRoot) override;

	class  FilterListener
	{
	public:
		/** Destructor. */
		virtual ~FilterListener() {}
		virtual void filteredParamChanged(MappingFilter *) {};
	};

	ListenerList<FilterListener> mappingFilterListeners;
	void addMappingFilterListener(FilterListener* newListener) { mappingFilterListeners.add(newListener); }
	void removeMappingFilterListener(FilterListener* listener) { mappingFilterListeners.remove(listener); }

	virtual String getTypeString() const { jassert(false); return "[ERROR]"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingFilter)
};




#endif  // MAPPINGFILTER_H_INCLUDED
