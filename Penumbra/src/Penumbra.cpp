#include "../../EnGL/src/EnGL/eglpch.h"
#include <EnGL.h>

class Penumbra : public EnGL::Application
{
public:
	Penumbra()
	{

	}

	~Penumbra()
	{

	}
};

EnGL::Application* EnGL::CreateApplication()
{
	return new Penumbra();
}