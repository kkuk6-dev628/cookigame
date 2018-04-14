#include "SoundController.h"
#include "SimpleAudioEngine.h"
#include "base/CCUserDefault.h"
#include "General/Utils.h"


using namespace CocosDenshion;

#define sae		SimpleAudioEngine::getInstance()

#define BgMusic_Volume	0.7f
#define Effects_Volume	1.0f

SoundController* SoundController::instance = nullptr;

const char* szMusicFiles[] =
{
	"music/music_title.mp3",
	"music/music_game_alt.mp3",
};

SoundController::SoundController()
{
	sae->setBackgroundMusicVolume(BgMusic_Volume);
	sae->setEffectsVolume(Effects_Volume);
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

void SoundController::preloadSounds()
{
	int i;
	for (i = 0; i < (int)MusicKind::_MusicCount; i++)
		sae->preloadBackgroundMusic(szMusicFiles[i]);

	for(auto se : SoundEffects::_values())
	{
		sae->preloadEffect(StringUtils::format("sound/%s.mp3", se._to_string()).c_str());
	}
}

void SoundController::playBgMusic(MusicKind kind)
{
	//sae->setBackgroundMusicVolume(0.3);

	sae->playBackgroundMusic(szMusicFiles[(int)kind], true);
	if (!_bgMusicOn)
	{
		sae->pauseBackgroundMusic();
		//cocos2d::log("---------------Background music is paused!-----------------");
	}
}

void SoundController::pauseBgMusic()
{
	sae->pauseBackgroundMusic();
}

void SoundController::resumeBgMusic()
{
	if (_bgMusicOn)
		sae->resumeBackgroundMusic();
}

void SoundController::setBgMusicVolume(float volume)
{
	sae->setBackgroundMusicVolume(volume);
}

void SoundController::playEffectSound(SoundEffects effect)
{
	if (_effectsOn && SoundEffects::_is_valid(effect))
		sae->playEffect(StringUtils::format("sound/%s.mp3", effect._to_string()).c_str());
}

void SoundController::playEffectSound(std::string effectName, char matchNumber)
{
	if (_effectsOn)
	{
		auto soundName = StringUtils::format("sound/sound_gem_match_tier%d.mp3", matchNumber > 7 ? 8 : matchNumber);
		sae->playEffect(soundName.c_str());
	}
}


void SoundController::toggleBgMusicOn()
{
	_bgMusicOn = !_bgMusicOn;
	//sae->setBackgroundMusicVolume(_bgMusicOn ? BgMusic_Volume : 0);
	if (_bgMusicOn)
		sae->resumeBackgroundMusic();
	else
		sae->pauseBackgroundMusic();
	saveSettings();
}

void SoundController::toggleEffectsOn()
{
	_effectsOn = !_effectsOn;
	//sae->setEffectsVolume(_effectsOn ? Effects_Volume : 0);
	if (!_effectsOn)
		sae->stopAllEffects();
	saveSettings();
}

void SoundController::readSettings()
{
	_bgMusicOn = cocos2d::UserDefault::getInstance()->getBoolForKey("bgMusicOn", true);
	_effectsOn = cocos2d::UserDefault::getInstance()->getBoolForKey("soundEffectsOn", true);
	sae->setBackgroundMusicVolume(_bgMusicOn ? BgMusic_Volume : 0);
	sae->setEffectsVolume(_effectsOn ? Effects_Volume : 0);
}

void SoundController::saveSettings()
{
	cocos2d::UserDefault* ud = cocos2d::UserDefault::getInstance();
	ud->setBoolForKey("bgMusicOn", _bgMusicOn);
	ud->setBoolForKey("soundEffectsOn", _effectsOn);
	ud->flush();
}
