#pragma once

class ISoundSystem
{
public:
	virtual ~ISoundSystem() {}
	virtual void PlaySound(const std::string& soundFile) = 0;
	virtual void PlayMusic(const std::string& musicFile) = 0;
	virtual void PauseAudio() = 0;
	virtual void UnpauseAudio() = 0;
	virtual void LowerSoundVolume(int amount) = 0;
	virtual void IncreaseSoundVolume(int amount) = 0;
	virtual void LowerMusicVolume(int amount) = 0;
	virtual void IncreaseMusicVolume(int amount) = 0;
};
