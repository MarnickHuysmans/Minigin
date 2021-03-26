#include "MiniginPCH.h"
#include "ServiceLocator.h"

dae::NullSoundSystem dae::ServiceLocator::m_DefaultSoundSystem{};
dae::ISoundSystem* dae::ServiceLocator::m_SoundSystemInstance = &m_DefaultSoundSystem;