#pragma once
#include "ISoundSystem.h"

namespace dae
{
	class NullSoundSystem final : public ISoundSystem {
	public:
		void PlaySound(const std::string&) override {}
		void PlayMusic(const std::string&) override {}
		void PauseAudio() override {}
		void UnpauseAudio() override {}
		void LowerSoundVolume(int) override {}
		void IncreaseSoundVolume(int) override {}
		void LowerMusicVolume(int) override {}
		void IncreaseMusicVolume(int) override {}
	};

	class ServiceLocator final
	{
		static ISoundSystem* m_SoundSystemInstance;
		static NullSoundSystem m_DefaultSoundSystem;

	public:
		static ISoundSystem& GetSoundSystem() { return *m_SoundSystemInstance; }
		static void RegisterSoundSystem(ISoundSystem* soundSystem)
		{
			m_SoundSystemInstance = soundSystem == nullptr ? m_SoundSystemInstance : soundSystem;
		}
	};
}
