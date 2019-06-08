/*
  ==============================================================================

    MatamoAnalytics.cpp
    Created: 29 May 2019 2:43:04am
    Author:  bkupe

  ==============================================================================
*/

#include "MatamoAnalytics.h"

juce_ImplementSingleton(MatamoAnalytics)


String getAppVersion();

MatamoAnalytics::MatamoAnalytics() :
	Thread("Matamo"),
	actionIsPing(false)
{
	
	String name = SystemStats::getFullUserName();
	String osName = SystemStats::getOperatingSystemName();

	Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().totalArea;

	String res = String(r.getWidth()) + "x" + String(r.getHeight());

	Random rand(Time::currentTimeMillis());
	String id = Base64::toBase64(name).substring(0, 16);
	while (id.length() < 16) id = "0" + id;

	String lang = SystemStats::getUserLanguage() +"-"+ SystemStats::getUserRegion();

#if JUCE_WINDOWS
	String os = "Windows";
	String ua = "Mozilla / 5.0 ("+osName+"; Win64; x64)";
#elif JUCE_MAC
	String os = "Mac OS";
	String ua = "Mozilla / 5.0 (Macintosh; Intel "+osName+")";
#else
	String os = "Linux";
	String ua = "Mozilla / 5.0 (Linux x86_64; "+osName+")";
#endif

	baseURL = URL("http://benjamin.kuperberg.fr/chataigne/matomo/matomo.php")
		.withParameter("rec", "1").withParameter("idsite", "1").withParameter("apiv", "1").withParameter("ua", ua)
		.withParameter("res", res).withParameter("rand", String(rand.nextInt())).withParameter("_id", id).withParameter("uid", name)
		.withParameter("_cvar", "{\"1\":[\"User name\",\"" + name + "\"], \"2\":[\"App Version\",\"" + getAppVersion() + "\"]}");
}

MatamoAnalytics::~MatamoAnalytics()
{
	signalThreadShouldExit();
	waitForThreadToExit(2000);
}

void MatamoAnalytics::log(AnalyticsAction action)
{
	log(actionNames[action]);
	actionIsPing = action == STOP;
}

void MatamoAnalytics::log(StringRef actionName)
{
	actionToLog = actionName;
	startThread();
}

void MatamoAnalytics::run()
{
	
	URL url = baseURL.withParameter("action_name", actionToLog);
	if (actionIsPing) url = url.withParameter("ping", "1");

	DBG("Send to analytics " << url.toString(false) << ", params :\n > " << url.getParameterValues().joinIntoString("\n > "));
	StringPairArray responseHeaders;
	int statusCode = 0;
	std::unique_ptr<InputStream> stream(url.createInputStream(true, nullptr, nullptr, String(),
		2000, // timeout in millisecs
		&responseHeaders, &statusCode));
#if JUCE_WINDOWS
	if (statusCode != 200)
	{
		NLOGWARNING("Matamo Analytics", "Failed to connect, status code = " + String(statusCode));
		return;
	}
#endif


	if (stream != nullptr)
	{
		String content = stream->readEntireStreamAsString();
		DBG("Got analytics data " << content);
	}
	else
	{
		NLOGWARNING("Matomo Analytics", "Error with request, status code : " << statusCode << ", url : " << url.toString(true));
	}
}
