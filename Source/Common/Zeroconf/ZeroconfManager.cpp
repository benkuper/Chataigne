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
	signalThreadShouldExit();
	waitForThreadToExit(3000);
}

void ZeroconfManager::addSearcher(StringRef name, StringRef serviceName)
{
	ZeroconfSearcher * s = getSearcher(name);

	if (s == nullptr)
	{
		searchers.getLock().enter();
		searchers.add(new ZeroconfSearcher(name, serviceName));
		searchers.getLock().exit();
	}
}

void ZeroconfManager::removeSearcher(StringRef name)
{
	ZeroconfSearcher * s = getSearcher(name);
	if (s == nullptr)
	{
		searchers.getLock().enter();
		searchers.removeObject(s);
		searchers.getLock().exit();
	}
}

ZeroconfManager::ZeroconfSearcher * ZeroconfManager::getSearcher(StringRef name)
{
	for (auto &s : searchers) if (s->name == name) return s;
	return nullptr;
}

ZeroconfManager::ServiceInfo * ZeroconfManager::showMenuAndGetService(StringRef searcherName, bool showLocal, bool showRemote, bool separateLocalAndRemote, bool excludeInternal)
{
	ZeroconfSearcher * s = getSearcher(searcherName);

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
			ServiceInfo * info = s->services[i];
			p.addItem(1 + i, info->name + " on " + info->host + " (" + info->ip + ":" + String(info->port) + ")");
		}
	}

	int result = p.show();

	if (result <= 0) return nullptr;
	
	return s->services[result - 1];
}

void ZeroconfManager::search()
{
	startThread();
}

void ZeroconfManager::timerCallback()
{
	search();
}


void ZeroconfManager::run()
{
	sleep(1000);

	bool changed = false;

	searchers.getLock().enter();
	for (auto & se : searchers)  changed |= se->search();
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
	Strings instances = servus.discover(Servus::Interface::IF_ALL, 200);

	bool changed = false;

	StringArray servicesArray;
	for (auto &s : instances)  servicesArray.add(s);

	Array<ServiceInfo *> servicesToRemove;

	for (auto &ss : services)
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
	for (auto &ss : servicesToRemove) removeService(ss);


	for (auto &s : servicesArray)
	{
		String host = servus.get(s.toStdString(), "servus_host");
		if (host.endsWithChar('.')) host = host.substring(0, host.length() - 1);

		int port = String(servus.get(s.toStdString(), "servus_port")).getIntValue();
		String ip = getIPForHostAndPort(host, port);

		bool isLocal = false;
		if (ip.isNotEmpty())
		{
			Array<IPAddress> localIps;
			IPAddress::findAllAddresses(localIps);
			for (auto &lip : localIps)
			{
				if (ip == lip.toString())
				{
					isLocal = true;
					break;
				}
			}
		}

		if (isLocal) ip = IPAddress::local().toString();
		ServiceInfo * info = getService(s, host, port);
		if (info == nullptr)
		{
			changed = true;
			addService(s, host, ip, port);
		}
		else if (info->host != host || info->port != port || info->ip != ip)
		{
			changed = true;
			updateService(info, host, ip, port);
		}



	}

	return changed;
}


String ZeroconfManager::ZeroconfSearcher::getIPForHostAndPort(String host, int port)
{
	String ip;

	struct addrinfo hints = { 0 };
	hints.ai_family = AF_INET;

	struct addrinfo* info = nullptr;
	getaddrinfo(host.toRawUTF8(), String(port).toRawUTF8(), &hints, &info);

	char * ipData = info->ai_addr->sa_data;
	if (info != nullptr) ip = String((uint8)ipData[2]) + "." + String((uint8)ipData[3]) + "." + String((uint8)ipData[4]) + "." + String((uint8)ipData[5]);
	
	freeaddrinfo(info);

	return ip;
}

ZeroconfManager::ServiceInfo * ZeroconfManager::ZeroconfSearcher::getService(StringRef sName, StringRef host, int port)
{
	for (auto &i : services)
	{
		if (i->name == sName && i->host == host && i->port == port) return i;
	}
	return nullptr;
}

void ZeroconfManager::ZeroconfSearcher::addService(StringRef sName, StringRef host, StringRef ip, int port)
{
	NLOG("Zeroconf", "New " << name << " service discovered : " << sName << " on " << host << ", " << ip << ":" << port);
	jassert(getService(sName, host, port) == nullptr);
	services.add(new ServiceInfo{ sName, host, ip, port });

}

void ZeroconfManager::ZeroconfSearcher::removeService(ServiceInfo * s)
{
	jassert(s != nullptr);
	NLOG("Zeroconf", name << " service removed : " << s->name);
	services.removeObject(s);
}

void ZeroconfManager::ZeroconfSearcher::updateService(ServiceInfo * service, StringRef host, StringRef ip, int port)
{
	jassert(service != nullptr);
	NLOG("Zeroconf", name << "service updated changed : " << name << " : " << host << ", " << ip << ":" << port);
	service->host = host;
	service->ip = ip;
	service->port = port;
}
