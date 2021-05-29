#pragma once
#include "BaseComponent.h"

namespace dae
{
	enum class direction;
	class PlayerControlledComponent : public BaseComponent , public Observer
	{
	public:
		virtual void move(direction) =0;
	protected:
		//Functions
		PlayerControlledComponent() = default;

		PlayerControlledComponent(const PlayerControlledComponent& other) = delete;
		PlayerControlledComponent(PlayerControlledComponent&& other) noexcept = delete;
		PlayerControlledComponent& operator=(const PlayerControlledComponent& other) = delete;
		PlayerControlledComponent& operator=(PlayerControlledComponent&& other) noexcept = delete;
	};
}


