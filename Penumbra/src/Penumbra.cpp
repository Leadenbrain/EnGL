#include <EnGL/eglpch.h>
#include <EnGL.h>

class ExampleLayer : public EnGL::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
		{

		}

		void OnUpdate()
		{
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
		PushOverlay(new EnGL::ImGuiLayer());
	}

	~Penumbra()
	{

	}
};

EnGL::Application* EnGL::CreateApplication()
{
	return new Penumbra();
}