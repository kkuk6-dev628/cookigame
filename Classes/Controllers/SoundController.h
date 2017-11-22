#pragma once
class SoundController
{
public:
	~SoundController();
	static SoundController* getInstance();

private:
	SoundController();
	static SoundController* instance;
};

