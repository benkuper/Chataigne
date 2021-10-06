/*
  ==============================================================================

	MappingFilterEditor.h
	Created: 4 Feb 2017 8:31:15pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

class ChannelListModel :
	public ListBoxModel
{
public:
	ChannelListModel(MappingFilter * f);
	~ChannelListModel();

	MappingFilter* filter;

	void selectedRowsChanged(int lastRowSelected) override;

	virtual int getNumRows() override;
	virtual void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) override;

	class  ModelListener
	{
	public:
		/** Destructor. */
		virtual ~ModelListener() {}
		virtual void selectionChanged() {}
	};

	ListenerList<ModelListener> modelListeners;
	void addModelListener(ModelListener* newListener) { modelListeners.add(newListener); }
	void removeModelListener(ModelListener* listener) { modelListeners.remove(listener); }
};


class ChannelSelectorUI :
	public TextButton,
	public ChannelListModel::ModelListener
{
public:
	ChannelSelectorUI(MappingFilter * f);
	~ChannelSelectorUI();

	MappingFilter * filter;
	ListBox* listBox;
	ChannelListModel model;

	void clicked() override;

	void selectionChanged() override;
};


class MappingFilterEditor :
	public BaseItemEditor,
	public MappingFilter::AsyncFilterListener
{
public:
	MappingFilterEditor(MappingFilter* m, bool isRoot);
	~MappingFilterEditor();

	MappingFilter* filter;
	std::unique_ptr<ParameterUI> filteredUI;
	std::unique_ptr<ChannelSelectorUI> channelEdit;

	void resizedInternalHeaderItemInternal(Rectangle<int>& r) override;

	void updateFilteredUI();

	virtual void newMessage(const MappingFilter::FilterEvent& e) override;
};
