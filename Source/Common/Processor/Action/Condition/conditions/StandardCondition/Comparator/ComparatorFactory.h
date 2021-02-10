/*
  ==============================================================================

    ComparatorFactory.h
    Created: 2 Nov 2016 10:54:37pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "BaseComparator.h"
#include "comparators/BoolComparators.h"
#include "comparators/NumberComparators.h"
#include "comparators/StringComparators.h"
#include "comparators/EnumComparator.h"
#include "comparators/Point2DComparators.h"
#include "comparators/Point3DComparators.h"

class ComparatorFactory
{
public:
	static BaseComparator * createComparatorForControllable(Parameter * sourceParam, Multiplex * multiplex)
	{
		BaseComparator* result = nullptr;
		switch (sourceParam->type)
		{
		case Controllable::BOOL:
			result = new BoolComparator(sourceParam, multiplex);
			break;

		case Controllable::FLOAT:
		case Controllable::INT:
			result = new NumberComparator(sourceParam, multiplex);
			break;

		case Controllable::STRING:
			result = new StringComparator(sourceParam, multiplex);
			break;

		case Controllable::ENUM:
			result = new EnumComparator(sourceParam, multiplex);
			break;

		case Controllable::POINT2D:
			result = new Point2DComparator(sourceParam, multiplex);
			break;

		case Controllable::POINT3D:
			result = new Point3DComparator(sourceParam, multiplex);
			break;
			
		default:
		    //not handled now
		    break;

		}

		return result;
	}

	
};