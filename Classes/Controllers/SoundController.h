#pragma once
#include <string>

class SoundEffects;

class SoundController
{
public:
	~SoundController();
	static SoundController* getInstance();

	enum MusicKind
	{
		musicGameMenu,
		musicGamePlay,
		_MusicCount
	};

	void preloadSounds();
	void playBgMusic(MusicKind kind);
	void pauseBgMusic();
	void resumeBgMusic();
	void setBgMusicVolume(float volume);

	void playEffectSound(SoundEffects effect);
	void playEffectSound(std::string effectName, char matchNumber);

	void toggleBgMusicOn();
	void toggleEffectsOn();

	bool isBgMusicOn() { return _bgMusicOn; }
	bool isEffectsOn() { return _effectsOn; }

	void readSettings();
	void saveSettings();

private:
	bool _bgMusicOn = true;
	bool _effectsOn = true;

	SoundController();
	static SoundController* instance;
};

