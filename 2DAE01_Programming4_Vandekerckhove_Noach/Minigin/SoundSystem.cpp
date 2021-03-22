#include "MiniginPCH.h"
#include "SoundSystem.h"

using namespace dae;
int SDLAudioSystem::m_Tail = 0;
int SDLAudioSystem::m_Head = 0;
dae::Audio SDLAudioSystem::m_Pending[SDLAudioSystem::MAX_PENDING];