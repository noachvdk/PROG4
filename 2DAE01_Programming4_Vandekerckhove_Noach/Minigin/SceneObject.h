#pragma once
namespace dae
{
	class Scene;
	class SceneObject
	{
	public:
		virtual void Update() = 0;
		virtual void Render() const = 0;

		SceneObject() = default;
		virtual ~SceneObject() = default;
		SceneObject(const SceneObject& other) = delete;
		SceneObject(SceneObject&& other) = delete;
		SceneObject& operator=(const SceneObject& other) = delete;
		SceneObject& operator=(SceneObject&& other) = delete;


		void SetParentScene(Scene* scene) { m_pParentScene = scene; }
		Scene* GetParentScene() { return m_pParentScene; }
	protected:
		Scene* m_pParentScene;
	};
}
