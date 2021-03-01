#include "MiniginPCH.h"
#include "PlayerComponent.h"
#include "InputManager.h"

using namespace dae;

PlayerComponent::PlayerComponent(const unsigned int playerID)
	:m_PlayerID(playerID)
{

}

PlayerComponent::~PlayerComponent()
{

}

void PlayerComponent::UpdateComponent()
{
	for (auto action : InputManager::GetInstance().GetInputActions())
	{
		if (action.ID == m_PlayerID)
		{
			if (action.type == TriggerType::OnPress)
			{
				if (InputManager::GetInstance().IsPressedDown(action.button, action.ID))
					action.command->Execute(*GetParentObject());
				if (InputManager::GetInstance().IsKeyPressedDown(action.keyboardCode))
					action.command->Execute(*GetParentObject());
			}
			else if (action.type == TriggerType::OnRelease)
			{
				if (InputManager::GetInstance().IsPressedReleased(action.button, action.ID))
					action.command->Execute(*GetParentObject());
				if (InputManager::GetInstance().IsKeyPressedReleased(action.keyboardCode))
					action.command->Execute(*GetParentObject());
			}
			else
			{
				if (InputManager::GetInstance().IsPressedHeld(action.button, action.ID))
					action.command->Execute(*GetParentObject());
				if (InputManager::GetInstance().IsKeyPressedHeld(action.keyboardCode))
					action.command->Execute(*GetParentObject());
			}
		}
	}
}