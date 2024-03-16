/*
  ==============================================================================

    ProcessorIncludes.h
    Created: 10 Mar 2021 2:03:55pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "Processor.h"
#include "ProcessorManager.h"

#include "Multiplex/MultiplexTarget.h"
#include "Multiplex/List/MultiplexList.h"
#include "Multiplex/List/MultiplexListManager.h"
#include "Multiplex/Multiplex.h"

#include "Common/Command/ui/BaseCommandHandlerManagerEditor.h"

#include "Action/Condition/Condition.h"
#include "Action/Condition/ConditionManagerListener.h"
#include "Action/Condition/ConditionManager.h"

#include "Action/Condition/conditions/ActivationCondition/ActivationCondition.h"
#include "Action/Condition/conditions/ConditionGroup/ConditionGroup.h"
#include "Action/Condition/conditions/ScriptCondition/ScriptCondition.h"
#include "Action/Condition/conditions/ManualCondition/ManualCondition.h"

#include "Action/Condition/conditions/StandardCondition/Comparator/BaseComparator.h"

#include "Action/Condition/conditions/StandardCondition/Comparator/comparators/BoolComparators.h"
#include "Action/Condition/conditions/StandardCondition/Comparator/comparators/EnumComparator.h"
#include "Action/Condition/conditions/StandardCondition/Comparator/comparators/NumberComparators.h"
#include "Action/Condition/conditions/StandardCondition/Comparator/comparators/Point2DComparators.h"
#include "Action/Condition/conditions/StandardCondition/Comparator/comparators/Point3DComparators.h"
#include "Action/Condition/conditions/StandardCondition/Comparator/comparators/StringComparators.h"

#include "Action/Condition/conditions/StandardCondition/Comparator/ComparatorFactory.h"


#include "Action/Condition/conditions/StandardCondition/StandardCondition.h"
#include "Action/Condition/conditions/MultiplexIndex/MultiplexIndexCondition.h"

#include "Action/Consequence/Consequence.h"
#include "Action/Consequence/ConsequenceManager.h"
#include "Action/Consequence/ConsequenceGroup.h"
#include "Action/Consequence/ui/ConsequenceManagerEditor.h"


#include "Action/Action.h"

#include "Mapping/Input/MappingInput.h"
#include "Mapping/Input/MappingInputManager.h"

#include "Mapping/Filter/MappingFilter.h"
#include "Mapping/Filter/MappingFilterManager.h"

#include "Mapping/Output/MappingOutput.h"
#include "Mapping/Output/MappingOutputManager.h"

#include "Mapping/Mapping.h"

#include "Mapping/Filter/filters/ScriptFilter.h"
#include "Mapping/Filter/filters/color/ColorShiftFilter.h"
#include "Mapping/Filter/filters/color/ColorRemapFilter.h"
#include "Mapping/Filter/filters/condition/ConditionFilter.h"
#include "Mapping/Filter/filters/conversion/SimpleConversionFilters.h"

#include "Mapping/Filter/filters/conversion/ConversionParamValueLink.h"
#include "Mapping/Filter/filters/conversion/ConvertedParameter.h"
#include "Mapping/Filter/filters/conversion/ConvertedParameterManager.h"
#include "Mapping/Filter/filters/conversion/ConversionFilter.h"
#include "Mapping/Filter/filters/conversion/MergeFilter.h"

#include "Mapping/Filter/filters/TimeFilter.h"
#include "Mapping/Filter/filters/number/CropFilter.h"
#include "Mapping/Filter/filters/number/DampingFilter.h"
#include "Mapping/Filter/filters/number/SpeedFilter.h"
#include "Mapping/Filter/filters/number/FreezeFilter.h"
#include "Mapping/Filter/filters/number/InverseFilter.h"
#include "Mapping/Filter/filters/number/LagFilter.h"
#include "Mapping/Filter/filters/DelayFilter.h"
#include "Mapping/Filter/filters/number/MathFilter.h"
#include "Mapping/Filter/filters/number/SimpleRemapFilter.h"
#include "Mapping/Filter/filters/number/CurveMapFilter.h"
#include "Mapping/Filter/filters/number/SimpleSmoothFilter.h"
#include "Mapping/Filter/filters/number/OneEuroFilter.h"

#include "Mapping/Filter/filters/string/StringFilter.h"


#include "Mapping/Filter/ui/MappingFilterEditor.h"

#include "Mapping/Filter/filters/conversion/ui/ConversionConnector.h"
#include "Mapping/Filter/filters/conversion/ui/ConversionSourceParameterUI.h"
#include "Mapping/Filter/filters/conversion/ui/ConversionParamValueLinkUI.h"
#include "Mapping/Filter/filters/conversion/ui/ConvertedParameterEditor.h"
#include "Mapping/Filter/filters/conversion/ui/ConvertedParameterManagerEditor.h"
#include "Mapping/Filter/filters/conversion/ui/ConversionFilterEditor.h"

#include "Mapping/Input/ui/MappingInputEditor.h"
#include "Mapping/Output/ui/MappingOutputManagerEditor.h"

#include "Action/Condition/ui/ConditionEditor.h"
#include "Action/Condition/ui/ConditionManagerEditor.h"

#include "Action/Condition/conditions/StandardCondition/Comparator/ui/BaseComparatorUI.h"
#include "Action/Condition/conditions/StandardCondition/ui/StandardConditionEditor.h"

#include "Action/Condition/conditions/MultiplexIndex/ui/MultiplexIndexConditionEditor.h"


#include "ui/ProcessorUI.h"
#include "ui/ProcessorManagerUI.h"

#include "Action/ui/ActionUI.h"
#include "Mapping/ui/MappingUI.h"

#include "Multiplex/List/ui/MultiplexListEditor.h"
#include "Multiplex/ui/MultiplexUI.h"

#include "Conductor/Conductor.h"
#include "Conductor/ConductorCue.h"
#include "Conductor/ui/ConductorUI.h"
#include "Conductor/ui/ConductorCueUI.h"