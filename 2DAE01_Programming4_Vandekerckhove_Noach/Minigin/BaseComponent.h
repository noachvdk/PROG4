#pragma once
#include "GameObject.h"

namespace dae
{
	class BaseComponent
	{
	public : 

		virtual ~BaseComponent() = default;

		virtual void UpdateComponent() = 0;
		virtual void RenderComponent() = 0;
		virtual void PostAddedToGameObject() = 0;

		GameObject* GetParentObject() const { return m_pParentObj; }
		virtual void SetParentObject(GameObject* parent);
	protected:
		//Functions
		BaseComponent();
		
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) noexcept = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) noexcept = delete;
		
		//variables
		GameObject* m_pParentObj;
	};

}

