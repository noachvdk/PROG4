#pragma once
#include "Minigin.h"

namespace dae
{
	class GameObject;
	class QbertGame final : public Minigin
	{
	public:
		QbertGame() : Minigin() {}
		void LoadGame() const override;
	};
}


