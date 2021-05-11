#pragma once
#include "Singleton.h"

struct Hex;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Rect;

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, SDL_Rect dest, SDL_Rect src, bool flipped) const;
		
		void RenderHex(const Hex& hex) const;

		/*[[nodiscard]]*/SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }
	private:
		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window;
		static bool m_ShowDemo;
		static bool m_ShowButtons;
	};
}

