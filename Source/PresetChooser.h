/*
  ==============================================================================

    PresetChooser.h
    Created: 20 Oct 2016 6:28:03pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef PRESETCHOOSER_H_INCLUDED
#define PRESETCHOOSER_H_INCLUDED

#include "ControllableContainer.h"

class PresetChooser : public ComboBox,
					  public ComboBox::Listener,
				      public ControllableContainerListener
{
public:
	PresetChooser(ControllableContainer * _container);
	~PresetChooser();

	ControllableContainer * container;

	virtual void updatePresetComboBox(bool forceUpdate = false);
	virtual void comboBoxChanged(ComboBox * comboBoxThatHasChanged) override;

	virtual void controllableContainerPresetLoaded(ControllableContainer *) override;

};



#endif  // PRESETCHOOSER_H_INCLUDED
