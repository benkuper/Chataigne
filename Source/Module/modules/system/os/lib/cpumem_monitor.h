#pragma once
#include <iomanip>
#include <math.h>
#include <sstream>
#include <string.h>
#include <string>

#ifdef WIN32
#include <pdh.h>
#include <psapi.h>
#include <tchar.h>
#pragma comment(lib, "pdh")

#elif __APPLE__

#elif __linux__
#include <sys/sysinfo.h>
#include <sys/times.h>
#include <sys/types.h>
//#include <sys/vtimes.h>

#else
#error "Unknown Operating System!"
#endif
namespace SL {
    namespace NET {

        inline std::string to_PrettyBytes(long long int bytes)
        {
            static auto convlam = [](const auto a_value, const int n) {
                std::ostringstream out;
                out << std::fixed << std::setprecision(n) << a_value;
                return out.str();
            };

            const char *suffixes[7];
            suffixes[0] = " B";
            suffixes[1] = " KB";
            suffixes[2] = " MB";
            suffixes[3] = " GB";
            suffixes[4] = " TB";
            suffixes[5] = " PB";
            suffixes[6] = " EB";
            unsigned int s = 0; // which suffix to use
            auto count = static_cast<double>(bytes);
            while (count >= 1024 && s < 7) {
                s++;
                count /= 1024;
            }
            if (count - floor(count) == 0.0)
                return std::to_string((int)count) + suffixes[s];
            else
                return convlam(count, 2) + suffixes[s];
        }
        struct MemoryUse {
            long long int VirtualTotalUsed = 0;
            long long int VirtualProcessUsed = 0;
            long long int VirtualTotalAvailable = 0;
            long long int PhysicalTotalUsed = 0;
            long long int PhysicalProcessUsed = 0;
            long long int PhysicalTotalAvailable = 0;
        };
        struct CPUUse {
            double ProcessUse = 0.0;
            double TotalUse = 0.0;
        };
#ifdef WIN32

        class CPUMemMonitor {
            HANDLE cpuQuery = NULL;
            HANDLE cpuTotal = NULL;
            ULARGE_INTEGER lastCPU = { 0 };
            ULARGE_INTEGER lastSysCPU = { 0 };
            ULARGE_INTEGER lastUserCPU = { 0 };
            int numProcessors = 1;
            HANDLE currentprocess = NULL;

        public:
            CPUMemMonitor()
            {
                PdhOpenQuery(NULL, NULL, &cpuQuery);

                PdhAddEnglishCounterW(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
                PdhCollectQueryData(cpuQuery);

                SYSTEM_INFO sysInfo;
                FILETIME ftime, fsys, fuser;

                GetSystemInfo(&sysInfo);
                numProcessors = sysInfo.dwNumberOfProcessors;

                GetSystemTimeAsFileTime(&ftime);
                memcpy(&lastCPU, &ftime, sizeof(FILETIME));

                currentprocess = GetCurrentProcess();
                GetProcessTimes(currentprocess, &ftime, &ftime, &fsys, &fuser);
                memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
                memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
            }
            ~CPUMemMonitor()
            {
                if (cpuQuery == NULL) {
                    PdhCloseQuery(cpuQuery);
                }
            }
            CPUUse getCPUUsage()
            {
                PDH_FMT_COUNTERVALUE counterVal;
                PdhCollectQueryData(cpuQuery);
                PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
                CPUUse c;
                c.TotalUse = counterVal.doubleValue;

                FILETIME ftime, fsys, fuser;
                ULARGE_INTEGER now, sys, user;
                double percent = 0.0;
                GetSystemTimeAsFileTime(&ftime);
                memcpy(&now, &ftime, sizeof(FILETIME));
                GetProcessTimes(currentprocess, &ftime, &ftime, &fsys, &fuser);
                memcpy(&sys, &fsys, sizeof(FILETIME));
                memcpy(&user, &fuser, sizeof(FILETIME));
                percent = static_cast<double>(sys.QuadPart - lastSysCPU.QuadPart) + (user.QuadPart - lastUserCPU.QuadPart);
                percent /= static_cast<double>(now.QuadPart - lastCPU.QuadPart);
                percent /= static_cast<double>(numProcessors);
                lastCPU = now;
                lastUserCPU = user;
                lastSysCPU = sys;
                c.ProcessUse = percent * 100.0;
                return c;
            }
            MemoryUse getMemoryUsage()
            {
                PROCESS_MEMORY_COUNTERS_EX pmc;
                GetProcessMemoryInfo(currentprocess, (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc));
                MEMORYSTATUSEX memInfo;
                memInfo.dwLength = sizeof(MEMORYSTATUSEX);
                GlobalMemoryStatusEx(&memInfo);
                MemoryUse m;
                m.PhysicalTotalUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
                m.PhysicalTotalAvailable = memInfo.ullTotalPhys;
                m.PhysicalProcessUsed = pmc.WorkingSetSize;

                m.VirtualTotalAvailable = memInfo.ullTotalPageFile;
                m.VirtualTotalUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
                m.VirtualProcessUsed = pmc.PrivateUsage;
                return m;
            }
        };
#elif __APPLE__

        class CPUMemMonitor {

