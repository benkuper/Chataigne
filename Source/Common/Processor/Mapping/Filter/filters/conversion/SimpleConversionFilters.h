/*
  ==============================================================================

    ConversionFilters.h
    Created: 17 Apr 2018 10:25:04am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class SimpleConversionFilter :
	public MappingFilter
{
public:
	SimpleConversionFilter(const String &name, var params, StringRef outTypeString, Multiplex* multiplex);
	virtual ~SimpleConversionFilter();

	var retargetValues; //ghosting
	String outTypeString;

	var ghostOptions;
	var ghostLinkData;
	bool autoLoadDataOnSetup;

	EnumParameter* retargetComponent;
	bool useBaseValue;
	Parameter * baseValue;

	var convertModeData; //ghost loading

	var getJSONData() override;
	void loadJSONDataItemInternal(var data) override;
	
	enum TransferType { DIRECT, EXTRACT, MERGE, TARGET};
	TransferType transferType;

	enum ExtractOption { MIN = 100, MAX = 101, AVERAGE = 102, LENGTH = 103, AREA = 104 };

	virtual void setupParametersInternal(int mutiplexIndex, bool rangeOnly = false) override;
	virtual Parameter* setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly) override;
	virtual void addExtraRetargetOptions() {}

	void filterParamChanged(Parameter * p) override;

	void updateOutRange(Parameter * source, Parameter * out);

	virtual ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;
	virtual var convertValue(Parameter * source, var sourceValue, int multiplexIndex) { return var(sourceValue) ; }
};

class ToBooleanFilter :
	public SimpleConversionFilter
{
public:
	ToBooleanFilter(var params, Multiplex* multiplex);
	~ToBooleanFilter() {}

	BoolParameter* toggleMode;

	virtual ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;

	String getTypeString() const override { return "Convert To Boolean"; }
};

class ToFloatFilter :
	public SimpleConversionFilter
{
public:
	ToFloatFilter(var params, Multiplex* multiplex);
	~ToFloatFilter() {}
	
	var convertValue(Parameter * source, var sourceValue, int multiplexIndex) override;

	String getTypeString() const override { return "Convert To Float"; }
};

class ToIntFilter :
	public SimpleConversionFilter
{
public:
	ToIntFilter(var params, Multiplex* multiplex);
	~ToIntFilter() {}

	enum Mode { FLOOR, ROUND, CEIL };
	EnumParameter * modeParam;

	var convertValue(Parameter * source, var sourceValue, int multiplexIndex) override;

	String getTypeString() const override { return "Convert To Integer"; }

};

class ToStringFilter :
	public SimpleConversionFilter
{
public:
	ToStringFilter(var params, Multiplex* multiplex);
	~ToStringFilter() {}

	enum Format { NUMBER, TIME, HEXA };
	EnumParameter* format;
	IntParameter* numDecimals;
	IntParameter* fixedLeading;

	enum Case { UPPER, LOWER, LCAMEL, UCAMEL };
	EnumParameter * forceCase;
	
	StringParameter* prefix;
	StringParameter* suffix;
	enum ConvertMode { KEY, VALUE };
	EnumParameter * enumConvertMode;

	void setupParametersInternal(int multiplexIndex, bool rangeOnly) override;
	var convertValue(Parameter * source, var sourceValue, int multiplexIndex) override;
	String getCasedString(const String& value);

	var getJSONData() override;

	void filterParamChanged(Parameter*) override;

	String getTypeString() const override { return "Convert To String"; }
};



class ToPoint2DFilter :
	public SimpleConversionFilter
{
public:
	ToPoint2DFilter(var params, Multiplex* multiplex);
	~ToPoint2DFilter() {}

	Point2DParameter* baseValue;

	var convertValue(Parameter * source, var sourceValue, int multiplexIndex) override;

	String getTypeString() const override { return "Convert To Point2D"; }

};


class ToPoint3DFilter :
	public SimpleConversionFilter
{
public:
	ToPoint3DFilter(var params, Multiplex* multiplex);
	~ToPoint3DFilter() {}


	var convertValue(Parameter * source, var sourceValue, int multiplexIndex) override;

	String getTypeString() const override { return "Convert To Point3D"; }

};

class ToColorFilter :
	public SimpleConversionFilter
{
public:
	ToColorFilter(var params, Multiplex* multiplex);
	~ToColorFilter();


	enum RetargetMode { HUE = -1, SAT = -2, VAL = -3};
	ColorParameter* baseColor;

	var getJSONData() override;

	void setupParametersInternal(int multiplexIndex, bool rangeOnly) override;
	virtual void addExtraRetargetOptions() override;
	ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;


	String getTypeString() const override { return "Convert To Color"; }

};
