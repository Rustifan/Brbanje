
#include "Brbanje.h"
//Entry
#include "Brbanje/Core/EntryPoint.h"


#include "BrbeetorLayer.h"



namespace Brbanje
{


	class Brbeetor : public Application
	{
	public:
		Brbeetor():Application("Brbeetor Editor")
		{
			//PushLayer(new ExampleLayer);
			PushLayer(new BrbeetorLayer());
		}

		~Brbeetor()
		{

		}



	};


	Application* Brbanje::CreateApplication()
	{
		return new Brbeetor();
	}

}