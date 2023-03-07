
#ifndef SIMPLEBLE_EXPORT_H
#define SIMPLEBLE_EXPORT_H

#ifdef SIMPLEBLE_STATIC_DEFINE
#  define SIMPLEBLE_EXPORT
#  define SIMPLEBLE_NO_EXPORT
#else
#  ifndef SIMPLEBLE_EXPORT
#    ifdef simpleble_EXPORTS
        /* We are building this library */
#      define SIMPLEBLE_EXPORT 
#    else
        /* We are using this library */
#      define SIMPLEBLE_EXPORT 
#    endif
#  endif

#  ifndef SIMPLEBLE_NO_EXPORT
#    define SIMPLEBLE_NO_EXPORT 
#  endif
#endif

#ifndef SIMPLEBLE_DEPRECATED
#  define SIMPLEBLE_DEPRECATED __declspec(deprecated)
#endif

#ifndef SIMPLEBLE_DEPRECATED_EXPORT
#  define SIMPLEBLE_DEPRECATED_EXPORT SIMPLEBLE_EXPORT SIMPLEBLE_DEPRECATED
#endif

#ifndef SIMPLEBLE_DEPRECATED_NO_EXPORT
#  define SIMPLEBLE_DEPRECATED_NO_EXPORT SIMPLEBLE_NO_EXPORT SIMPLEBLE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef SIMPLEBLE_NO_DEPRECATED
#    define SIMPLEBLE_NO_DEPRECATED
#  endif
#endif

#endif /* SIMPLEBLE_EXPORT_H */
