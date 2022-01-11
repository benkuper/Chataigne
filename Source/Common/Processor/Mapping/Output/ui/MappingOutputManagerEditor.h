/*
  ==============================================================================

    MappingOutputManagerEditor.h
    Created: 31 May 2018 7:55:02am
    Author:  Ben

  ==============================================================================
*/

#pragma once


class MappingOutputManagerEditor :
	public BaseCommandHandlerManagerEditor<MappingOutput>,
	public MappingOutputManager::AsyncListener
{
public:
	MappingOutputManagerEditor(MappingOutputManager * output, bool isRoot);
	~MappingOutputManagerEditor();

	MappingOutputManager * outputManager;
	std::unique_ptr<ParameterUI> outUI;

	void updateOutputUI();

	void resizedInternalHeader(Rectangle<int> &r) override;

	void newMessage(const MappingOutputManager::OutputManagerEvent &e) override;
};