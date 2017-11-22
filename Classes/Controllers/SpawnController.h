#pragma once

class SpawnController
{
public:
	
	~SpawnController();
	static SpawnController* getInstance();

private:
	SpawnController();
	static SpawnController* instance;
};

