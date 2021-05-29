#pragma once
#include "BaseComponent.h"
#include "imgui.h"
namespace dae
{
	class MainMenuUIComponent final : public BaseComponent
	{
	public:
		MainMenuUIComponent();

		MainMenuUIComponent(const MainMenuUIComponent& other) = delete;
		MainMenuUIComponent(MainMenuUIComponent&& other) noexcept = delete;
		MainMenuUIComponent& operator=(const MainMenuUIComponent& other) = delete;
		MainMenuUIComponent& operator=(MainMenuUIComponent&& other) noexcept = delete;

		void UpdateComponent() override {};
		void RenderComponent() override;
		void PostAddedToGameObject() override {};

	private:
		static bool m_ShowButtons;
		const float m_Size{ };
		const float m_Indent{};
		const ImVec2 m_ButtonSize{};
	};
	
}

