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

	EnumParameter * retargetComponent;
	var retargetValues; //ghosting
	String outTypeString;


	var ghostOptions;

	var getJSONData() override;
	void loadJSONDataItemInternal(var data) override;
	
	enum TransferType { DIRECT, EXTRACT, TARGET};
	TransferType transferType;

	virtual Parameter* setupSingleParameterInternal(Parameter* source, int multiplexIndex) override;
	virtual ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;
	virtual var convertValue(Parameter * source, var sourceValue) { return var(sourceValue) ; }
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

	virtual Parameter * setupSingleParameterInternal(Parameter* source, int multiplexIndex) override;
	var convertValue(Parameter * source, var sourceValue) override;

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

	var convertValue(Parameter * source, var sourceValue) override;

	String getTypeString() const override { return "Convert To Integer"; }

};

class ToStringFilter :
	public SimpleConversionFilter
{
public:
	ToStringFilter(var params, Multiplex* multiplex);
	~ToStringFilter() {}

	enum Format { NUMBER, TIME };
	EnumParameter* format;
	IntParameter* numDecimals;
	IntParameter * fixedLeading;
	StringParameter* prefix;
	StringParameter* suffix;
	//IntParameter * numLeadingZeros;

	var convertValue(Parameter * source, var sourceValue) override;

	void filterParamChanged(Parameter*) override;

	String getTypeString() const override { return "Convert To String"; }
};



class ToPoint2DFilter :
	public SimpleConversionFilter
{
public:
	ToPoint2DFilter(var params, Multiplex* multiplex);
	~ToPoint2DFilter() {}

	var convertValue(Parameter * source, var sourceValue) override;

	String getTypeString() const override { return "Convert To Point2D"; }

};


class ToPoint3DFilter :
	public SimpleConversionFilter
{
public:
	ToPoint3DFilter(var params, Multiplex* multiplex);
	~ToPoint3DFilter() {}

	var convertValue(Parameter * source, var sourceValue) override;

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

	Parameter* setupSingleParameterInternal(Parameter* sourceParam, int multiplexIndex) override;
	ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;


	String getTypeString() const override { return "Convert To Color"; }

};