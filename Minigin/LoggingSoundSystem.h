#pragma once
#include "ISoundSystem.h"

class LoggingSoundSystem final : public ISoundSystem
{
public:
	void PlaySound(const std::string& soundFile) override;
	void PlayMusic(const std::string& musicFile) override;
	void PauseAudio() override;
	void UnpauseAudio() override;
	void LowerSoundVolume(int amount) override;
	void IncreaseSoundVolume(int amount) override;
	void LowerMusicVolume(int amount) override;
	void IncreaseMusicVolume(int amount) override;

	explicit LoggingSoundSystem(ISoundSystem* realSoundSystem);
	~LoggingSoundSystem();

	LoggingSoundSystem(const LoggingSoundSystem& other) = delete;
	LoggingSoundSystem(LoggingSoundSystem&& other) noexcept = delete;
	LoggingSoundSystem& operator=(const LoggingSoundSystem& other) = delete;
	LoggingSoundSystem& operator=(LoggingSoundSystem&& other) noexcept = delete;
	
private:
	ISoundSystem* m_RealSoundSystem;
};

