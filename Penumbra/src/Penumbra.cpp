#include <EnGL/eglpch.h>
#include <EnGL.h>

#include <EnGL/Window.h>
#include <EnGL/Application.h>

#include <EnGL/Renderer/Shader.h>
#include <EnGL/Renderer/VertexArray.h>
#include <EnGL/Renderer/Renderer.h>
#include <EnGL/Renderer/Buffer.h>
#include <Platform/OpenGL/OpenGLBuffer.h>

#include <FreeImage/FreeImage.h>
#include <ctime>


std::shared_ptr<EnGL::Shader> m_SquareShader;
std::shared_ptr<EnGL::VertexBuffer> squareVB;
std::shared_ptr<EnGL::VertexArray> m_SquareVA;


class ExampleLayer : public EnGL::Layer
{
public:
	float x_move = 0.0;
	float y_move = 0.0;
	float x_trans = 0.0;
	float y_trans = 0.0;
	float trans = 0.0;
	float u_Z = 0.0;
	float u_X = 0.0;
	bool SCRSHT = false;
	bool PAUSE = false;
	int scr_ind = 0;
	char intStr[50];


	std::clock_t start =  std::clock();
	float duration;


	ExampleLayer()
		: Layer("Example")
		{
			// std::shared_ptr<EnGL::Shader> m_SquareShader;
			
			m_SquareVA.reset(EnGL::VertexArray::Create());

			EnGL::WindowProps props = EnGL::WindowProps();
			float WinHeight = 1080.0f;
			float WinWidth = 1920.0f;

			glm::mat4 proj = glm::ortho(0.0f, WinWidth, 0.0f, WinHeight, 0.0f, 1.0f);
			glm::mat4 mvp = proj;

			float squareVertices[3 * 4] = {
				0.0f, 0.0f, 0.0f,
				WinWidth, 0.0f, 0.0f,
				WinWidth, WinHeight, 0.0f,
				0.0f,  WinHeight, 0.0f
			};

			
			// squareVB.reset(EnGL::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
			squareVB.reset(EnGL::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

			squareVB->SetLayout({
				{ EnGL::ShaderDataType::Float3, "a_Position" }
			});

			m_SquareVA->AddVertexBuffer(squareVB);

			uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
			std::shared_ptr<EnGL::IndexBuffer> squareIB;
			squareIB.reset(EnGL::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
			m_SquareVA->SetIndexBuffer(squareIB);

			std::string uName = "u_MVP";
			m_SquareShader.reset(new EnGL::Shader("../res/shaders/Spongebob.shader"));
			m_SquareShader->SetUniformMat4f(uName, mvp);
			EGL_TRACE(m_SquareVA->GetIndexBuffer()->GetCount());

			m_SquareShader->Bind();
			EnGL::Renderer::Submit(m_SquareVA);
			EnGL::Renderer::EndScene();
			//m_SquareVA->Bind();
			// glClearColor(0.1f, 0.1f, 0.1f, 1);
			// glClear(GL_COLOR_BUFFER_BIT);

			// glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

		}

		void OnUpdate(std::shared_ptr<EnGL::Window> window)
		{
			EnGL::WindowProps props = EnGL::WindowProps();
			float WinHeight = 1080.0f;
			float WinWidth = 1920.0f;
			y_trans = (props.Height - WinHeight)/props.Height;
			x_trans = (props.Width - WinWidth)/props.Width;

			if (PAUSE) {
				start += (std::clock() - start) - duration;
			}
			else if(SCRSHT)
			{
				duration += 16;
				start += (std::clock() - start) - duration;
			}
			else {
				duration = (std::clock() - start);
			}
			

			std::string dump = "u_Time";
			std::string extension = ".jpg";

			std::sprintf(intStr, "%d", scr_ind);
			std::string str = std::string(intStr);

			std::string Screenshot_str = "Screenshot_"+str+extension;

			glm::mat4 view = glm::translate(glm::mat4(), glm::vec3(trans, 0.0f ,0.0f));
			glm::mat4 proj = glm::ortho(0.0f, WinWidth, 0.0f, WinHeight, 0.0f, 1.0f);
			glm::mat4 mvp = view * proj;
			std::string uName1 = "u_MVP";
			std::string uName2 = "u_Z"; 
			std::string uName3 = "u_Move";
			std::string uName4 = "u_X";
			m_SquareShader->SetUniform1f(dump, duration*0.001);
			m_SquareShader->SetUniformMat4f(uName1,mvp);
			m_SquareShader->SetUniform1f(uName2, u_Z);
			m_SquareShader->SetUniform1f(uName4, u_X);
			m_SquareShader->SetUniform2f(uName3,x_trans+x_move,y_trans+y_move);
			if (SCRSHT) {
				m_SquareShader->ScreenDump(Screenshot_str.c_str(),1920,1080);
				scr_ind += 1; 
			}

			if (EnGL::Input::IsKeyPressed(EGL_KEY_W)){
				EGL_TRACE("W was pressed (poll)!");
				u_Z += 0.1;

			}

			if (EnGL::Input::IsKeyPressed(EGL_KEY_S)){
				EGL_TRACE("W was pressed (poll)!");
				u_Z -= 0.1;
			}

			if (EnGL::Input::IsKeyPressed(EGL_KEY_A)){
				EGL_TRACE("W was pressed (poll)!");
				u_X += 0.1;
			}

			if (EnGL::Input::IsKeyPressed(EGL_KEY_D)){
				EGL_TRACE("W was pressed (poll)!");
				u_X -= 0.1;
			}

			if (EnGL::Input::IsKeyPressed(EGL_KEY_LEFT)){
				EGL_TRACE("Left was pressed (poll)!");
				x_move += 0.01;
			}

			if (EnGL::Input::IsKeyPressed(EGL_KEY_RIGHT)){
				EGL_TRACE("Right was pressed (poll)!");
				x_move -= 0.01;
			}

			if (EnGL::Input::IsKeyPressed(EGL_KEY_UP)){
				EGL_TRACE("Up was pressed (poll)!");
				y_move += 0.01;
			}

			if (EnGL::Input::IsKeyPressed(EGL_KEY_DOWN)){
				EGL_TRACE("Down was pressed (poll)!");
				y_move -= 0.01;
			}
			m_SquareShader->Bind();
			EnGL::Renderer::Submit(m_SquareVA);
			EnGL::Renderer::EndScene();
			//m_SquareVA->Bind();
			// EGL_TRACE(m_SquareVA->GetIndexBuffer()->GetCount());
			// glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);


		}

		void OnEvent(EnGL::Event& event)
		{
			if (event.GetEventType() == EnGL::EventType::KeyPressed)
			{
				EnGL::KeyPressedEvent& e = (EnGL::KeyPressedEvent&)event;
				if (e.GetKeyCode() == EGL_KEY_TAB){
					EGL_TRACE("Tab key is pressed (event)!");
					SCRSHT = !SCRSHT;
				}
				EGL_TRACE("{0}", (char)e.GetKeyCode());
			}

			if (event.GetEventType() == EnGL::EventType::KeyPressed)
			{
				EnGL::KeyPressedEvent& e = (EnGL::KeyPressedEvent&)event;
				if (e.GetKeyCode() == EGL_KEY_SPACE){
					EGL_TRACE("Space key is pressed (event)!");
					PAUSE = !PAUSE;
				}
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