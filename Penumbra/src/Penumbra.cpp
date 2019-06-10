#include <EnGL/eglpch.h>
#include <EnGL.h>
#include <EnGL/Renderer/Shader.h>
#include <EnGL/Application.h>



class ExampleLayer : public EnGL::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
		{

		}

		void OnUpdate()
		{
			/*glm::mat4 proj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, 0.0f, 1.0f);
			std::string uName = "u_MVP";*/
			//m_Shader->SetUniformMat4f(uName,proj);

			if (EnGL::Input::IsKeyPressed(EGL_KEY_TAB))
				EGL_TRACE("Tab was pressed (poll)!");
		}

		void OnEvent(EnGL::Event& event)
		{
			if (event.GetEventType() == EnGL::EventType::KeyPressed)
			{
				EnGL::KeyPressedEvent& e = (EnGL::KeyPressedEvent&)event;
				if (e.GetKeyCode() == EGL_KEY_TAB)
					EGL_TRACE("Tab key is pressed (event)!");
				EGL_TRACE("{0}", (char)e.GetKeyCode());
			}
		}
};

class Penumbra : public EnGL::Application
{
public:
	Penumbra()
	{
		PushLayer(new ExampleLayer());
		//PushOverlay(new EnGL::ImGuiLayer());
	}

	~Penumbra()
	{

	}
};

EnGL::Application* EnGL::CreateApplication()
{
	return new Penumbra();
}