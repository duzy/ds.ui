#ifndef __DSO_VISIBILITY_h____by_Duzy_Chan__
#define __DSO_VISIBILITY_h____by_Duzy_Chan__ 1
#if defined _WIN32 || defined __CYGWIN__
#  ifdef DS_BUILDING_UI
#    ifdef __GNUC__
#      define DS_UI_PUBLIC __attribute__((dllexport))
#    else
#      define DS_UI_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#    endif
#  else
#    ifdef __GNUC__
#      define DS_UI_PUBLIC __attribute__((dllimport))
#    else
#      define DS_UI_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#    endif
#  endif /* DS_BUILDING_UI */
#  define DS_UI_PRIVATE
#else
#  if __GNUC__ >= 4
#    define DS_UI_PUBLIC __attribute__ ((visibility("default")))
#    define DS_UI_PRIVATE  __attribute__ ((visibility("hidden")))
#  else
#    define DS_UI_PUBLIC
#    define DS_UI_PRIVATE
#  endif
#endif
#endif//__DSO_VISIBILITY_h____by_Duzy_Chan__
