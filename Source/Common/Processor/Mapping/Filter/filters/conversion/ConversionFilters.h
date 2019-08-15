/*
  ==============================================================================

    ConversionFilters.h
    Created: 17 Apr 2018 10:25:04am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../MappingFilter.h"

class ConversionFilter :
	public MappingFilter
{
public:
	ConversionFilter(const String &name, var params, StringRef outTypeString);
	virtual ~ConversionFilter();

	EnumParameter * retargetComponent;
	var retargetValue; //ghosting
	
	enum TransferType { DIRECT, EXTRACT, TARGET};
	TransferType transferType;

	Parameter * setupParameterInternal(Parameter * sourceParam) override;
	virtual void processInternal() override;
	virtual var convertValue(var sourceValue) { return var(sourceValue); }
};


class ToFloatFilter :
	public ConversionFilter
{
public:
	ToFloatFilter(var params);
	~ToFloatFilter() {}

	var convertValue(var sourceValue) override;

	static ToFloatFilter * create(var params) { return new ToFloatFilter(params); }
	String getTypeString() const override { return "Convert To Float"; }
};

class ToIntFilter :
	public ConversionFilter
{
public:
	ToIntFilter(var params);
	~ToIntFilter() {}

	enum Mode { FLOOR, ROUND, CEIL };
	EnumParameter * modeParam;

	var convertValue(var sourceValue) override;

	static ToIntFilter * create(var params) { return new ToIntFilter(params); }
	String getTypeString() const override { return "Convert To Integer"; }

};

class ToStringFilter :
	public ConversionFilter
{
public:
	ToStringFilter(var params);
	~ToStringFilter() {}

	IntParameter * numDecimals;
	//IntParameter * numLeadingZeros;

	var convertValue(var sourceValue) override;

	static ToStringFilter * create(var params) { return new ToStringFilter(params); }
	String getTypeString() const override { return "Convert To String"; }

};



class ToPoint2DFilter :
	public ConversionFilter
{
public:
	ToPoint2DFilter(var params);
	~ToPoint2DFilter() {}

	var convertValue(var sourceValue) override;

	static ToPoint2DFilter * create(var params) { return new ToPoint2DFilter(params); }
	String getTypeString() const override { return "Convert To Point2D"; }

};


class ToPoint3DFilter :
	public ConversionFilter
{
public:
	ToPoint3DFilter(var params);
	~ToPoint3DFilter() {}

	var convertValue(var sourceValue) override;

	static ToPoint3DFilter * create(var params) { return new ToPoint3DFilter(params); }
	String getTypeString() const override { return "Convert To Point3D"; }

};

class ToColorFilter :
	public ConversionFilter
{
public:
	ToColorFilter(var params);
	~ToColorFilter();


	enum RetargetMode { HUE = -1, SAT = -2, VAL = -3};

	ColorParameter* baseColor;

	Parameter* setupParameterInternal(Parameter* sourceParam) override;
	void processInternal() override;
	//var convertValue(var sourceValue) override;

	static ToColorFilter* create(var params) { return new ToColorFilter(params); }
	String getTypeString() const override { return "Convert To Color"; }

};