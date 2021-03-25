#include "MiniginPCH.h"
#include "SimpleSoundSystem.h"

#include <SDL.h>

#include "audio.h"

void dae::SimpleSoundSystem::PlaySound(const std::string& soundFile)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_Sounds.push(SoundFile(SoundType::Sound, soundFile));
	m_ConditionVariable.notify_one();
}

void dae::SimpleSoundSystem::PlayMusic(const std::string& musicFile)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_Sounds.push(SoundFile(SoundType::Music, musicFile));
	m_ConditionVariable.notify_one();
}

void dae::SimpleSoundSystem::PauseAudio()
{
	m_Pause = true;
}

void dae::SimpleSoundSystem::UnpauseAudio()
{
	m_Pause = false;
	m_PauseConditionVariable.notify_one();
}

void dae::SimpleSoundSystem::LowerSoundVolume(int amount)
{
	int volume = m_SoundVolume;
	m_SoundVolume = volume > amount ? volume - amount : 0;
}

void dae::SimpleSoundSystem::IncreaseSoundVolume(int amount)
{
	int volume = m_SoundVolume;
	m_SoundVolume = volume + amount > SDL_MIX_MAXVOLUME ? SDL_MIX_MAXVOLUME : volume + amount;
}

void dae::SimpleSoundSystem::LowerMusicVolume(int amount)
{
	int volume = m_MusicVolume;
	m_MusicVolume = volume > amount ? volume - amount : 0;
}

void dae::SimpleSoundSystem::IncreaseMusicVolume(int amount)
{
	int volume = m_MusicVolume;
	m_MusicVolume = volume + amount > SDL_MIX_MAXVOLUME ? SDL_MIX_MAXVOLUME : volume + amount;
}

dae::SimpleSoundSystem::SimpleSoundSystem()
	: ISoundSystem()
	, m_SoundVolume(SDL_MIX_MAXVOLUME)
	, m_MusicVolume(SDL_MIX_MAXVOLUME)
{
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	initAudio();
	m_Thread = std::thread(&SimpleSoundSystem::Update, this);
}

dae::SimpleSoundSystem::~SimpleSoundSystem()
{
	m_Running = false;
	UnpauseAudio();
	m_ConditionVariable.notify_one();
	m_Thread.join();

	endAudio();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void dae::SimpleSoundSystem::Update()
{
	std::unique_lock<std::mutex> uniqueLock(m_Mutex);
	std::unique_lock<std::mutex> pauseUniqueLock(m_PauseMutex);
	while (m_Running)
	{
		if (m_Pause)
		{
			m_PauseConditionVariable.wait(pauseUniqueLock);
		}
		uniqueLock.lock();
		if (!m_Sounds.empty())
		{
			SoundFile soundFile = m_Sounds.front();
			m_Sounds.pop();
			uniqueLock.unlock();
			try
			{
				switch (soundFile.first)
				{
				case SoundType::Sound:
					playSound(soundFile.second.c_str(), m_SoundVolume);
					break;
				case SoundType::Music:
					playMusic(soundFile.second.c_str(), m_MusicVolume);
					break;
				}
			}
			catch (...)
			{
			}
		}
		else
		{
			m_ConditionVariable.wait(uniqueLock);
		}
	}
}
