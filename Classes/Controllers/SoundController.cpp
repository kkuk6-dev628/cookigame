#include "SoundController.h"

SoundController* SoundController::instance = nullptr;

SoundController::SoundController()
{
}


SoundController::~SoundController()
{
}

SoundController* SoundController::getInstance()
{
	if (instance == nullptr)
	{
		instance = new SoundController();
	}
	return instance;
}
