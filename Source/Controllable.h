/*
 ==============================================================================

 Controllable.h
 Created: 8 Mar 2016 1:08:56pm
 Author:  bkupe

 ==============================================================================
 */

#ifndef CONTROLLABLE_H_INCLUDED
#define CONTROLLABLE_H_INCLUDED


#include "JuceHeader.h"//keep
#include "Inspectable.h"

class ControllableContainer;
class ControllableUI;

class Controllable :
	public Inspectable
{
public:
  enum Type { //Add type here if creating new type of Controllable
    TRIGGER,
    FLOAT,
    INT,
    BOOL,
    STRING,
	ENUM,
	POINT2D,
	POINT3D,
    RANGE,
    PROXY,

  };


  Controllable(const Type &type, const String &niceName, const String &description, bool enabled = true);
  virtual ~Controllable();


  Type type;
  String niceName;
  String shortName;
  String description;


  bool enabled;
  bool hasCustomShortName;
  bool isControllableExposed;
  bool isControllableFeedbackOnly;
  bool hideInEditor;
  bool hideInOutliner;
  String controlAddress;

  bool replaceSlashesInShortName;

  ControllableContainer * parentContainer;

  void setNiceName(const String &_niceName);
  void setCustomShortName(const String &_shortName);
  void setAutoShortName();

  void setEnabled(bool value, bool silentSet = false, bool force = false);

  void setParentContainer(ControllableContainer * container);
  void updateControlAddress();

  String getControlAddress(ControllableContainer * relativeTo = nullptr);

  // used for generating editor
  virtual ControllableUI * createDefaultUI(Controllable * targetControllable = nullptr) = 0;

  //used for script variables
  //virtual DynamicObject * createDynamicObject();

public:
  class  Listener
  {
  public:
    /** Destructor. */
    virtual ~Listener() {}
    virtual void controllableStateChanged(Controllable * ) {};
    virtual void controllableControlAddressChanged(Controllable * ) {};
    virtual void controllableNameChanged(Controllable *) {};
    virtual void controllableRemoved(Controllable * ) {};
  };

  ListenerList<Listener> listeners;
  void addControllableListener(Listener* newListener) { listeners.add(newListener); }
  void removeControllableListener(Listener* listener) { listeners.remove(listener); }


  //Script set method handling
 // static var setControllableValue(const juce::var::NativeFunctionArgs& a);


  InspectorEditor * getEditor() override;

private:

  WeakReference<Controllable>::Master masterReference;
  friend class WeakReference<Controllable>;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Controllable)
};


#endif  // CONTROLLABLE_H_INCLUDED
