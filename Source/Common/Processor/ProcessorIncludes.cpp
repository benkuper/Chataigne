/*
  ==============================================================================

    ProcessorIncludes.cpp
    Created: 10 Mar 2021 2:03:55pm
    Author:  bkupe

  ==============================================================================
*/

#include "ProcessorIncludes.h"

#include "MainIncludes.h"

#include "Multiplex/List/MultiplexList.cpp"
#include "Multiplex/List/MultiplexListManager.cpp"
#include "Multiplex/List/ui/MultiplexListEditor.cpp"

#include "Multiplex/MultiplexTarget.cpp"
#include "Multiplex/Multiplex.cpp"
#include "Multiplex/ui/MultiplexUI.cpp"

#include "Action/Action.cpp"
#include "Action/Condition/Condition.cpp"
#include "Action/Condition/ConditionManager.cpp"

#include "Action/Condition/conditions/ActivationCondition/ActivationCondition.cpp"
#include "Action/Condition/conditions/ConditionGroup/ConditionGroup.cpp"
#include "Action/Condition/conditions/ScriptCondition/ScriptCondition.cpp"
#include "Action/Condition/conditions/ManualCondition/ManualCondition.cpp"

#include "Action/Condition/conditions/StandardCondition/Comparator/BaseComparator.cpp"
#include "Action/Condition/conditions/StandardCondition/Comparator/comparators/BoolComparators.cpp"
#include "Action/Condition/conditions/StandardCondition/Comparator/comparators/EnumComparator.cpp"
#include "Action/Condition/conditions/StandardCondition/Comparator/comparators/NumberComparators.cpp"
#include "Action/Condition/conditions/StandardCondition/Comparator/comparators/Point2DComparators.cpp"
#include "Action/Condition/conditions/StandardCondition/Comparator/comparators/Point3DComparators.cpp"
#include "Action/Condition/conditions/StandardCondition/Comparator/comparators/StringComparators.cpp"
#include "Action/Condition/conditions/StandardCondition/Comparator/ui/BaseComparatorUI.cpp"

#include "Action/Condition/conditions/StandardCondition/StandardCondition.cpp"
#include "Action/Condition/conditions/StandardCondition/ui/StandardConditionEditor.cpp"

#include "Action/Condition/conditions/MultiplexIndex/MultiplexIndexCondition.cpp"
#include "Action/Condition/conditions/MultiplexIndex/ui/MultiplexIndexConditionEditor.cpp"


#include "Action/Condition/ui/ConditionEditor.cpp"
#include "Action/Condition/ui/ConditionManagerEditor.cpp"
#include "Action/Consequence/Consequence.cpp"
#include "Action/Consequence/ConsequenceManager.cpp"
#include "Action/Consequence/ConsequenceGroup.cpp"
#include "Action/Consequence/ui/ConsequenceManagerEditor.cpp"
#include "Action/ui/ActionUI.cpp"

#include "Processor.cpp"
#include "ProcessorManager.cpp"
#include "ui/ProcessorManagerUI.cpp"
#include "ui/ProcessorUI.cpp"
