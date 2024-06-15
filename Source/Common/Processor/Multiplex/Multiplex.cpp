/*
  ==============================================================================

	Multiplex.cpp
	Created: 17 Dec 2020 5:12:36pm
	Author:  bkupe

  ==============================================================================
*/

Multiplex::Multiplex(var params) :
	Processor(getTypeString()),
	isChangingCount(false),
	listManager(this),
	processorManager("Processors", this)

{
	type = MULTIPLEX;

	count = addIntParameter("Count", "The number of items for each list of this multiplex", 1, 0);
	previewIndex = addIntParameter("Preview Index", "The index (1-N) of this multiplex to preview. The parameters shown in the mapping UI and inspector, filter and outputs are changed when you changed this value.", 1, 1, count->intValue());

	itemColor->setDefaultValue(Colours::deepskyblue.darker().withSaturation(.4f));

	addChildControllableContainer(&listManager);

	processorManager.hideInEditor = true;
	addChildControllableContainer(&processorManager);

	saveAndLoadRecursiveData = true;
}

Multiplex::~Multiplex()
{
	DBG("Multiplex clear");
}

void Multiplex::onContainerParameterChangedInternal(Parameter* p)
{
	Processor::onContainerParameterChangedInternal(p);
	if (p == count)
	{
		isChangingCount = true;
		for (auto& l : listManager.items) l->setSize(count->intValue());
		previewIndex->setRange(1, count->intValue());
		multiplexListeners.call(&MultiplexListener::multiplexCountChanged);
		isChangingCount = false;
	}
	else if (p == previewIndex)
	{
		multiplexListeners.call(&MultiplexListener::multiplexPreviewIndexChanged);
	}
}

void Multiplex::updateDisables(bool force, bool fromActivation)
{
	processorManager.setForceDisabled(!enabled->boolValue() || forceDisabled, force, fromActivation);
}

void Multiplex::showAndGetList(ControllableContainer* startFromCC, std::function<void(ControllableContainer*)> returnFunc)
{
	PopupMenu p;
	for (int i = 0; i < listManager.items.size(); i++) p.addItem(i + 1, listManager.items[i]->niceName);

	p.showMenuAsync(PopupMenu::Options(), [this, returnFunc](int result)
		{
			if (result == 0) return;
			returnFunc(listManager.items[result - 1]);
		}
	);
}

ProcessorUI* Multiplex::getUI()
{
	return new MultiplexUI(this);
}
