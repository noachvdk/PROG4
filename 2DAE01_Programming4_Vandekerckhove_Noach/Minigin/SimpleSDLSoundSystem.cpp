#include "MiniginPCH.h"
#include "SimpleSDLSoundSystem.h"

using namespace dae;
int SDLAudioSystem::m_Tail = 0;
int SDLAudioSystem::m_Head = 0;
dae::Audio SDLAudioSystem::m_Pending[MAX_PENDING];