#pragma once

#include "Core.h"
#include "Window.h"
#include "Events/Event.h"
#include "EnGL/Renderer/Shader.h"

namespace EnGL {
	
	class EGL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(std::shared_ptr<Window> window) {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
		//std::unique_ptr<Shader> m_Shader;
	};
}