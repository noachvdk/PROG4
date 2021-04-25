#pragma once
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		Scene& GetCurrentScene();

		void SetCurrentSceneName(const std::string& scene);
		std::string GetCurrentSceneName() const { return m_CurrentSceneName; }
		
		void Update();
		void Render();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;
		std::string m_CurrentSceneName;
	};
}
