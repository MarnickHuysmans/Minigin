#include "MiniginPCH.h"
#include "SimpleSoundSystem.h"

#include <SDL.h>

#include "audio.h"

void SimpleSoundSystem::PlaySound(const std::string& soundFile)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_Sounds.push(SoundFile(SoundType::Sound, soundFile));
	m_ConditionVariable.notify_one();
}

void SimpleSoundSystem::PlayMusic(const std::string& musicFile)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_Sounds.push(SoundFile(SoundType::Music, musicFile));
	m_ConditionVariable.notify_one();
}

void SimpleSoundSystem::PauseAudio()
{
	m_Pause = true;
}

void SimpleSoundSystem::UnpauseAudio()
{
	m_Pause = false;
	m_PauseConditionVariable.notify_one();
}

void SimpleSoundSystem::LowerSoundVolume(int amount)
{
	int volume = m_SoundVolume;
	m_SoundVolume = volume > amount ? volume - amount : 0;
}

void SimpleSoundSystem::IncreaseSoundVolume(int amount)
{
	int volume = m_SoundVolume;
	m_SoundVolume = volume + amount > SDL_MIX_MAXVOLUME ? SDL_MIX_MAXVOLUME : volume + amount;
}

void SimpleSoundSystem::LowerMusicVolume(int amount)
{
	int volume = m_MusicVolume;
	m_MusicVolume = volume > amount ? volume - amount : 0;
}

void SimpleSoundSystem::IncreaseMusicVolume(int amount)
{
	int volume = m_MusicVolume;
	m_MusicVolume = volume + amount > SDL_MIX_MAXVOLUME ? SDL_MIX_MAXVOLUME : volume + amount;
}

SimpleSoundSystem::SimpleSoundSystem()
	: m_SoundVolume(SDL_MIX_MAXVOLUME)
	, m_MusicVolume(SDL_MIX_MAXVOLUME)
{
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	initAudio();
	std::thread thread(&SimpleSoundSystem::Update, this);
	if (thread.joinable())
	{
		thread.detach();
	}
}

SimpleSoundSystem::~SimpleSoundSystem()
{
	endAudio();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void SimpleSoundSystem::Update()
{
	std::unique_lock<std::mutex> uniqueLock(m_Mutex);
	std::unique_lock<std::mutex> pauseUniqueLock(m_PauseMutex);
	while (true)
	{
		if (m_Pause)
		{
			m_PauseConditionVariable.wait(pauseUniqueLock, [&] {return !m_Pause; });
		}
		uniqueLock.lock();
		if (m_Sounds.size() == 0)
		{
			m_ConditionVariable.wait(uniqueLock, [&] {return m_Sounds.size() > 0; });
		}
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
}
