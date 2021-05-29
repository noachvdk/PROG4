#include "MiniginPCH.h"
#include "MainMenuUIComponent.h"

#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl.h"
#include "GameSettings.h"
#include "Renderer.h"
#include "SceneManager.h"

using namespace dae;
bool dae::MainMenuUIComponent::m_ShowButtons = false;

MainMenuUIComponent::MainMenuUIComponent()
	: m_Size(200)
	, m_Indent(50)
	, m_ButtonSize(100,20)
{
}

void MainMenuUIComponent::RenderComponent()
{
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(ImVec2(main_viewport->GetCenter().x - (m_Size / 2), main_viewport->GetCenter().y - (m_Size / 2)));
	ImGui::SetNextWindowSize(ImVec2(m_Size, m_Size));
	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoTitleBar;
	windowFlags |= ImGuiWindowFlags_NoScrollbar;
	windowFlags |= ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoCollapse;
	windowFlags |= ImGuiWindowFlags_NoNav;
	windowFlags |= ImGuiWindowFlags_NoBackground;
	if (!ImGui::Begin("buttons", &m_ShowButtons, windowFlags))
		ImGui::End();

	ImGui::Indent(m_Indent);
	if (ImGui::Button("Single player", m_ButtonSize))
	{
		GameSettings::GetInstance().SetGameMode(GameMode::SinglePlayer);
		SceneManager::GetInstance().SetCurrentSceneName("SinglePlayerLevel");
	}
	if (ImGui::Button("Co-op", m_ButtonSize))
	{
		GameSettings::GetInstance().SetGameMode(GameMode::Coop);
		SceneManager::GetInstance().SetCurrentSceneName("CoopLevel");
	}
	if (ImGui::Button("Versus", m_ButtonSize))
	{
		GameSettings::GetInstance().SetGameMode(GameMode::Versus);
		SceneManager::GetInstance().SetCurrentSceneName("SinglePlayerLevel");
	}
	ImGui::Unindent();
	ImGui::End();
}
