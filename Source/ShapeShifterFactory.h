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

const static StringArray globalPanelNames = { "Inputs","Outputs","State Machine","Time Machine","Inspector","Logger"};
enum PanelName { InputsPanel,OutputsPanel,StateMachinePanel,TimeMachinePanel, InspectorPanel, LoggerPanel};

class ShapeShifterFactory
{
public:
	static ShapeShifterContent * createContentForIndex(PanelName panelName);
	static ShapeShifterContent * createContentForName(String name);
};



#endif  // SHAPESHIFTERFACTORY_H_INCLUDED
