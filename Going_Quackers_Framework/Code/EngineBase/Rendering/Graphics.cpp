#include "Graphics.h"

Graphics* Graphics::getInstance()
{
	if (SingletonInstance == 0)
		SingletonInstance = new Graphics();

	return SingletonInstance;
}

bool Graphics::InitaliseAPIs()
{
	//- Create Temp Storage for APi -//
	GraphicsInterface* lp_interface;
	
	//- Instanciate new Api -//
	lp_interface = (GraphicsInterface*) new DirectXGraphics();

	//- Setup all elements required for engine -//
	if (!((DirectXGraphics*)CurrentGraphicsAPI)->InitalizeGraphicalApi())
	{
		throw std::runtime_error("DIRECT X API SETTUP FAILED");
		return false;
	}
	//- save to api database -//
	APIs.push_back(lp_interface);

	//- null pointer for next api -//
	lp_interface = nullptr;

	//- Duplicate code above for new API -//
	//- Template Code below -//

	//lp_interface = new YOUR API CLASS HERE(); <--------------------------------------------

	//if (!SetUpAPI(lp_interface))
	//{
	//	throw std::runtime_error("API SETTUP FAILED");
	//	return false;
	//}

	//lp_interface = nullptr;

	//- Allocate the current Grpahics Api -//
	CurrentGraphicsAPI = APIs[0];
	//((DirectXGraphics*)CurrentGraphicsAPI)->CreateEngineWindow();

	return true;
}