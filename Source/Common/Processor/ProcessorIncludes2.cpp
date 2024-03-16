/*
  ==============================================================================

    ProcessorIncludes2.cpp
    Created: 10 Mar 2021 3:01:19pm
    Author:  bkupe

  ==============================================================================
*/

#include "ProcessorIncludes.h"
#include "MainIncludes.h"

#include "Mapping/Filter/MappingFilter.cpp"
#include "Mapping/Filter/MappingFilterManager.cpp"
#include "Mapping/Filter/filters/ScriptFilter.cpp"

#include "Mapping/Filter/filters/color/ColorShiftFilter.cpp"
#include "Mapping/Filter/filters/color/ColorRemapFilter.cpp"

#include "Mapping/Filter/filters/condition/ConditionFilter.cpp"
#include "Mapping/Filter/filters/conversion/ConversionFilter.cpp"
#include "Mapping/Filter/filters/conversion/ConversionParamValueLink.cpp"
#include "Mapping/Filter/filters/conversion/ConvertedParameter.cpp"
#include "Mapping/Filter/filters/conversion/ConvertedParameterManager.cpp"
#include "Mapping/Filter/filters/conversion/SimpleConversionFilters.cpp"
#include "Mapping/Filter/filters/conversion/MergeFilter.cpp"
#include "Mapping/Filter/filters/conversion/ui/ConversionConnector.cpp"
#include "Mapping/Filter/filters/conversion/ui/ConversionFilterEditor.cpp"
#include "Mapping/Filter/filters/conversion/ui/ConversionParamValueLinkUI.cpp"
#include "Mapping/Filter/filters/conversion/ui/ConversionSourceParameterUI.cpp"
#include "Mapping/Filter/filters/conversion/ui/ConvertedParameterEditor.cpp"
#include "Mapping/Filter/filters/conversion/ui/ConvertedParameterManagerEditor.cpp"
#include "Mapping/Filter/filters/TimeFilter.cpp"
#include "Mapping/Filter/filters/number/CropFilter.cpp"
#include "Mapping/Filter/filters/number/CurveMapFilter.cpp"
#include "Mapping/Filter/filters/number/DampingFilter.cpp"
#include "Mapping/Filter/filters/number/SpeedFilter.cpp"
#include "Mapping/Filter/filters/number/FreezeFilter.cpp"
#include "Mapping/Filter/filters/number/InverseFilter.cpp"
#include "Mapping/Filter/filters/number/LagFilter.cpp"
#include "Mapping/Filter/filters/DelayFilter.cpp"
#include "Mapping/Filter/filters/number/MathFilter.cpp"
#include "Mapping/Filter/filters/number/SimpleRemapFilter.cpp"
#include "Mapping/Filter/filters/number/SimpleSmoothFilter.cpp"
#include "Mapping/Filter/filters/number/OneEuroFilter.cpp"

#include "Mapping/Filter/filters/string/StringFilter.cpp"

#include "Mapping/Filter/ui/MappingFilterEditor.cpp"
#include "Mapping/Input/MappingInput.cpp"
#include "Mapping/Input/MappingInputManager.cpp"
#include "Mapping/Input/ui/MappingInputEditor.cpp"
#include "Mapping/Mapping.cpp"
#include "Mapping/Output/MappingOutput.cpp"
#include "Mapping/Output/MappingOutputManager.cpp"
#include "Mapping/Output/ui/MappingOutputManagerEditor.cpp"
#include "Mapping/ui/MappingUI.cpp"

#include "Conductor/Conductor.cpp"
#include "Conductor/ConductorCue.cpp"
#include "Conductor/ui/ConductorUI.cpp"
#include "Conductor/ui/ConductorCueUI.cpp"