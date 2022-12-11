/*
  ==============================================================================

	DMXUniverseManager.h
	Created: 10 Dec 2022 6:34:52pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class DMXUniverseManager :
	public BaseManager<DMXUniverse>
{
public:
	DMXUniverseManager(bool useParams = false);
	~DMXUniverseManager();

	bool useParams;

	DMXUniverse* createItem() override;
};