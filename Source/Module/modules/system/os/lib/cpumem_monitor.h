#pragma once
#include "JuceHeader.h"

#ifndef OS_SYSINFO_SUPPORT
#define OS_SYSINFO_SUPPORT 1
#endif

#if OS_SYSINFO_SUPPORT
#if JUCE_WINDOWS
#include <windows.h>
#include <psapi.h>
#include <pdh.h>
#pragma comment(lib,"pdh.lib")
#elif JUCE_LINUX
#include <fstream>
#include <sstream>
#elif JUCE_MAC
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <unistd.h>
#include <mach/mach.h>
#endif
#endif

class OSSystemInfo {
public:
	static double getSystemCPUUsage() {
#if OS_SYSINFO_SUPPORT

#if JUCE_WINDOWS
		PDH_HQUERY query;
		PDH_HCOUNTER counter;
		PDH_FMT_COUNTERVALUE value;

		PdhOpenQuery(NULL, NULL, &query);
		PdhAddEnglishCounter(query, "\\Processor(_Total)\\% Processor Time", NULL, &counter);
		PdhCollectQueryData(query);
		Thread::getCurrentThread()->wait(500);
		PdhCollectQueryData(query);
		PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, NULL, &value);
		PdhCloseQuery(query);

		return value.doubleValue / 100.0;

#elif JUCE_LINUX
		std::ifstream file("/proc/stat");
		std::string line;
		std::getline(file, line);

		// /proc/stat format: cpu user nice system idle iowait irq softirq
		std::istringstream ss(line);
		std::string cpuLabel;
		ss >> cpuLabel;
		if (cpuLabel != "cpu") {
			DBG("Error reading CPU stats");
			return -1.0;
		}

		int user, nice, system, idle, iowait, irq, softirq;
		ss >> user >> nice >> system >> idle >> iowait >> irq >> softirq;

		int totalCpuTime = user + nice + system + idle + iowait + irq + softirq;
		int idleTime = idle + iowait;
		double cpuUsage = 1.0 - (double)idleTime / totalCpuTime;
		return cpuUsage;

#elif JUCE_MAC
		mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
		host_cpu_load_info_data_t cpuinfo;
		if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuinfo, &count) != KERN_SUCCESS) {
			DBG("Error getting CPU info on macOS");
			return -1.0;
		}

		double totalTicks = 0;
		for (int i = 0; i < CPU_STATE_MAX; i++) {
			totalTicks += cpuinfo.cpu_ticks[i];
		}

		double idleTicks = cpuinfo.cpu_ticks[CPU_STATE_IDLE];
		double cpuUsage = 1.0 - (idleTicks / totalTicks);
		return cpuUsage;
#else
		DBG("Platform not supported");
		return -1.0;
#endif

#else // OS_SYSINFO_SUPPORT
		DBG("OS_SYSINFO_SUPPORT not enabled");
		return 0.0;
#endif
	}

	static double getSystemMemoryUsageRatio()
	{
#if OS_SYSINFO_SUPPORT
#if JUCE_WINDOWS
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);
		double usedMemory = static_cast<double>(memInfo.ullTotalPhys - memInfo.ullAvailPhys);
		double totalMemory = static_cast<double>(memInfo.ullTotalPhys);
		return usedMemory / totalMemory;
#elif JUCE_LINUX
		std::ifstream memInfo("/proc/meminfo");
		std::string line;
		long totalMem = 0, freeMem = 0, bufferMem = 0, cacheMem = 0;
		while (std::getline(memInfo, line)) {
			std::istringstream iss(line);
			std::string label;
			long value;
			iss >> label >> value;

			if (label == "MemTotal:") {
				totalMem = value;
			}
			else if (label == "MemFree:") {
				freeMem = value;
			}
			else if (label == "Buffers:") {
				bufferMem = value;
			}
			else if (label == "Cached:") {
				cacheMem = value;
			}
		}

		long usedMem = totalMem - freeMem - bufferMem - cacheMem;
		return static_cast<double>(usedMem) / static_cast<double>(totalMem);

#elif JUCE_MAC
		int mib[2];
		int64_t totalMemory, freeMemory;
		size_t length = sizeof(int64_t);
		mib[0] = CTL_HW;
		mib[1] = HW_MEMSIZE;
		sysctl(mib, 2, &totalMemory, &length);

		vm_size_t page_size;
		mach_port_t mach_port;
		mach_msg_type_number_t count;
		vm_statistics64_data_t vm_stats;
		mach_port = mach_host_self();
		count = sizeof(vm_stats) / sizeof(natural_t);
		host_page_size(mach_port, &page_size);
		if (host_statistics64(mach_port, HOST_VM_INFO64, (host_info64_t)&vm_stats, &count) != KERN_SUCCESS) {
			DBG("Failed to get memory info");
			return -1.0; // Return -1 to indicate failure
		}

		int64_t usedMemory = (int64_t)vm_stats.active_count + (int64_t)vm_stats.inactive_count +
			(int64_t)vm_stats.wire_count;
		usedMemory *= (int64_t)page_size;
		return static_cast<double>(usedMemory) / static_cast<double>(totalMemory);
#endif

#else 
		DBG("OS_SYSINFO_SUPPORT not enabled");
		return 0.0;
#endif
	}
};