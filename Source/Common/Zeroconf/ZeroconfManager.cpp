/*
  ==============================================================================

	ZeroconfManager.cpp
	Created: 1 Mar 2019 9:15:55am
	Author:  bkupe

  ==============================================================================
*/

#ifdef JUCE_WINDOWS
#include <Winsock2.h>
#include <Ws2tcpip.h>
#else
#include <netdb.h>
#include "ZeroconfManager.h"
#endif /* _WIN32 */

juce_ImplementSingleton(ZeroconfManager)

ZeroconfManager::ZeroconfManager() :
	zeroconfAsyncNotifier(5)
{
}

ZeroconfManager::~ZeroconfManager()
{
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

	//search();

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
			p.addItem(1 + i, info->name + " on " + info->host + " (" + info->getIP() + ":" + String(info->port) + ")");
		}
	}

	int result = p.show();

	if (result <= 0) return nullptr;

	return s->services[result - 1];
}

ZeroconfManager::ZeroconfSearcher::ZeroconfSearcher(StringRef name, StringRef serviceName) :
	Thread("Zeroconf Searcher "+name),
	name(name),
	serviceName(serviceName)
{
	startThread();
}

ZeroconfManager::ZeroconfSearcher::~ZeroconfSearcher()
{
	if (servus != nullptr && servus->isBrowsing()) servus->endBrowsing();
	servus.reset();
	stopThread(4000);
	services.clear();
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
	NLOG("Zeroconf", "New " << name << " service discovered : " << s->name << " on " << s->host << ", " << s->ip << ":" << s->port << (s->isLocal?" (local)":""));// << service->keys);
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

	//NLOG("Zeroconf", "New " << name << " service updated : " << service->name << " on " << service->host << ", " << service->ip << ":" << service->port << (service->isLocal ? " (local)" : ""));
	listeners.call(&SearcherListener::serviceUpdated, service);
}

void ZeroconfManager::ZeroconfSearcher::instanceAdded(const std::string& instance)
{
	Servus::Data d;
	servus->getData(d);

	String host = servus->get(instance, "servus_host");
	if (host.endsWithChar('.')) host = host.substring(0, host.length() - 1);

	int port = String(servus->get(instance, "servus_port")).getIntValue();
	String ip = String(servus->get(instance, "servus_ip"));

	Strings skeys = servus->getKeys(instance);
	HashMap<String, String> keys;
	for (auto& k : skeys)
	{
		if (k == "" || k == "servus_port" || k == "servus_host") continue;
		String kv = servus->get(instance, k);
		keys.set(k, kv);
	}

	ServiceInfo* info = getService(instance, host, port);

	if (info == nullptr)
	{
		addService(instance, host, ip, port, keys);
	}
	else if (info->host != host || info->port != port || info->ip != ip)
	{
		updateService(info, host, ip, port, keys);
	}
}

void ZeroconfManager::ZeroconfSearcher::instanceRemoved(const std::string& instance)
{
	String s = instance;
	//String host = servus->get(instance, "servus_host");
	//if (host.endsWithChar('.')) host = host.substring(0, host.length() - 1);
	//int port = String(servus->get(instance, "servus_port")).getIntValue();

	for (auto& ss : services)
	{
		if (ss->name == s)
		{
			removeService(ss);
			return;
		}
	}
}

void ZeroconfManager::ZeroconfSearcher::run()
{
	servus.reset(new Servus(String(serviceName).toStdString()));
	servus->addListener(this);

	servus->beginBrowsing(Servus::Interface::IF_ALL);

	while (!threadShouldExit())
	{
		servus->browse(1000);
		wait(500);
	}

	
}

ZeroconfManager::ServiceInfo::ServiceInfo(StringRef name, StringRef host, StringRef ip, int port, const HashMap<String, String>& _keys) :
	name(name), host(host), ip(ip), port(port)
{
	setKeys(_keys);
	isLocal = NetworkHelpers::isIPLocal(ip);
	//DBG("New service info, keys : " << keys.size() << ", items");
}