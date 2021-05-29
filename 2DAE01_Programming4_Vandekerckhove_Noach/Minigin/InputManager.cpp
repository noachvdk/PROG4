#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include <algorithm>    // std::find
#include <vector>  

using namespace dae;

bool InputManager::ProcessInput()
{
	// todo: read the input from gamepad
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i) //Update GamePadStates
	{
		m_OldGamepadState[i] = m_CurrGamepadState[i];
		const DWORD dwResult = XInputGetState(i, &m_CurrGamepadState[i]);
	}
	//Read input from keyboard
	m_Event = SDL_Event(); //Reset 
	SDL_PollEvent(&m_Event);

	if (IsPressedDown(ControllerButton::ButtonStart,0) || (IsKeyPressedDown(SDLK_ESCAPE)))
		return false;

	if ((GetRightTrigger(0) > 0.0f) || (GetLeftTrigger(0) > 0.0f))
		Rumble(0,GetLeftTrigger(0), GetRightTrigger(0));
	else
		Rumble(0);

	return true;
}

bool InputManager::IsPressedDown(ControllerButton button, const int playerID) const
{
	return ((m_CurrGamepadState[playerID].Gamepad.wButtons == (WORD)button) != 0) && ((m_OldGamepadState[playerID].Gamepad.wButtons == (WORD)button) == 0);
}

bool InputManager::IsPressedHeld(ControllerButton button, const int playerID) const
{
	return ((m_CurrGamepadState[playerID].Gamepad.wButtons == (WORD)button) != 0);
}

bool InputManager::IsPressedReleased(ControllerButton button, const int playerID) const
{
	return ((m_CurrGamepadState[playerID].Gamepad.wButtons == (WORD)button) == 0) && ((m_OldGamepadState[playerID].Gamepad.wButtons == (WORD)button) != 0);
}

bool InputManager::IsKeyPressedDown(const SDL_Keycode key) const
{	
	if (m_Event.type == SDL_KEYDOWN && !m_Event.key.repeat)
	{
		return (m_Event.key.keysym.sym == key);
	}
	return false;
}

bool InputManager::IsKeyPressedHeld(const SDL_Keycode key) const
{
	if (m_Event.type == SDL_KEYDOWN)
	{
		return (m_Event.key.keysym.sym == key);
	}
	return false;
}

bool InputManager::IsKeyPressedReleased(const SDL_Keycode key) const
{
	if (m_Event.type == SDL_KEYUP && !m_Event.key.repeat) //Requires work
	{
		return (m_Event.key.keysym.sym == key);
	}
	return false;
}

void InputManager::AddInputAction(InputAction action)
{
	m_InputActions.push_back(action);
}

void InputManager::ResetInputActions()
{
	m_InputActions.resize(0);
	m_InputActions.clear();
}

bool InputManager::GetIsLeftStickInDeadZone(const int playerID) const
{
	const short x = m_CurrGamepadState[playerID].Gamepad.sThumbLX;
	const short y = m_CurrGamepadState[playerID].Gamepad.sThumbLY;

	if ((x > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || x < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(y > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || y < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		return false;

	return true;
}

bool InputManager::GetIsRightStickInDeadZone(const int playerID) const
{
	const short x = m_CurrGamepadState[playerID].Gamepad.sThumbRX;
	const short y = m_CurrGamepadState[playerID].Gamepad.sThumbRY;

	if ((x > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || x < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(y > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || y < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
		return false;

	return true;
}

glm::vec2 InputManager::GetLeftStick(const int playerID) const
{
	const short x = m_CurrGamepadState[playerID].Gamepad.sThumbLX;
	const short y = m_CurrGamepadState[playerID].Gamepad.sThumbLY;

	glm::vec2 res{};
	if (GetIsLeftStickInDeadZone(playerID))
		return res;
	res.x = static_cast<float>(x) / 32768.0f;
	res.y = static_cast<float>(y) / 32768.0f;

	return res;
}

glm::vec2 InputManager::GetRightStick(const int playerID) const
{
	const short x = m_CurrGamepadState[playerID].Gamepad.sThumbRX;
	const short y = m_CurrGamepadState[playerID].Gamepad.sThumbRY;

	glm::vec2 res{};
	if (GetIsRightStickInDeadZone(playerID))
		return res;
	res.x = static_cast<float>(x) / 32768.0f;
	res.y = static_cast<float>(y) / 32768.0f;

	return res;
}

float InputManager::GetLeftTrigger(const int playerID) const
{
	BYTE trigger = m_CurrGamepadState[playerID].Gamepad.bLeftTrigger;

	if (trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return trigger / 255.0f;

	return 0.0f; //not pressed;
}

float InputManager::GetRightTrigger(const int playerID) const
{
	BYTE trigger = m_CurrGamepadState[playerID].Gamepad.bRightTrigger;

	if (trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return trigger / 255.0f;

	return 0.0f; //not pressed;
}

void InputManager::Rumble(const int playerID, float left, float right)
{
	XINPUT_VIBRATION VibrationState;

	ZeroMemory(&VibrationState, sizeof(XINPUT_VIBRATION));

	int leftRumble = int(left * 65535.0f);
	int rightRumble = int(right * 65535.0f);

	VibrationState.wLeftMotorSpeed = WORD(leftRumble);
	VibrationState.wRightMotorSpeed = WORD(rightRumble);

	XInputSetState(playerID, &VibrationState);
}