/*
  ==============================================================================

    QLabModule.h
    Created: 10 May 2020 6:30:00pm
    Author:  jgastonraoul

  ==============================================================================
*/

#pragma once

class QLabModule :
	public OSCModule
{
public:
	QLabModule();
	~QLabModule() {}

	StringParameter* workspaceID;

	String getWorkspacePrefix() const;

	String getAddressForRoutedValue(Controllable* c, OSCRouteParams* op) override;

	static QLabModule * create() { return new QLabModule(); }
	virtual String getDefaultTypeString() const override { return "QLab"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(QLabModule)
};


