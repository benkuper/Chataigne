#ifndef JOYTIME_INPUT_HIDAPI_H
#define JOYTIME_INPUT_HIDAPI_H
#include "joytime-core.h"
#include "joytime_input_hidapi_EXPORTS.h"

/*
 * This is a header for C clients of the library.
 * If you're using C++, you'll probably want to use "joytime-input-hidapi.hpp"
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static const int Joytime_vendor = 0x057e;

enum Joytime_ProductIDs {
  LeftJoyconID = 0x2006,
  RightJoyconID = 0x2007,
  ProID = 0x2009,
};

JOYTIME_INPUT_HIDAPI_EXPORT Joytime_Controller** Joytime_scanForControllers(int* resultingSize);
JOYTIME_INPUT_HIDAPI_EXPORT void Joytime_initController(Joytime_Controller* controller, bool calibrate);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* JOYTIME_INPUT_HIDAPI_H */
