
#ifndef JOYTIME_CORE_EXPORT_H
#define JOYTIME_CORE_EXPORT_H

#ifdef JOYTIME_CORE_BUILT_AS_STATIC
#  define JOYTIME_CORE_EXPORT
#  define JOYTIME_CORE_NO_EXPORT
#else
#  ifndef JOYTIME_CORE_EXPORT
#    ifdef joytime_core_EXPORTS
        /* We are building this library */
#      define JOYTIME_CORE_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define JOYTIME_CORE_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef JOYTIME_CORE_NO_EXPORT
#    define JOYTIME_CORE_NO_EXPORT 
#  endif
#endif

#ifndef JOYTIME_CORE_DEPRECATED
#  define JOYTIME_CORE_DEPRECATED __declspec(deprecated)
#endif

#ifndef JOYTIME_CORE_DEPRECATED_EXPORT
#  define JOYTIME_CORE_DEPRECATED_EXPORT JOYTIME_CORE_EXPORT JOYTIME_CORE_DEPRECATED
#endif

#ifndef JOYTIME_CORE_DEPRECATED_NO_EXPORT
#  define JOYTIME_CORE_DEPRECATED_NO_EXPORT JOYTIME_CORE_NO_EXPORT JOYTIME_CORE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef JOYTIME_CORE_NO_DEPRECATED
#    define JOYTIME_CORE_NO_DEPRECATED
#  endif
#endif

#endif
