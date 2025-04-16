
/*
  ==============================================================================

	MorpherManagerUI.h
	Created: 11 Dec 2017 5:01:24pm
	Author:  Ben

  ==============================================================================
*/

#pragma once


class MorpherViewUI;

class MorpherPanel :
	public ShapeShifterContentComponent,
	public InspectableSelectionManager::Listener,
	public Inspectable::InspectableListener,
	public Parameter::AsyncListener
{
public:
	MorpherPanel(StringRef contentName);
	~MorpherPanel();

	CVGroup* currentGroup;
	std::unique_ptr<MorpherViewUI> currentMorpherUI;
	
	void setGroup(CVGroup* g);
	void setMorpher(Morpher* m);

	void paint(Graphics& g) override;
	void resized() override;

	void inspectablesSelectionChanged() override;
	void inspectableDestroyed(Inspectable* i) override;
	void newMessage(const Parameter::ParameterEvent& e) override;

	static MorpherPanel* create(const String& contentName) { return new MorpherPanel(contentName); }
};



class MorpherViewUI :
	public ManagerViewUI<CVPresetManager, CVPreset, CVPresetMorphUI>,
	public ContainerAsyncListener,
	public UITimerTarget
{
public:
	MorpherViewUI(Morpher* morpher);
	~MorpherViewUI();

	Morpher* morpher;
	MorphTargetUI mainTargetUI;

	Image bgImage;

	void paintBackground(Graphics& g) override;

	void setupBGImage();

	void resized() override;

	void setViewZoom(float value) override;

	void mouseDown(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;
	void mouseDoubleClick(const MouseEvent& e) override;

	void itemDragMove(const SourceDetails& details) override;
	void itemDropped(const SourceDetails& details) override;

	void newMessage(const ContainerAsyncEvent& e) override;
	void controllableFeedbackUpdateAsync(Controllable* c);

	void handlePaintTimerInternal() override;
    void paintOverChildren(juce::Graphics& g) override;
};