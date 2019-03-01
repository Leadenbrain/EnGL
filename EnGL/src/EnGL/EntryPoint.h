#pragma once

#ifdef EGL_PLATFORM_WINDOWS

	extern EnGL::Application* EnGL::CreateApplication();

	int main(int argc, char** argv)
	{
		EnGL::Log::Init();
		EnGL::Log::GetCoreLogger()->warn("Initialized Core Log");
		EnGL::Log::GetClientLogger()->info("Initialized Client Log");

		printf("EnGL Version Zero\n");
		auto app = EnGL::CreateApplication();
		app->Run();
		delete app;
	}

#endif