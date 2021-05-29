#include "MiniginPCH.h"
#include "SceneManager.h"

#include "GameSettings.h"
#include "Scene.h"
#include "InputManager.h"

void dae::SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		if(scene->GetName() == m_CurrentSceneName)
		scene->Update();
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_Scenes)
	{
		if (scene->GetName() == m_CurrentSceneName)
		scene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	m_CurrentSceneName = name;
	return *scene;
}

dae::Scene& dae::SceneManager::GetCurrentScene()
{
	for (const auto& scene : m_Scenes)
	{
		if (scene->GetName() == m_CurrentSceneName)
			return *scene;
	}
	const auto scene = std::shared_ptr<Scene>(new Scene("Empty"));
	return *scene;
}

void dae::SceneManager::SetCurrentSceneName(const std::string& scene)
{
	m_CurrentSceneName = scene;
	InputManager::GetInstance().ResetInputActions(); //Reset inputactions
	InputManager::GetInstance().Reset(); //Reset input so the input to change level doesn't get registered in the next scene
	LevelManager::GetInstance().SetCurrentLevelID(1);
	//add the inputactions relevant to the scene
	auto actions = GetCurrentScene().GetSceneInputActions();
	for (auto temp : actions)
	{
		InputManager::GetInstance().AddInputAction(temp);
	}
}