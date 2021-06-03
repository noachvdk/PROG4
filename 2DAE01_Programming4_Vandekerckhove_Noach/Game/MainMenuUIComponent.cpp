#include "MiniginPCH.h"
#include "MainMenuUIComponent.h"

#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl.h"
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

	for (const auto& option :m_SceneOptions)
	{
		if (ImGui::Button(option.first.c_str(), m_ButtonSize))
		{
			GameSettings::GetInstance().SetGameMode(option.second);
			SceneManager::GetInstance().SetCurrentSceneName(option.first.c_str());
		}
	}
	ImGui::Unindent();
	ImGui::End();
}
