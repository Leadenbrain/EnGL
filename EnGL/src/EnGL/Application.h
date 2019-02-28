#pragma once

#include "Core.h"

namespace EnGL {
	
	class EGL_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void Run();
	};

	// Define in Client
	Application* CreateApplication();

}