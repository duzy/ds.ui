#ifndef __DSO_VISIBILITY_hpp____by_Duzy_Chan__
#define __DSO_VISIBILITY_hpp____by_Duzy_Chan__ 1
#if defined _WIN32 || defined __CYGWIN__
#  ifdef DS_BUILDING_DSO
#    ifdef __GNUC__
#      define DSO_PUBLIC __attribute__((dllexport))
#    else
#      define DSO_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#    endif
#  else
#    ifdef __GNUC__
#      define DSO_PUBLIC __attribute__((dllimport))
#    else
#      define DSO_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#    endif
#  endif /* DS_BUILDING_DSO */
#  define DSO_PRIVATE
#else
#  if __GNUC__ >= 4
#    define DSO_PUBLIC __attribute__ ((visibility("default")))
#    define DSO_PRIVATE  __attribute__ ((visibility("hidden")))
#  else
#    define DSO_PUBLIC
#    define DSO_PRIVATE
#  endif
#endif
#endif//__DSO_VISIBILITY_hpp____by_Duzy_Chan__
