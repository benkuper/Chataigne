#pragma once
#include <cinttypes>
#include <memory>
#include <ossia-config.hpp>
#include <ossia_export.h>
#include <string>
#include <vector>

namespace servus
{
class Servus;
}

namespace ossia
{
namespace net
{
struct OSSIA_EXPORT minuit_connection_data
{
  std::string name;
  std::string host;
  int32_t remote_port{};
  int32_t local_port{};
};

struct OSSIA_EXPORT oscquery_connection_data
{
  std::string name;
  std::string host;
  int32_t port{};
};

struct OSSIA_EXPORT zeroconf_server
{
  zeroconf_server();
  zeroconf_server(const zeroconf_server&) = delete;
  zeroconf_server(zeroconf_server&&);
  zeroconf_server& operator=(const zeroconf_server&) = delete;
  zeroconf_server& operator=(zeroconf_server&&);
  ~zeroconf_server();

  zeroconf_server(std::unique_ptr<servus::Servus> s);

private:
  std::unique_ptr<servus::Servus> server;
};

/**
 * @brief zeroconf_supported
 * @return true if the library was built with zeroconf support
 */
OSSIA_EXPORT bool zeroconf_supported();

/**
 * @brief list_minuit_devices
 *
 * @note This function blocks for five seconds.
 *
 * @return a list of the zeroconf-enabled minuit clients
 */
OSSIA_EXPORT std::vector<minuit_connection_data> list_minuit_devices();

/**
 * @brief list_oscquery_devices
 *
 * @note This function blocks for five seconds
 *
 * @return A list of the zeroconf-enabled oscquery clients.
 */
OSSIA_EXPORT std::vector<oscquery_connection_data> list_oscquery_devices();

/**
 * @brief make_zeroconf_server
 *
 * The available services names are :
 * * Minuit: _minuit._tcp
 *
 * @return A running Zeroconf server exposing the device info.
 */
OSSIA_EXPORT zeroconf_server make_zeroconf_server(
    std::string description, std::string service, std::string local_name,
    int32_t local_port, int32_t remote_port);
}
}
