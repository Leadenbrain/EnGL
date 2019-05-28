#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "EnGL/Renderer/Shader.h"
#include "EnGL/Renderer/IndexBuffer.h"
#include "EnGL/Renderer/VertexBuffer.h"
#include "EnGL/Renderer/VertexArray.h"
#include "EnGL/Renderer/VertexBufferLayout.h"

namespace EnGL {
	
	class EGL_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return * m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBufferLayout> m_layout;

	private:
		static Application* s_Instance;
	};

	// Define in Client
	Application* CreateApplication();

}