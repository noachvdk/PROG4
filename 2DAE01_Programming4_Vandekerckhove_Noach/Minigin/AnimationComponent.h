#pragma once
#include <SDL.h>
#include "BaseComponent.h"


namespace dae
{
	enum class AnimState : int
	{
		Idle = 0,
		Jumping = 1,
		Dying = 2,
		FacingAway = 3,
		FacingForward = 4,
		None = 5,
		Invisible = 6
	};
	
	class Texture2D;
	class AnimationComponent final : public BaseComponent
	{
	public:
		AnimationComponent(const std::string& filename,unsigned int rows, unsigned int columns,float animTime,AnimState state, bool move = false);
		~AnimationComponent() override = default;
		
		AnimationComponent(const AnimationComponent& other) = delete;
		AnimationComponent(AnimationComponent&& other) noexcept = delete;
		AnimationComponent& operator=(const AnimationComponent& other) = delete;
		AnimationComponent& operator=(AnimationComponent&& other) noexcept = delete;

		void UpdateComponent() override;
		void RenderComponent() override;
		void PostAddedToGameObject() override {};

		AnimState GetAnimState() const { return m_State; }
		int GetTextureFrameHeight()const { return m_FrameHeight; }
		int GetTextureFrameWidth()const { return m_FrameWidth; }
		SDL_Rect GetDestRect() const { return m_DestRect; }

		void SetFlipped(bool value) { m_IsFlipped = value; }
		void SetOffset(float x, float y);
	private:
		bool m_MoveIndependently;
		bool m_IsFlipped;
		//
		int m_FrameWidth;
		int m_FrameHeight;
		int m_Width;
		int m_Height;
		//
		int m_CurrentRow;
		int m_CurrentColumn;
		int m_MaxRow;
		int m_MaxColumn;
		//anim time
		float m_OffsetX, m_OffsetY;
		float m_AnimTimer;
		float m_MaxAnimTime;
		//rendering
		AnimState m_State;
		SDL_Rect m_SrcRect;
		SDL_Rect m_DestRect;
		std::shared_ptr<Texture2D> m_pTexture;
	};
}

