#include "../../EnGL/src/EnGL/eglpch.h"
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
			EGL_INFO("ExampleLayer::Update");
		}

		void OnEvent(EnGL::Event& event)
		{
			EGL_TRACE("{0}", event);
		}
};

class Penumbra : public EnGL::Application
{
public:
	Penumbra()
	{
		PushLayer(new ExampleLayer());
	}

	~Penumbra()
	{

	}
};

EnGL::Application* EnGL::CreateApplication()
{
	return new Penumbra();
}