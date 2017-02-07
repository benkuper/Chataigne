/*
  ==============================================================================

    ShapeShifterFactory.h
    Created: 18 May 2016 11:33:09am
    Author:  bkupe

  ==============================================================================
*/

#ifndef SHAPESHIFTERFACTORY_H_INCLUDED
#define SHAPESHIFTERFACTORY_H_INCLUDED

#include "JuceHeader.h"//keep

class ShapeShifterContent;


const static StringArray globalPanelNames = { "Modules","State Machine","Time Machine","States","Sequences","Inspector","Logger","Outliner","OSC Models"};
enum PanelName {ModulesPanel, StateMachinePanel, TimeMachinePanel, StatesPanel, SequencesPanel, InspectorPanel, LoggerPanel, OutlinerPanel, OSCCommandModelPanel};

class ShapeShifterFactory
{
public:
	static ShapeShifterContent * createContentForIndex(PanelName panelName);
	static ShapeShifterContent * createContentForName(const String &name);
};



#endif  // SHAPESHIFTERFACTORY_H_INCLUDED
