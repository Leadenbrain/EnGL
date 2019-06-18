#include <EnGL/eglpch.h>
#include <EnGL.h>
#include <FreeImage/FreeImage.h>
#include <EnGL/Window.h>
#include <EnGL/Renderer/Shader.h>
#include <EnGL/Application.h>
#include <ctime>



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

		}

		void OnUpdate(std::shared_ptr<EnGL::Window> window, std::shared_ptr<EnGL::Shader> shader)
		{
			EnGL::WindowProps props = EnGL::WindowProps();
			float WinHeight = window->GetHeight();
			float WinWidth = window->GetWidth();
			y_trans = (props.Height - WinHeight)/props.Height;
			x_trans = (props.Width - WinWidth)/props.Width;

			if (PAUSE) {
				start += (std::clock() - start) - duration;
			}
			else if(!SCRSHT)
			{
				duration = (std::clock() - start);
			}
			else {
				duration += 16;
				start += (std::clock() - start) - duration;
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
			shader->SetUniform1f(dump, duration*0.001);
			shader->SetUniformMat4f(uName1,mvp);
			shader->SetUniform1f(uName2, u_Z);
			shader->SetUniform1f(uName4, u_X);
			shader->SetUniform2f(uName3,x_trans+x_move,y_trans+y_move);
			if (SCRSHT) {
				shader->ScreenDump(Screenshot_str.c_str(),1920,1080);
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