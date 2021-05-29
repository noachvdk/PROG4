#include "MiniginPCH.h"
#include "CollisionManager.h"
#include "ColliderComponent.h"
#include "Scene.h"
using namespace dae;

void CollisionManager::AddCollider(ColliderComponent* collider)
{
	m_Colliders.push_back(collider);
}

void CollisionManager::SetVisualize(bool value)
{
	for (auto& collider : m_Colliders)
	{
		collider->SetVisualize(value);	
	}
}

void CollisionManager::Update()
{
	for(auto collider : m_Colliders)
	{
		if(collider->GetParentObject()->GetParentScene()->GetName() == SceneManager::GetInstance().GetCurrentSceneName())
		{
			for (auto otherCollider : m_Colliders)
			{
				if (collider != otherCollider)
					collider->CheckCollision(otherCollider);
			}
		}
	}
}
