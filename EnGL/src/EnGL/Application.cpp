#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace EnGL {
	
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			EGL_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			EGL_TRACE(e);
		}
		while (true);
	}
	
}