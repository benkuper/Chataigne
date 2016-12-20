/*
 ==============================================================================

 Parameter.h
 Created: 8 Mar 2016 1:08:19pm
 Author:  bkupe

 ==============================================================================
 */

#ifndef PARAMETER_H_INCLUDED
#define PARAMETER_H_INCLUDED

#include "Controllable.h"
#include "QueuedNotifier.h"

class Parameter : public Controllable
{
public:
    Parameter(const Type &type, const String & niceName, const String &description, var initialValue, var minValue, var maxValue, bool enabled = true);
    virtual ~Parameter() {Parameter::masterReference.clear();}


    var defaultValue;
    var value;
    var lastValue;
    var minimumValue;
    var maximumValue;

	var defaultMinValue;//for autoAdaptRange
	var defaultMaxValue;
   
	void setRange(var,var, bool setDefaultRange = true);


	bool isEditable; 
	bool isPresettable;
    bool isOverriden;
	bool autoAdaptRange;

    void resetValue(bool silentSet = false);
    virtual void setValue(var _value, bool silentSet = false, bool force = false);
    virtual void setValueInternal(var & _value);

	virtual bool checkValueIsTheSame(var newValue, var oldValue); //can be overriden to modify check behavior

    //For Number type parameters
    void setNormalizedValue(const float &normalizedValue, bool silentSet = false, bool force = false);
    float getNormalizedValue();

    //helpers for fast typing
    virtual float floatValue() { return (float)value; }
	virtual double doubleValue(){return (double)value;}
	virtual int intValue() { return (int)value; }
	virtual bool boolValue() { return (bool)value; }
	virtual String stringValue() { return value.toString(); }

    void notifyValueChanged();


	virtual var getJSONDataInternal() override;
	virtual void loadJSONDataInternal(var data) override;
	
	//virtual DynamicObject * createDynamicObject() override;

    //Listener
	class  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}
		virtual void parameterValueChanged(Parameter * p) = 0;
		virtual void parameterRangeChanged(Parameter * ){};
    };

    ListenerList<Listener> listeners;
    void addParameterListener(Listener* newListener) { listeners.add(newListener); }
    void removeParameterListener(Listener* listener) { listeners.remove(listener); }
	


    // ASYNC
    class  ParamWithValue{
    public:
		enum Type { VALUE, RANGE };

		ParamWithValue(Parameter * p,var v,Type t):parameter(p),value(v),type(t){}
		Type type;

        Parameter * parameter;
        var value;
		bool isRange() const { return type == RANGE; }

    };
    QueuedNotifier<ParamWithValue> queuedNotifier;
    typedef QueuedNotifier<ParamWithValue>::Listener AsyncListener;


    void addAsyncParameterListener(AsyncListener* newListener) { queuedNotifier.addListener(newListener); }
    void addAsyncCoalescedListener(AsyncListener* newListener) { queuedNotifier.addAsyncCoalescedListener(newListener); }
    void removeAsyncParameterListener(AsyncListener* listener) { queuedNotifier.removeListener(listener); }


	//JS Helper
	//static var getValue(const juce::var::NativeFunctionArgs &a);

private:



    void checkVarIsConsistentWithType();

    WeakReference<Parameter>::Master masterReference;
    friend class WeakReference<Parameter>;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Parameter)

};


#endif  // PARAMETER_H_INCLUDED
