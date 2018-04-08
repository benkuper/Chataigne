/*
  ==============================================================================

    ModuleChooserUI.h
    Created: 13 Mar 2017 4:22:50pm
    Author:  Ben-Portable

  ==============================================================================
*/

#ifndef MODULECHOOSERUI_H_INCLUDED
#define MODULECHOOSERUI_H_INCLUDED


#include "ModuleManagerUI.h"


class ModuleChooserUI :
	public ModuleManager::AsyncListener,
	public ContainerAsyncListener,
	public ComboBox,
	public ComboBox::Listener
{
public:
	ModuleChooserUI();
	virtual ~ModuleChooserUI();

	std::function<bool(Module *)> filterModuleFunc;

	void buildModuleBox();

	void setModuleSelected(Module *, bool silent);

	void comboBoxChanged(ComboBox *) override;

	void newMessage(const ModuleManager::ManagerEvent &e) override;
	void newMessage(const ContainerAsyncEvent &e) override;

	class ChooserListener
	{
	public:
		virtual ~ChooserListener() {}
		virtual void selectedModuleChanged(ModuleChooserUI *, Module *) {}
		virtual void moduleListChanged(ModuleChooserUI  *) {}
	};

	ListenerList<ChooserListener> chooserListeners;
	void addChooserListener(ChooserListener* newListener) { chooserListeners.add(newListener); }
	void removeChooserListener(ChooserListener* listener) { chooserListeners.remove(listener); }
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleChooserUI)
};



#endif  // MODULECHOOSERUI_H_INCLUDED
