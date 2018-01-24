#ifndef JOYTIME_INPUT_HIDAPI_HPP
#define JOYTIME_INPUT_HIDAPI_HPP

#include <vector>
#include "hidapi.h"
#include "joytime-core.hpp"
#include "joytime_input_hidapi_EXPORTS.h"

namespace Joytime {
  static const int vendor = 0x057e;
  enum class ProductIDs: uint16_t {
    LeftJoycon = 0x2006,
    RightJoycon = 0x2007,
    Pro = 0x2009,
  };
  namespace HIDAPI {
    enum class ConnectionType: uint8_t {
      Bluetooth,
      USB,
    };
    class Handle {
      public:
        hid_device* hidapiHandle;
        ConnectionType connection;
        Handle(ConnectionType, hid_device*);
    };
  };
  JOYTIME_INPUT_HIDAPI_EXPORT std::vector<Joytime::Controller> scanForControllers();
  JOYTIME_INPUT_HIDAPI_EXPORT void initController(Joytime::Controller*, bool = false);
}

#endif /* JOYTIME_INPUT_HIDAPI_HPP */
