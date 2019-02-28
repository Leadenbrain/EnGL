#pragma once

#ifdef EGL_PLATFORM_WINDOWS

extern EnGL::Application* EnGL::CreateApplication();

int main(int argc, char** argv)
{
	printf("EnGL Version 0.0\n");
	auto app = EnGL::CreateApplication();
	app->Run();
	delete app;
}

#else
	#error EGL Only Suppoted on Windows
#endif
