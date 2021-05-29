#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include "Subject.h"

namespace dae
{
	class Transform;
	class BaseComponent;
	class GameObject final : public SceneObject
	{
	public:
		void Update() override;
		void Render() const override;

		void SetPosition(float x, float y);

		void AddComponent(BaseComponent* component);

		template<class T>
		T* GetComponent()
		{
			for (auto component : m_pComponents)
			{
				if (component != nullptr && typeid(*component) == typeid(T))
					return static_cast<T*>(component);
			}
			return nullptr;
		}

		//For components that inherit
		template<class T>
		T* GetComponentDynamic()
		{
			for (auto component : m_pComponents)
			{
				if (component != nullptr)
				{
					if(dynamic_cast<T*>(component))
						return static_cast<T*>(component);
					
				}
			}
			return nullptr;
		}
		
		template<class T>
		std::vector<T*> GetComponents()
		{
			std::vector<T*> components{};
			for (auto component : m_pComponents)
			{
				if (component != nullptr && typeid(*component) == typeid(T))
					components.push_back(static_cast<T*>(component));
			}
			return components;
		}
		Transform GetTransform() const { return m_Transform; }
		std::vector<BaseComponent*> GetAllComponents() const { return m_pComponents; }

		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_Transform;
		std::vector<BaseComponent*> m_pComponents;
	};
}
