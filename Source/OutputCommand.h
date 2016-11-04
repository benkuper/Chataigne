/*
  ==============================================================================

    OutputCommand.h
    Created: 3 Nov 2016 10:56:27am
    Author:  bkupe

  ==============================================================================
*/

#ifndef OUTPUTCOMMAND_H_INCLUDED
#define OUTPUTCOMMAND_H_INCLUDED

#include "BaseCommand.h"
class Output;

class OutputCommand :
	public BaseCommand
{
public:
	OutputCommand(Output * output,CommandContext context, var params);
	virtual ~OutputCommand();

	Output * output;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OutputCommand)
};


#endif  // OUTPUTCOMMAND_H_INCLUDED
