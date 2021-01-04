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
	static BaseComparator * createComparatorForControllable(Parameter * sourceParam)
	{
		switch (sourceParam->type)
		{
		case Controllable::BOOL:
			return new BoolComparator(sourceParam);
			break;

		case Controllable::FLOAT:
		case Controllable::INT:
			return new NumberComparator(sourceParam);
			break;

		case Controllable::STRING:
			return new StringComparator(sourceParam);
			break;

		case Controllable::ENUM:
			return new EnumComparator(sourceParam);
			break;

		case Controllable::POINT2D:
			return new Point2DComparator(sourceParam);
			break;

		case Controllable::POINT3D:
			return new Point3DComparator(sourceParam);
			break;
			
		default:
		    //not handled now
		    break;

		}
		return nullptr;
	}

	
};