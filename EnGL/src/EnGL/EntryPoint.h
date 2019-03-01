#pragma once

#ifdef EGL_PLATFORM_WINDOWS

	extern EnGL::Application* EnGL::CreateApplication();

	int main(int argc, char** argv)
	{
		EnGL::Log::Init();
		EGL_CORE_WARN("Initialized Core Log");
		EGL_INFO("Initialized Client Log");

		auto app = EnGL::CreateApplication();
		app->Run();
		delete app;
	}

#endif