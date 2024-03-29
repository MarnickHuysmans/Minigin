#pragma once
#include "ISoundSystem.h"
#include "mutex"
#include "queue"

namespace dae
{
	class SimpleSoundSystem final : public ISoundSystem
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

		SimpleSoundSystem();
		~SimpleSoundSystem();

		SimpleSoundSystem(const SimpleSoundSystem& other) = delete;
		SimpleSoundSystem(SimpleSoundSystem&& other) noexcept = delete;
		SimpleSoundSystem& operator=(const SimpleSoundSystem& other) = delete;
		SimpleSoundSystem& operator=(SimpleSoundSystem&& other) noexcept = delete;

	private:
		enum class SoundType
		{
			Sound,
			Music
		};
		void Update();
		typedef std::pair<SoundType, std::string> SoundFile;

		std::thread m_Thread;

		int m_SoundVolume;
		int m_MusicVolume;
		std::queue<SoundFile> m_Sounds{};

		std::mutex m_Mutex;
		std::condition_variable m_ConditionVariable;
		std::mutex m_PauseMutex;
		std::condition_variable m_PauseConditionVariable;
		bool m_Pause = false;;
		bool m_Running = true;
	};
}
