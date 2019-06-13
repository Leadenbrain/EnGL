#include "eglpch.h"
#include "Application.h"

#include "Input.h"

namespace EnGL {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case EnGL::ShaderDataType::Float:    return GL_FLOAT;
			case EnGL::ShaderDataType::Float2:   return GL_FLOAT;
			case EnGL::ShaderDataType::Float3:   return GL_FLOAT;
			case EnGL::ShaderDataType::Float4:   return GL_FLOAT;
			case EnGL::ShaderDataType::Mat3:     return GL_FLOAT;
			case EnGL::ShaderDataType::Mat4:     return GL_FLOAT;
			case EnGL::ShaderDataType::Int:      return GL_INT;
			case EnGL::ShaderDataType::Int2:     return GL_INT;
			case EnGL::ShaderDataType::Int3:     return GL_INT;
			case EnGL::ShaderDataType::Int4:     return GL_INT;
			case EnGL::ShaderDataType::Bool:     return GL_BOOL;
		}

		EGL_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}	

	Application* Application::s_Instance = nullptr;
	
	Application::Application()
	{
		EGL_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;
		WindowProps props = WindowProps();

		m_Window = std::unique_ptr<Window>(Window::Create(props));
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float WinHeight = m_Window->GetHeight();
		float WinWidth = m_Window->GetWidth();

		float vertices[] = {
			   0.0f,   0.0f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			WinWidth,   0.0f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			WinWidth, WinHeight,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
			   0.0f, WinHeight, 0.0f, 0.3f, 0.8f, 0.2f, 1.0f
		};

/*		float vertices[3 * 7] = {
			640.0f, 360.0, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 1280.0f, 360.0, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 960.0f, 720.0f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};	*/	

		float y_trans = (props.Height - WinHeight)/props.Height;
		float x_trans = (props.Width - WinWidth)/props.Width;


		glm::mat4 view = glm::translate(glm::mat4(), glm::vec3(x_trans, y_trans ,0.0f));
		glm::mat4 proj = glm::ortho(0.0f, WinWidth, 0.0f, WinHeight, -1.0f, 1.0f);
		glm::mat4 mvp = view*proj;


		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Colour" }
			};

			m_VertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		 // uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string uName = "u_MVP";
		m_Shader.reset(new Shader("../res/shaders/Snake.shader"));
		m_Shader->SetUniformMat4f(uName, mvp);

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
		Shader* shader = nullptr;
		Window* window = nullptr;

		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			
			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			shader = m_Shader.get();
			window = m_Window.get();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(window, shader);
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	
}