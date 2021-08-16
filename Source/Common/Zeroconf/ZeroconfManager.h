/*
  ==============================================================================

    ZeroconfManager.h
    Created: 1 Mar 2019 9:15:55am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "servus/servus.h"
#include "servus/listener.h"

class ZeroconfManager
{
public:
	juce_DeclareSingleton(ZeroconfManager, true);
	ZeroconfManager();
	~ZeroconfManager();

	class ServiceInfo
	{
	public:
		ServiceInfo(StringRef name, StringRef host, StringRef ip, int port, const HashMap<String, String>& _keys);

		String name;
		String host;
		String ip;
		int port;
		HashMap<String, String> keys;
		bool isLocal;

		void setKeys(const HashMap<String, String>& _keys)
		{
			keys.clear();
			HashMap<String, String>::Iterator i(_keys);
			while (i.next()) keys.set(i.getKey(), i.getValue());
		}

		void addKey(String key, String value) { keys.set(key, value); }
		String getKey(String key) { return keys.contains(key) ? keys[key] : ""; }

		String getIP() const { return isLocal ? "127.0.0.1" : ip; }

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ServiceInfo)

	};

	class ZeroconfSearcher :
		public servus::Listener,
		public Thread
	{
	public:
		ZeroconfSearcher(StringRef name, StringRef serviceName);
		~ZeroconfSearcher();

		String name;
		String serviceName;
		std::unique_ptr<servus::Servus> servus;
		OwnedArray<ServiceInfo> services;
		CriticalSection browseLock;

		ServiceInfo * getService(StringRef name, StringRef host, int port);
		void addService(StringRef name, StringRef host, StringRef ip, int port, const HashMap<String, String> & keys = HashMap<String, String>());
		void removeService(ServiceInfo * service);
		void updateService(ServiceInfo * service, StringRef host, StringRef ip, int port, const HashMap<String, String>  &keys = HashMap<String, String>());

		void instanceAdded(const std::string& instance) override;
		void instanceRemoved(const std::string& instance) override;

		String getIPForHost(String host);

		void run() override;

		class SearcherListener
		{
		public:
			virtual ~SearcherListener() {}
			virtual void serviceAdded(ServiceInfo* service) {};
			virtual void serviceUpdated(ServiceInfo* service) {};
			virtual void serviceRemoved(ServiceInfo* service) {};
		};

		ListenerList<SearcherListener> listeners;
		void addSearcherListener(SearcherListener* newListener) { listeners.add(newListener); }
		void removeSearcherListener(SearcherListener* listener) { listeners.remove(listener); }
	};

	OwnedArray<ZeroconfSearcher, CriticalSection> searchers;

	ZeroconfSearcher * addSearcher(StringRef name, StringRef serviceName);
	void removeSearcher(StringRef name);

	ZeroconfSearcher * getSearcher(StringRef name);

	ServiceInfo* showMenuAndGetService(StringRef service, bool showLocal = true, bool showRemote = true, bool separateLocalAndRemote = true, bool excludeInternal = true, const String &nameFilter = "");
	
	class ZeroconfEvent {
	public:
		enum Type { SERVICES_CHANGED };
		ZeroconfEvent(Type type) : type(type) {}
		Type type;
	};

	QueuedNotifier<ZeroconfEvent> zeroconfAsyncNotifier;
	typedef QueuedNotifier<ZeroconfEvent>::Listener AsyncListener;
	void addAsyncZeroconfListener(AsyncListener* newListener) { zeroconfAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedZeroconfListener(AsyncListener* newListener) { zeroconfAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncZeroconfListener(AsyncListener* listener) { zeroconfAsyncNotifier.removeListener(listener); }

};
