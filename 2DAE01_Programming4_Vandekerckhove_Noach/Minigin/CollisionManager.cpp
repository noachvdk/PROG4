#include "MiniginPCH.h"
#include "CollisionManager.h"
#include "ColliderComponent.h"
#include "Scene.h"
using namespace dae;

void CollisionManager::AddCollider(ColliderComponent* collider)
{
	m_pColliders.push_back(collider);
}

void CollisionManager::SetVisualize(bool value)
{
	for (auto& collider : m_pColliders)
	{
		collider->SetVisualize(value);	
	}
}

void CollisionManager::Update()
{
	for(auto collider : m_pColliders)
	{
		//Don't update colliders that aren't in the active scene
		if(collider->GetParentObject()->GetParentScene()->GetName() == SceneManager::GetInstance().GetCurrentSceneName())
		{
			for (auto otherCollider : m_pColliders)
			{
				if (collider != otherCollider)
					collider->CheckCollision(otherCollider);
			}
		}
	}
}
