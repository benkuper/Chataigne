/*
  ==============================================================================

    ControllableIndexedContainer.cpp
    Created: 10 Mar 2016 6:08:58pm
    Author:  bkupe

  ==============================================================================
*/

#include "ControllableIndexedContainer.h"

//Controllable * ControllableIndexedContainer::getControllableForAddress(StringArray addressSplit, bool /*recursive*/, bool /*getNotExposed*/)
//{
//
//    //Indexed container is not made to directly contain contrallable, but only multiple containers of the same type
//    bool isTargetAControllable = addressSplit.size() == 1;
//    if (isTargetAControllable) return nullptr;
//
//    int index = addressSplit[0].getIntValue();
//    if (index >= controllableContainers.size()) return nullptr;
//
//
//    addressSplit.remove(0);
//    return controllableContainers[index]->getControllableForAddress(addressSplit);
//}
