/*
  ==============================================================================

    Output.h
    Created: 28 Oct 2016 8:02:31pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

#include "BaseItem.h"
class CommandDefinition;

class Output :
	public BaseItem
{
public:
	Output(const String &name = "Output");
	virtual ~Output();

	OwnedArray<CommandDefinition> commandDefs;
	
	PopupMenu commandMenu;
	void rebuildCommandMenu(int baseID);
	PopupMenu getCommandMenu(int baseID);
	CommandDefinition * getCommandDefinitionFor(const String &menuPath, const String &inputType);

	var getJSONData() override;

	virtual String getTypeString() const { jassert(false); return "Output"; } //should always overriden

	class  OutputListener
	{
	public:
		/** Destructor. */
		virtual ~OutputListener() {}
		virtual void outputCommandChanged(Output *) {}
	};

	ListenerList<OutputListener> outputListeners;
	void addOutputListener(OutputListener* newListener) { outputListeners.add(newListener); }
	void removeOutputListener(OutputListener* listener) { outputListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Output)
};

#endif  // OUTPUT_H_INCLUDED
