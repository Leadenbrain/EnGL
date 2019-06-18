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
		
		WindowProps props = WindowProps();
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_VertexArray.reset(VertexArray::Create());

		float WinHeight = m_Window->GetHeight();
		float WinWidth = m_Window->GetWidth();

		float vertices[] = {
			WinWidth/3.0f, WinHeight/3.0f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			2.0f*WinWidth/3.0f, WinHeight/3.0f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			WinWidth/2.0f, 2.0f*WinHeight/3.0f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		/*float vertices[3 * 7] = {
			640.0f, 360.0, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 1280.0f, 360.0, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 960.0f, 720.0f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};*/		

		float y_trans = (props.Height - WinHeight)/props.Height;
		float x_trans = (props.Width - WinWidth)/props.Width;


		glm::mat4 view = glm::translate(glm::mat4(), glm::vec3(x_trans, y_trans ,0.0f));
		glm::mat4 proj = glm::ortho(0.0f, WinWidth, 0.0f, WinHeight, -1.0f, 1.0f);
		glm::mat4 mvp = view*proj;



		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Colour" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(VertexArray::Create());

		float squareVertices[3 * 4] = {
			0.0f, 0.0f, 0.0f,
			WinWidth, 0.0f, 0.0f,
			WinWidth, WinHeight, 0.0f,
			0.0f,  WinHeight, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
		});

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string uName = "u_MVP";

		m_Shader.reset(new Shader("../res/shaders/Test.shader"));
		m_SquareShader.reset(new Shader("../res/shaders/FluidGlass.shader"));
		m_Shader->SetUniformMat4f(uName, mvp);
		m_SquareShader->SetUniformMat4f(uName, mvp);

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
		Window* Window = nullptr;

		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			float WinHeight = m_Window->GetHeight();
			float WinWidth = m_Window->GetWidth();

			WindowProps props = WindowProps();

			float y_trans = (props.Height - WinHeight)/props.Height;
			float x_trans = (props.Width - WinWidth)/props.Width;


			glm::mat4 view = glm::translate(glm::mat4(), glm::vec3(x_trans, y_trans ,0.0f));
			glm::mat4 proj = glm::ortho(0.0f, WinWidth, 0.0f, WinHeight, -1.0f, 1.0f);
			glm::mat4 mvp = view*proj;

			std::string uName = "u_MVP";



			m_SquareShader->Bind();
			m_SquareVA->Bind();
			// m_SquareShader->SetUniformMat4f(uName, mvp);
			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
			
			for (Layer* layer : m_LayerStack)
				{ layer->OnUpdate(m_Window, m_SquareShader);
			m_Shader->Bind();
			m_Shader->SetUniformMat4f(uName, mvp);
				m_VertexArray->Bind();
				glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
			} 
			m_Window->OnUpdate();

			// ScreenDump(1920,1080);
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	
}