        public:
            CPUMemMonitor() {}
            ~CPUMemMonitor() {}
            CPUUse getCPUUsage()
            {
                CPUUse c = { 0 };
                return c;
            }
            MemoryUse getMemoryUsage()
            {
                MemoryUse m = { 0 };
                return m;
            }
        };
#elif __linux__

        class CPUMemMonitor {
        private:
            long long int parseLine(char *line)
            {

                // This assumes that a digit will be found and the line ends in " Kb".
                int i = strlen(line);
                const char *p = line;
                while (*p < '0' || *p > '9')
                    p++;
                line[i - 3] = '\0';
                return atoll(p) * 1000; // translate to bytes
            }
            void getprocessmemory(MemoryUse &m)
            {
                m.PhysicalProcessUsed = m.VirtualProcessUsed = 0;
                FILE *file = fopen("/proc/self/status", "r");
                if (file == NULL) return;
                char line[128];
                while (fgets(line, 128, file) != NULL && (m.VirtualProcessUsed == 0 || m.PhysicalProcessUsed == 0)) {
                    if (strncmp(line, "VmSize:", 7) == 0) {
                        m.VirtualProcessUsed = parseLine(line);
                    }
                    else if (strncmp(line, "VmRSS:", 6) == 0) {
                        m.PhysicalProcessUsed = parseLine(line);
                    }
                }
                fclose(file);
            }
            unsigned long long lastTotalUser = 0;
            unsigned long long lastTotalUserLow = 0;
            unsigned long long lastTotalSys = 0;
            unsigned long long lastTotalIdle = 0;
            clock_t lastCPU = { 0 };
            clock_t lastSysCPU = { 0 };
            clock_t lastUserCPU = { 0 };
            int numProcessors = 0;

        public:
            CPUMemMonitor()
            {
                FILE *file = fopen("/proc/stat", "r");
                if (file) {
                    fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow, &lastTotalSys, &lastTotalIdle);
                    fclose(file);
                }

                file = NULL;
                struct tms timeSample;
                char line[128];

                lastCPU = times(&timeSample);
                lastSysCPU = timeSample.tms_stime;
                lastUserCPU = timeSample.tms_utime;
                if (file) {
                    file = fopen("/proc/cpuinfo", "r");
                    numProcessors = 0;
                    while (fgets(line, 128, file) != NULL) {
                        if (strncmp(line, "processor", 9) == 0)
                            numProcessors++;
                    }
                    fclose(file);
                }
            }
            ~CPUMemMonitor() {}
            CPUUse getCPUUsage()
            {
                CPUUse c;
                double percent;
                unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

                FILE * file = fopen("/proc/stat", "r");
                if (file) {
                    fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow, &totalSys, &totalIdle);
                    fclose(file);
                }

                if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow || totalSys < lastTotalSys || totalIdle < lastTotalIdle) {
                    // Overflow detection. Just skip this value.
                    percent = -1.0;
                }
                else {
                    total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) + (totalSys - lastTotalSys);
                    percent = total;
                    total += (totalIdle - lastTotalIdle);
                    percent /= total;
                    percent *= 100.0;
                }

                lastTotalUser = totalUser;
                lastTotalUserLow = totalUserLow;
                lastTotalSys = totalSys;
                lastTotalIdle = totalIdle;
                c.TotalUse = percent;

                struct tms timeSample;
                clock_t now;
                percent = 0.0;
                now = times(&timeSample);
                if (now <= lastCPU || timeSample.tms_stime < lastSysCPU || timeSample.tms_utime < lastUserCPU) {
                    // Overflow detection. Just skip this value.
                    percent = -1.0;
                }
                else {
                    percent = (timeSample.tms_stime - lastSysCPU) + (timeSample.tms_utime - lastUserCPU);
                    percent /= (now - lastCPU);
                    percent /= numProcessors;
                    percent *= 100.0;
                }
                lastCPU = now;
                lastSysCPU = timeSample.tms_stime;
                lastUserCPU = timeSample.tms_utime;
                c.ProcessUse = percent;
                return c;
            }
            MemoryUse getMemoryUsage()
            {

                struct sysinfo memInfo;
                sysinfo(&memInfo);

                long long int totalPhysMem = memInfo.totalram;
                // Multiply in next statement to avoid int overflow on right hand side...
                totalPhysMem *= memInfo.mem_unit;

                long long int physMemUsed = memInfo.totalram - memInfo.freeram;
                // Multiply in next statement to avoid int overflow on right hand side...
                physMemUsed *= memInfo.mem_unit;
                MemoryUse m;
                m.PhysicalTotalUsed = physMemUsed;
                m.PhysicalTotalAvailable = totalPhysMem;
                m.PhysicalProcessUsed = 0;

                long long int totalVirtualMem = memInfo.totalram;
                // Add other values in next statement to avoid int overflow on right hand side...
                totalVirtualMem += memInfo.totalswap;
                totalVirtualMem *= memInfo.mem_unit;

                long long int virtualMemUsed = memInfo.totalram - memInfo.freeram;
                // Add other values in next statement to avoid int overflow on right hand side...
                virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
                virtualMemUsed *= memInfo.mem_unit;

                m.VirtualTotalAvailable = totalVirtualMem;
                m.VirtualTotalUsed = virtualMemUsed;
                m.VirtualProcessUsed = 0;
                getprocessmemory(m);
                return m;
            }
        };
#else
#error "Unknown Operating System!"
#endif
    } // namespace NET
} // namespace SL
