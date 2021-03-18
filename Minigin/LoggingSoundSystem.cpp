#include "MiniginPCH.h"
#include "LoggingSoundSystem.h"

void LoggingSoundSystem::PlaySound(const std::string& soundFile)
{
	m_RealSoundSystem->PlaySound(soundFile);
	std::cout << "Playing " << soundFile << std::endl;
}

void LoggingSoundSystem::PlayMusic(const std::string& musicFile)
{
	m_RealSoundSystem->PlayMusic(musicFile);
	std::cout << "Playing " << musicFile << std::endl;
}

void LoggingSoundSystem::PauseAudio()
{
	m_RealSoundSystem->PauseAudio();
	std::cout << "Playback paused" << std::endl;
}

void LoggingSoundSystem::UnpauseAudio()
{
	m_RealSoundSystem->UnpauseAudio();
	std::cout << "Playback unpaused" << std::endl;
}

void LoggingSoundSystem::LowerSoundVolume(int amount)
{
	m_RealSoundSystem->LowerSoundVolume(amount);
	std::cout << "Lowering sound volume by " << amount << std::endl;
}

void LoggingSoundSystem::IncreaseSoundVolume(int amount)
{
	m_RealSoundSystem->IncreaseSoundVolume(amount);
	std::cout << "Increasing sound volume by " << amount << std::endl;
}

void LoggingSoundSystem::LowerMusicVolume(int amount)
{
	m_RealSoundSystem->LowerMusicVolume(amount);
	std::cout << "Lowering music volume by " << amount << std::endl;
}

void LoggingSoundSystem::IncreaseMusicVolume(int amount)
{
	m_RealSoundSystem->IncreaseMusicVolume(amount);
	std::cout << "Increasing music volume by " << amount << std::endl;
}

LoggingSoundSystem::LoggingSoundSystem(ISoundSystem* realSoundSystem)
	: m_RealSoundSystem(realSoundSystem)
{
}

LoggingSoundSystem::~LoggingSoundSystem()
{
	delete m_RealSoundSystem;
}
