/*
  ==============================================================================

	ZeroconfManager.cpp
	Created: 1 Mar 2019 9:15:55am
	Author:  bkupe

  ==============================================================================
*/

#include "ZeroconfManager.h"

#ifdef JUCE_WINDOWS
#include <Winsock2.h>
#include <Ws2tcpip.h>
#else
#include <netdb.h>
#endif /* _WIN32 */

juce_ImplementSingleton(ZeroconfManager)

ZeroconfManager::ZeroconfManager() :
	Thread("Zeroconf"),
	zeroconfAsyncNotifier(5)
{
	startThread();
	startTimer(5000); //every 10s

}

ZeroconfManager::~ZeroconfManager()
{
	for (auto& s : searchers) s->servus.endBrowsing();
	stopThread(3000);
}

ZeroconfManager::ZeroconfSearcher* ZeroconfManager::addSearcher(StringRef name, StringRef serviceName)
{
	ZeroconfSearcher* s = getSearcher(name);

	if (s == nullptr)
	{
		s = new ZeroconfSearcher(name, serviceName);
		searchers.getLock().enter();
		searchers.add(s);
		searchers.getLock().exit();
	}

	search();

	return s;
}

void ZeroconfManager::removeSearcher(StringRef name)
{
	ZeroconfSearcher* s = getSearcher(name);
	if (s == nullptr)
	{
		searchers.getLock().enter();
		searchers.removeObject(s);
		searchers.getLock().exit();
	}
}

ZeroconfManager::ZeroconfSearcher* ZeroconfManager::getSearcher(StringRef name)
{
	for (auto& s : searchers) if (s->name == name) return s;
	return nullptr;
}

ZeroconfManager::ServiceInfo* ZeroconfManager::showMenuAndGetService(StringRef searcherName, bool showLocal, bool showRemote, bool separateLocalAndRemote, bool excludeInternal)
{
	ZeroconfSearcher* s = getSearcher(searcherName);

	if (s == nullptr)
	{
		DBG("No searcher found for service " << searcherName);
		return nullptr;
	}

	PopupMenu p;
	if (s->services.isEmpty())
	{
		p.addItem(-1, "No service found", false);
	}
	else
	{
		for (int i = 0; i < s->services.size(); i++)
		{
			ServiceInfo* info = s->services[i];
			p.addItem(1 + i, info->name + " on " + info->host + " (" + info->ip + ":" + String(info->port) + ")");
		}
	}

	int result = p.show();

	if (result <= 0) return nullptr;

	return s->services[result - 1];
}

void ZeroconfManager::search()
{
	if (Engine::mainEngine->isClearing) return;
	startThread();
}

void ZeroconfManager::timerCallback()
{
	search();
}


void ZeroconfManager::run()
{
	wait(300);

	bool changed = false;

	searchers.getLock().enter();
	for (auto& se : searchers)
	{
		if (threadShouldExit()) return;
		changed |= se->search();
	}
	searchers.getLock().exit();

	if (changed) zeroconfAsyncNotifier.addMessage(new ZeroconfEvent(ZeroconfEvent::SERVICES_CHANGED));
}

ZeroconfManager::ZeroconfSearcher::ZeroconfSearcher(StringRef name, StringRef serviceName) :
	name(name),
	serviceName(serviceName),
	servus(String(serviceName).toStdString())
{
}

ZeroconfManager::ZeroconfSearcher::~ZeroconfSearcher()
{
	services.clear();
}

