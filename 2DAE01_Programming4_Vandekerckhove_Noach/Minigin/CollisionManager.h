#pragma once
#include "Singleton.h"
namespace dae
{
	class ColliderComponent;
	class CollisionManager final : public Singleton<CollisionManager>
	{
	public:
		friend class Singleton<CollisionManager>;

		~CollisionManager() = default;

		CollisionManager(const CollisionManager& other) = delete;
		CollisionManager(CollisionManager&& other) noexcept = delete;
		CollisionManager& operator=(const CollisionManager& other) = delete;
		CollisionManager& operator=(CollisionManager&& other) noexcept = delete;

		void AddCollider(ColliderComponent* collider);
		void SetVisualize(bool value);

		void Update();
	private:
		CollisionManager() = default;
		std::vector<ColliderComponent*> m_pColliders;
	};
}


