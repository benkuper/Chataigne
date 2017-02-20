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
#include "ScriptTarget.h"

class ControllableContainer;
class ControllableUI;

class Controllable :
	public Inspectable,
	public ScriptTarget
{
public:
	enum Type { //Add type here if creating new type of Controllable
		CUSTOM,
		TRIGGER,
		FLOAT,
		INT,
		BOOL,
		STRING,
		ENUM,
		POINT2D,
		POINT3D,
		TARGET
	};


	Controllable(const Type &type, const String &niceName, const String &description, bool enabled = true);
	virtual ~Controllable();


	Type type;
	String niceName;
	String shortName;
	String description;
	String argumentsDescription;

	bool enabled;
	bool hasCustomShortName;
	bool isControllableExposed;
	bool isControllableFeedbackOnly;
	bool hideInEditor;
	bool hideInOutliner;

	String controlAddress;

	//ControllableChooser
	bool isTargettable;

	//save & load
	bool isSavable;
	bool saveValueOnly;


	//user control
	bool isCustomizableByUser;
	bool isRemovableByUser;

	bool replaceSlashesInShortName;

	ControllableContainer * parentContainer;

	void setNiceName(const String &_niceName);
	void setCustomShortName(const String &_shortName);
	void setAutoShortName();

	virtual void setEnabled(bool value, bool silentSet = false, bool force = false);

	void setParentContainer(ControllableContainer * container);
	void updateControlAddress();

	void remove(); // called from external to make this object ask for remove

	virtual var getJSONData(ControllableContainer * relativeTo = nullptr);
	virtual var getJSONDataInternal(); // to be overriden
	virtual void loadJSONData(var data);
	virtual void loadJSONDataInternal(var data) {} //to be overriden

	String getControlAddress(ControllableContainer * relativeTo = nullptr);

	// used for generating editor
	virtual ControllableUI * createDefaultUI(Controllable * targetControllable = nullptr) = 0;


	static var setValueFromScript(const juce::var::NativeFunctionArgs& a);

public:
	class  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}
		virtual void controllableStateChanged(Controllable *) {};
		virtual void controllableControlAddressChanged(Controllable *) {};
		virtual void controllableNameChanged(Controllable *) {};
		virtual void controllableRemoved(Controllable *) {};
		virtual void askForRemoveControllable(Controllable *) {}
	};

	ListenerList<Listener> listeners;
	void addControllableListener(Listener* newListener) { listeners.add(newListener); }
	void removeControllableListener(Listener* listener) { listeners.remove(listener); }

	virtual InspectableEditor * getEditor(bool /*isRootEditor*/) override;

	virtual String getTypeString() const { jassert(false); return ""; } //should be overriden

private:

	WeakReference<Controllable>::Master masterReference;
	friend class WeakReference<Controllable>;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Controllable)
};


#endif  // CONTROLLABLE_H_INCLUDED
