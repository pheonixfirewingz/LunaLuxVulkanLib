#ifndef _PLATFORM_
#define _PLATFORM_
/*------------------------------------------------------------------------
 * this header file is used to detected the platform and control. it is used define macros
 * that are used to stop platform pacific code from being compiled on the platform it
 * was never written to run on.
 *
 * FOR WINDOWS:
 * LLE_WINDOWS macro is defined.
 *
 * FOR APPLE:
 * LLE_MAC macro is defined for mac builds.
 * LLE_IPHONE macro is defined for ipad and iphone builds.
 * LLE_IPHONE_EMULATOR is for when you use the emulation on mac.
 *
 * FOR LINUX:
 * LLE_LINUX macro is defined.
 *
 * FOR OTHER PLATFORMS:
 * it is set up so it will cause a compiler error as this library will never support the platform.
 * --------------------------------------------------------------------------
 */
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64) || defined(_WINDOWS) || defined(__CYGWIN__) && !defined(_WIN32)
#define LLE_WINDOWS
#endif
#if defined(__linux__)
#define LLE_LINUX
#endif
#if defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR == 1
        #define LLE_IPHONE_EMULATOR
    #elif TARGET_OS_IPHONE == 1
        #define LLE_IPHONE
    #elif TARGET_OS_MAC == 1
        #define LLE_MAC
    #endif
#endif
#if defined(__sun) && defined(__SVR4) || defined(BSD) || defined(__QNX__)
#error "this is not designed to run on this Enviroment"
#endif
#endif