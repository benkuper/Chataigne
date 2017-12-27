#ifndef OSSIA_LOG_H
#define OSSIA_LOG_H
#include <ossia_export.h>

#if defined(__cplusplus)
extern "C"
{
#endif


typedef void (*ossia_log_fun_t)(const char *);
OSSIA_EXPORT
void ossia_set_debug_logger(ossia_log_fun_t fp);
OSSIA_EXPORT
void ossia_log_error(const char* error);


#if defined(__cplusplus)
}
#endif
#endif
