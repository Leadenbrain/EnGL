#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace EnGL {
	class EGL_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define EGL_CORE_FATAL(...) 	::EnGL::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define EGL_CORE_ERROR(...) 	::EnGL::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EGL_CORE_WARN(...) 		::EnGL::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EGL_CORE_INFO(...) 		::EnGL::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EGL_CORE_TRACE(...) 	::EnGL::Log::GetCoreLogger()->trace(__VA_ARGS__)

#define EGL_FATAL(...) 			::EnGL::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define EGL_ERROR(...) 			::EnGL::Log::GetClientLogger()->error(__VA_ARGS__)
#define EGL_WARN(...) 			::EnGL::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EGL_INFO(...) 			::EnGL::Log::GetClientLogger()->info(__VA_ARGS__)
#define EGL_TRACE(...) 			::EnGL::Log::GetClientLogger()->trace(__VA_ARGS__)