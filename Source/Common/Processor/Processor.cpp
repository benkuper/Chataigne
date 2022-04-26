/*
  ==============================================================================

	Processor.cpp
	Created: 15 Oct 2017 1:24:47pm
	Author:  Ben

  ==============================================================================
*/

Processor::Processor(const String & name, bool canBeDisabled, bool canHaveScripts) :
	BaseItem(name, canBeDisabled, canHaveScripts),
	forceDisabled(false),
	processorNotifier(10)
{
	itemDataType = "Processor";
	editorIsCollapsed = true;
	showWarningInUI = true;
}

Processor::~Processor()
{

}

void Processor::onContainerParameterChangedInternal(Parameter* p)
{
	BaseItem::onContainerParameterChangedInternal(p);
	if (p == enabled)
	{
		updateDisables();
	}
}

void Processor::setForceDisabled(bool value, bool force)
{
	if (forceDisabled == value && !force) return;
	forceDisabled = value;
	updateDisables();
	processorNotifier.addMessage(new ProcessorEvent(ProcessorEvent::FORCE_DISABLED_CHANGED, this));
}

ProcessorUI * Processor::getUI()
{
	return new ProcessorUI(this);
}

