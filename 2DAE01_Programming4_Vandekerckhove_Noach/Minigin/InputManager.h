#pragma once
#include <Windows.h>
#include <XInput.h>
#include "Singleton.h"
#include <vector>
#include "Command.h"
#include "SDL.h"

namespace dae
{
	enum class ControllerButton : WORD
	{
		ButtonA = XINPUT_GAMEPAD_A,
		ButtonB = XINPUT_GAMEPAD_B,
		ButtonX = XINPUT_GAMEPAD_X,
		ButtonY = XINPUT_GAMEPAD_Y,
		ButtonDown = XINPUT_GAMEPAD_DPAD_DOWN,
		ButtonUp = XINPUT_GAMEPAD_DPAD_UP,
		ButtonLeft = XINPUT_GAMEPAD_DPAD_LEFT,
		ButtonRight = XINPUT_GAMEPAD_DPAD_RIGHT,
		ButtonLeftShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER,
		ButtonRightShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		ButtonLeftThumb = XINPUT_GAMEPAD_LEFT_THUMB,
		ButtonRightThumb = XINPUT_GAMEPAD_RIGHT_THUMB,
		ButtonStart = XINPUT_GAMEPAD_START,
		ButtonBack = XINPUT_GAMEPAD_BACK,
		NoAction
	};

	enum class TriggerType
	{
		OnPress,
		OnRelease,
		OnHold
	};

	class InputAction final
	{
	public:
		const unsigned int ID;
		const SDL_Keycode keyboardCode; 
		const TriggerType type;
		ControllerButton button;
		Command* command;

		InputAction()
			: ID{}
			, keyboardCode{}
			, type{}
			, button{}
			, command{ nullptr }
		{}

		InputAction(const unsigned int ActionID, const SDL_Keycode keyCode ,const TriggerType triggerType, ControllerButton actionbutton, Command* actionCommand)
			: ID{ ActionID }
			, keyboardCode{ keyCode }
			, type{ triggerType }
			, button{ actionbutton }
			, command{ actionCommand }
		{}
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		//Buttons
		bool IsPressedDown(ControllerButton button, const int playerID) const;
		bool IsPressedHeld(ControllerButton button, const int playerID) const;
		bool IsPressedReleased(ControllerButton button, const int playerID) const;
		//Sticks
		bool GetIsLeftStickInDeadZone(const int playerID) const;
		bool GetIsRightStickInDeadZone(const int playerID) const;
		glm::vec2 GetLeftStick(const int playerID) const;
		glm::vec2 GetRightStick(const int playerID)const;
		//Triggers
		float GetLeftTrigger(const int playerID) const;
		float GetRightTrigger(const int playerID) const;
		//Rumble
		void Rumble(const int playerID, float leftRumble = 0.f, float rightRumble = 0.0f);

		//Actions
		void AddInputAction(InputAction action);
		std::vector<InputAction> GetInputActions() const { return m_InputActions; }
		//Keys
		bool IsKeyPressedDown(const SDL_Keycode key) const;
		bool IsKeyPressedHeld(const SDL_Keycode key) const;
		bool IsKeyPressedReleased(const SDL_Keycode key) const;
		
		InputManager();
		~InputManager();
	private:
		XINPUT_STATE m_CurrGamepadState[XUSER_MAX_COUNT];
		XINPUT_STATE m_OldGamepadState[XUSER_MAX_COUNT];

		SDL_Event m_Event;

		std::vector<InputAction> m_InputActions{};
	};

}
