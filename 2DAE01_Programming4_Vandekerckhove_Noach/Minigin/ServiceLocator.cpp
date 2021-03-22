#include "MiniginPCH.h"
#include "ServiceLocator.h"

using namespace dae;
NullAudioSystem ServiceLocator::m_DefaultSoundSystem;
SoundSystem* ServiceLocator::m_SoundSystem = &m_DefaultSoundSystem;
bool ServiceLocator::m_SoundSystemRegistered = false;