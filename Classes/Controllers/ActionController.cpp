#include "ActionController.h"

ActionController* ActionController::instance = nullptr;

ActionController::ActionController()
{
}


ActionController::~ActionController()
{
}

ActionController* ActionController::getInstance()
{
	if (instance == nullptr)
	{
		instance = new ActionController();
	}
	return instance;
}