bool ZeroconfManager::ZeroconfSearcher::search()
{
	if (Thread::getCurrentThread()->threadShouldExit()) return false;

	Strings instances = servus.discover(Servus::Interface::IF_ALL, 200);
	bool changed = false;

	if (Thread::getCurrentThread()->threadShouldExit()) return false;

	StringArray servicesArray;
	for (auto& s : instances)  servicesArray.add(s);

	Array<ServiceInfo*> servicesToRemove;

	for (auto& ss : services)
	{
		if (servicesArray.contains(ss->name))
		{
			String host = servus.get(ss->name.toStdString(), "servus_host");
			if (host.endsWithChar('.')) host = host.substring(0, host.length() - 1);
			int port = String(servus.get(ss->name.toStdString(), "servus_port")).getIntValue();

			if (ss->host != host || ss->port != port) servicesToRemove.add(ss);
		}
		else
		{
			servicesToRemove.add(ss);
		}
	}
	for (auto& ss : servicesToRemove) removeService(ss);


	for (auto& s : servicesArray)
	{
		if (Thread::getCurrentThread()->threadShouldExit()) return false;

		Servus::Data d;
		servus.getData(d);

		String host = servus.get(s.toStdString(), "servus_host");
		if (host.endsWithChar('.')) host = host.substring(0, host.length() - 1);

		int port = String(servus.get(s.toStdString(), "servus_port")).getIntValue();
		String ip = String(servus.get(s.toStdString(), "servus_ip"));

		Strings skeys = servus.getKeys(s.toStdString());
		HashMap<String, String> keys;
		for (auto& k : skeys)
		{
			if (k == "" || k == "servus_port" || k == "servus_host") continue;
			String kv = servus.get(s.toStdString(), k);
			keys.set(k, kv);
		}

		ServiceInfo* info = getService(s, host, port);

		if (info == nullptr)
		{
			changed = true;
			addService(s, host, ip, port, keys);
		}
		else if (info->host != host || info->port != port || info->ip != ip)
		{
			changed = true;
			updateService(info, host, ip, port, keys);
		}
	}

	return changed;
}


ZeroconfManager::ServiceInfo* ZeroconfManager::ZeroconfSearcher::getService(StringRef sName, StringRef host, int port)
{
	for (auto& i : services)
	{
		if (Thread::getCurrentThread()->threadShouldExit()) return nullptr;
		if (i->name == sName && i->host == host && i->port == port) return i;
	}
	return nullptr;
}

void ZeroconfManager::ZeroconfSearcher::addService(StringRef sName, StringRef host, StringRef ip, int port, const HashMap<String, String>& keys)
{
	if (Thread::getCurrentThread()->threadShouldExit()) return;

	String keysStr = ", keys : ";
	HashMap<String, String>::Iterator it(keys);
	while (it.next()) keysStr += "\n > " + it.getKey() + " = " + it.getValue();


	jassert(getService(sName, host, port) == nullptr);
	ServiceInfo* s = new ServiceInfo(sName, host, ip, port, keys);

	services.add(s);
	NLOG("Zeroconf", "New " << name << " service discovered : " << s->name << " on " << s->host << ", " << s->ip << ":" << s->port);// << service->keys);
	listeners.call(&SearcherListener::serviceAdded, s);
}

void ZeroconfManager::ZeroconfSearcher::removeService(ServiceInfo* s)
{
	jassert(s != nullptr);
	NLOG("Zeroconf", name << " service removed : " << s->name);
	listeners.call(&SearcherListener::serviceRemoved, s);
	services.removeObject(s);
}

void ZeroconfManager::ZeroconfSearcher::updateService(ServiceInfo* service, StringRef host, StringRef ip, int port, const HashMap<String, String>& keys)
{
	jassert(service != nullptr);
	service->host = host;
	service->ip = ip;
	service->port = port;
	service->setKeys(keys);

	NLOG("Zeroconf", "New " << name << " service updated : " << service->name << " on " << service->host << ", " << service->ip << ":" << service->port);
	listeners.call(&SearcherListener::serviceUpdated, service);
}


ZeroconfManager::ServiceInfo::ServiceInfo(StringRef name, StringRef host, StringRef ip, int port, const HashMap<String, String>& _keys) :
	name(name), host(host), ip(ip), port(port)
{
	setKeys(_keys);
	//DBG("New service info, keys : " << keys.size() << ", items");
}