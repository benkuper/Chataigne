/*
  ==============================================================================

    ConversionFilters.h
    Created: 17 Apr 2018 10:25:04am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../MappingFilter.h"
 

class SimpleConversionFilter :
	public MappingFilter
{
public:
	SimpleConversionFilter(const String &name, var params, StringRef outTypeString);
	virtual ~SimpleConversionFilter();

	EnumParameter * retargetComponent;
	var retargetValues; //ghosting
	String outTypeString;
	
	enum TransferType { DIRECT, EXTRACT, TARGET};
	TransferType transferType;

	virtual Parameter * setupSingleParameterInternal(Parameter* source) override;
	virtual bool processSingleParameterInternal(Parameter* source, Parameter* out) override;
	virtual var convertValue(Parameter * source, var sourceValue) { return var(sourceValue) ; }
};

class ToBooleanFilter :
	public SimpleConversionFilter
{
public:
	ToBooleanFilter(var params);
	~ToBooleanFilter() {}

	BoolParameter* toggleMode;

	virtual bool processSingleParameterInternal(Parameter* source, Parameter* out) override;

	static ToBooleanFilter* create(var params) { return new ToBooleanFilter(params); }
	String getTypeString() const override { return "Convert To Boolean"; }
};

class ToFloatFilter :
	public SimpleConversionFilter
{
public:
	ToFloatFilter(var params);
	~ToFloatFilter() {}

	var convertValue(Parameter * source, var sourceValue) override;

	static ToFloatFilter * create(var params) { return new ToFloatFilter(params); }
	String getTypeString() const override { return "Convert To Float"; }
};

class ToIntFilter :
	public SimpleConversionFilter
{
public:
	ToIntFilter(var params);
	~ToIntFilter() {}

	enum Mode { FLOOR, ROUND, CEIL };
	EnumParameter * modeParam;

	var convertValue(Parameter * source, var sourceValue) override;

	static ToIntFilter * create(var params) { return new ToIntFilter(params); }
	String getTypeString() const override { return "Convert To Integer"; }

};

class ToStringFilter :
	public SimpleConversionFilter
{
public:
	ToStringFilter(var params);
	~ToStringFilter() {}

	enum Format { NUMBER, TIME };
	EnumParameter* format;
	IntParameter * numDecimals;
	StringParameter* prefix;
	StringParameter* suffix;
	//IntParameter * numLeadingZeros;

	var convertValue(Parameter * source, var sourceValue) override;

	void filterParamChanged(Parameter*) override;

	static ToStringFilter * create(var params) { return new ToStringFilter(params); }
	String getTypeString() const override { return "Convert To String"; }

};



class ToPoint2DFilter :
	public SimpleConversionFilter
{
public:
	ToPoint2DFilter(var params);
	~ToPoint2DFilter() {}

	var convertValue(Parameter * source, var sourceValue) override;

	static ToPoint2DFilter * create(var params) { return new ToPoint2DFilter(params); }
	String getTypeString() const override { return "Convert To Point2D"; }

};


class ToPoint3DFilter :
	public SimpleConversionFilter
{
public:
	ToPoint3DFilter(var params);
	~ToPoint3DFilter() {}

	var convertValue(Parameter * source, var sourceValue) override;

	static ToPoint3DFilter * create(var params) { return new ToPoint3DFilter(params); }
	String getTypeString() const override { return "Convert To Point3D"; }

};

class ToColorFilter :
	public SimpleConversionFilter
{
public:
	ToColorFilter(var params);
	~ToColorFilter();


	enum RetargetMode { HUE = -1, SAT = -2, VAL = -3};

	ColorParameter* baseColor;

	Parameter* setupSingleParameterInternal(Parameter* sourceParam) override;
	bool processSingleParameterInternal(Parameter * source, Parameter * out) override;

	static ToColorFilter* create(var params) { return new ToColorFilter(params); }
	String getTypeString() const override { return "Convert To Color"; }

};