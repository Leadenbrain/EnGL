#include "eglpch.h"
#include "Application.h"

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

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		/*float vertices[3 * 3] = {
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 0.0f,  1.0f, 0.0f
		};*/

/*		float vertices[] = {
			-1.0f, -1.0f, 
			 1.0f, -1.0f, 
			 1.0f,  1.0f, 
			-1.0f,  1.0f 
		};*/

		float vertices[] = {
			   0.0f,   0.0f,
			1920.0f,   0.0f,
			1920.0f, 1080.0f,
			   0.0f, 1080.0f
		};

		// glm::mat4 proj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);
		// glm::mat4 mvp = proj;


/*		VertexArray* m_VertexArray = new VertexArray();
		VertexBuffer* m_VertexBuffer = new VertexBuffer(vertices, 2*4*sizeof(float));

		VertexBufferLayout* m_layout = new VertexBufferLayout();

		m_layout->Push<float>(2);
		m_VertexArray->AddBuffer(m_VertexBuffer, m_layout);*/

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

		unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };
		IndexBuffer* m_IndexBuffer = new IndexBuffer(indices, 6);

		// std::string uName = "u_MVP";
		m_Shader.reset(new Shader("../res/shaders/Raymarch.shader"));
		// m_Shader->SetUniformMat4f(uName, mvp);

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

			glm::mat4 mvp = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, 0.0f, 1.0f);

			std::string uName = "u_MVP";

			m_Shader->Bind();
			m_Shader->SetUniformMat4f(uName,mvp);

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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