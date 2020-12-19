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
	static BaseComparator * createComparatorForControllable(Array<WeakReference<Controllable>> sources)
	{
		jassert(sources.size() > 0);

		switch (sources[0]->type)
		{
		case Controllable::TRIGGER:
			return new TriggerComparator(sources);
			break;

		case Controllable::BOOL:
			return new BoolComparator(sources);
			break;

		case Controllable::INT:
			return new IntComparator(sources);
			break;

		case Controllable::FLOAT:
			return new FloatComparator(sources);
			break;

		case Controllable::STRING:
			return new StringComparator(sources);
			break;

		case Controllable::ENUM:
			return new EnumComparator(sources);
			break;

		case Controllable::POINT2D:
			return new Point2DComparator(sources);
			break;

		case Controllable::POINT3D:
			return new Point3DComparator(sources);
			break;
			
		default:
		    //not handled now
		    break;

		}
		return nullptr;
	}

	
};