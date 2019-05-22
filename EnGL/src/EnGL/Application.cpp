#include "eglpch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"

//#include <glad/glad.h>
#include "Input.h"

namespace EnGL {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;
	
	Application::Application()
	{
		EGL_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		GLCall(glGenVertexArrays(1, &m_VertexArray));
		glBindVertexArray(m_VertexArray);

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float vertices[] = {
			   0.0f,    0.0f, 0.0f, 0.0f,
			1920.0f,    0.0f, 1.0f, 0.0f
			1920.0f, 1080.0f, 1.0f, 1.0f,
			   0.0f, 1080.0f, 0.0f, 1.0f
		};

		unsigned int indices[6] = { 
			0, 1, 2,
			2, 3, 0 
		};

		/*
		VertexArray va;
		VertexBuffer vb(vertices, 4*4*sizeof(float));
		
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb,layout);

		IndexBuffer ib(indices, 6);

		glm::mat4 proj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);

		Shader shader("res/shaders/Raymarch.shader");
		shader.Bind();

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();
		
		*/
		glBufferData(GL_ARRAY_BUFFER, 4*4*sizeof(float), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

/*			{
				glm::mat4 mvp = proj;//*view*matrix
				shader.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(va, ib, shader);
			}*/

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	
}