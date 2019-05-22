#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm> 
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Log.h"

#include <glad/glad.h>

#define ASSERT(x) if (!(x)) __builtin_trap();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

#ifdef EGL_PLATFORM_WINDOWS
	#include <Windows.h>
#endif