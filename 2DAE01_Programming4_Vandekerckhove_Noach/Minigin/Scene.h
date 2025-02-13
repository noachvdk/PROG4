#pragma once
#include "InputManager.h"
#include "SceneManager.h"

namespace dae
{
	class SceneObject;
	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
		friend Scene& SceneManager::GetCurrentScene();
	public:
		void Add(const std::shared_ptr<SceneObject>& object);
		void AddInputAction(const InputAction& action);
		const std::string GetName() const { return m_Name; }

		std::vector<InputAction> GetSceneInputActions() const { return m_InputActions; }

		void Update();
		void Render() const;

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector < std::shared_ptr<SceneObject>> m_Objects{};
		std::vector<InputAction> m_InputActions{};
		static unsigned int m_IdCounter; 
	};

}
