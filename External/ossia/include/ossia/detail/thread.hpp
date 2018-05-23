#pragma once
#include <ossia/detail/config.hpp>
#include <ossia_export.h>
#include <thread>

namespace ossia
{
OSSIA_EXPORT
void set_thread_realtime(std::thread& t);

OSSIA_EXPORT
std::string get_exe_path();

OSSIA_EXPORT
std::string get_module_path();

OSSIA_EXPORT
int get_pid();
}
