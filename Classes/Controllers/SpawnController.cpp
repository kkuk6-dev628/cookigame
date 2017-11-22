#include "SpawnController.h"

SpawnController* SpawnController::instance = nullptr;
SpawnController::SpawnController()
{
}


SpawnController::~SpawnController()
{
}

SpawnController* SpawnController::getInstance()
{
	if (instance == nullptr)
		instance = new SpawnController();
	return instance;
}
