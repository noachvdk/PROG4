#pragma once
#include "BaseComponent.h"
#include "imgui.h"
#include <unordered_map>
#include "GameSettings.h"
namespace dae
{
	class MainMenuUIComponent final : public BaseComponent
	{
	public:
		MainMenuUIComponent();
		~MainMenuUIComponent() = default;
		MainMenuUIComponent(const MainMenuUIComponent& other) = delete;
		MainMenuUIComponent(MainMenuUIComponent&& other) noexcept = delete;
		MainMenuUIComponent& operator=(const MainMenuUIComponent& other) = delete;
		MainMenuUIComponent& operator=(MainMenuUIComponent&& other) noexcept = delete;

		void UpdateComponent() override {};
		void RenderComponent() override;
		void PostAddedToGameObject() override {};

		void AddOption(const std::string& option, GameMode mode) { m_SceneOptions.emplace( option, mode ); }
		void SetButtonSize(const ImVec2& size) { m_ButtonSize = size; }
	private:
		static bool m_ShowButtons;
		const float m_Size{ };
		const float m_Indent{};
		ImVec2 m_ButtonSize{};
		std::unordered_map<std::string, GameMode>m_SceneOptions;
	};
	
}

