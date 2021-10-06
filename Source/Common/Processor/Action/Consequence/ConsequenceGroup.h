/*
  ==============================================================================

    ConsequenceGroup.h
    Created: 6 Oct 2021 6:11:00pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConsequenceGroup :
	public BaseItem
{
public:
	ConsequenceGroup(var params = var(), Multiplex* multiplex = nullptr);
	~ConsequenceGroup();

	ConsequenceManager csm;

	String getTypeString() const override { return "Group"; }
};