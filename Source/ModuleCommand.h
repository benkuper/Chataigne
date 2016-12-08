/*
  ==============================================================================

    ModuleCommand.h
    Created: 8 Dec 2016 2:36:21pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MODULECOMMAND_H_INCLUDED
#define MODULECOMMAND_H_INCLUDED

#include "BaseCommand.h"
class Module;

class ModuleCommand :
	public BaseCommand
{
public:
	ModuleCommand(Module * module, CommandContext context, var params);
	virtual ~ModuleCommand();

	Module * module;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleCommand)
};


#endif  // MODULECOMMAND_H_INCLUDED
