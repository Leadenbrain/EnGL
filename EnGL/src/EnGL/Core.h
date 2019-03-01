#pragma once

#ifdef EGL_PLATFORM_WINDOWS
	#ifdef EGL_BUILD_DLL
		#define EGL_API __declspec(dllexport)
	#else
		#define EGL_API __declspec(dllimport)
	#endif
#elif EGL_PLATFORM_LINUX
	#define EGL_API
#else
	#error Windows is the only supported Platform
#endif

#define BIT(x) (1 << x)