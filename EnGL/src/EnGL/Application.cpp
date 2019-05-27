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

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		/*float vertices[3 * 3] = {
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 0.0f,  1.0f, 0.0f
		};*/

		float vertices[] = {
			-1.0f, -1.0f, 
			 1.0f, -1.0f, 
			 1.0f,  1.0f, 
			-1.0f,  1.0f 
		};

/*		vertices[] = {
			   0.0f,   0.0f,
			1280.0f,   0.0f,
			1280.0f, 720.0f,
			   0.0f, 720.0f
		}

		glm::mat4 proj = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
		glm::mat4 mvp = proj;*/


		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		std::string vertexSource = R"(
				#version 330 core

				layout(location = 0)in vec4 a_Position;
				out vec4 v_Position;

				void main()
				{
					gl_Position = a_Position;
					v_Position = a_Position;
				}
		)";

		std::string fragmentSource = R"(
				#version 330 core

				layout(location = 0) out vec4 colour;

				in vec4 v_Position;

				float spheres(vec3 z)
				{
					z = mod((z),1.0)-vec3(0.5);
					return length(z)-0.1;
				}

				float Scale = 2.0;
				int Iterations = 10;
				float Power = 8.0;
				float Bailout = 1.15;
				float Offset = 1.0;
				float min_dist = 0.0000005;
				float max_dist = 50.0;

				vec4 ray_march(vec3 o, vec3 r, float sharpness) {
					float d = min_dist;
					float s = 0.0;
					float t= 0.0;
					float min_d = 1.0;
					for (; s < 50; s += 1.0) { 
						vec3 p = o + r*t;
						d = spheres(p);
						if ( d < min_dist ) {
							s += d/min_dist;
							break;
						} else if ( t > 50.0) {
							break;
						}
						t += d;
						min_d = min(min_d,sharpness*d/t);
					}
					return vec4(d,s,t,min_d);
				};

				void main()
				{
					vec2 uv = 2*v_Position.xy-1.0;
					vec3 r = normalize(vec3(uv, 1.0));
						vec3 o = vec3(0.0,0.0,-2.0);
					vec4 e = vec4(min_dist, 0.0,0.0,0.0);
					vec3 n = normalize(vec3(spheres(r + e.xyy) - spheres(r - e.xyy),\
						spheres(r + e.yxy) - spheres(r - e.yxy),\
						spheres(r + e.yyx) - spheres(r - e.yyx)));

					vec3 light_dir = vec3(0.0);

				    
				    // ambient occlusion
				//    vec2 rmvec = trace(o,r);
					vec4 rmvec = ray_march(o,r,4.0);
				    float d = rmvec.x;
				    float s = rmvec.y;
				    float t = rmvec.z;
				    float m = rmvec.w;
				    float a = 1.0 / (1.0 + t + 0.01);
				    vec3 col = vec3(0.0);
				    vec3 orig_col = vec3(0.8,0.3,0.5)*255;
				    if (d < min_dist) {
				    vec3 reflected = r.xyz - 2.0*dot(r.xyz,n)*n;
				    
					float k = 2.0;
					vec3 light_pt = vec3(-1.0,-1.0,10.0);
					light_pt.xyz += n*min_dist * 0.01;
					vec4 rm = ray_march(light_pt, light_dir,4.0);
					k = rm.w * min(rm.z,1.0);
					
					
					float specular = max(dot(reflected, light_dir),0.0);
					specular = pow(specular, 16);
					col += specular*k;
					
					k = min(k, 0.8* 0.5 * (dot(n,light_dir) - 1.0) + 1.0);
					k = max(k,0.2);
					col += orig_col*k;
					
				    //vec3 col = 0.5 + 0.5*cos(u_Time*0.001+uv.xyx+vec3(0.1*t,0.2*t,0.4*t));
				    col += (1.0 - a)*vec3(0.8);
				    a = t / max_dist;
				    col = (1.0 - a)*col*a*vec3(0.2,0.8,0.5);

				    float fog = 1.0/(1.0+t*0.009);
				    vec3 fc = vec3(fog,0.5*fog, 0.2*fog);
				    } else {
				    	col += vec3(0.8,0.3,0.5);
				    	//col += (1.0 - m) * (1.0 - m) * vec3(-0.2,0.5,-0.2);
				    }
				    vec4 fragColour = vec4(col,1.0);
					colour = fragColour;
				}
		)";

		m_Shader.reset(new Shader("../res/shaders/Raymarch.shader"));

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

			m_Shader->Bind();

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