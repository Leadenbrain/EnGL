#include "eglpch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>

namespace EnGL {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
	
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		
		EGL_CORE_INFO("{0}", e);
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1,0,1,1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	
}