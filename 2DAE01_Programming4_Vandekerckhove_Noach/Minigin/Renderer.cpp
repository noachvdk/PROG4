#include "MiniginPCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"
#include "LevelManager.h"
#include "Hex.h"

#include "imgui.h"
#include "../imgui-1.81/backends/imgui_impl_sdl.h"
#include "../imgui-1.81/backends/imgui_impl_opengl2.h"
//Imgui_widgets.cpp altered line 6190
//added a pragma warning disable to prevent Unreferenced parameter error

bool dae::Renderer::m_ShowDemo = false;
bool dae::Renderer::m_ShowButtons = false;

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window * window)
{
	m_Window = window;
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();


	
	//m_ShowDemo = true;
}

void dae::Renderer::Render() const
{
	SDL_RenderClear(m_Renderer);

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_Window);
	ImGui::NewFrame();
	
	SceneManager::GetInstance().Render();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(m_Renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	/*safeDelete(m_ShowDemo);*/

	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, SDL_Rect dest, SDL_Rect src, bool flipped) const
{
	if (flipped)
		SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dest, 0.0f, nullptr, SDL_FLIP_NONE);
	else
		SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dest, 0.0f, nullptr, SDL_FLIP_HORIZONTAL);
}

//some logic & code is sampled from https://www.redblobgames.com/grids/hexagons/implementation.html

void dae::Renderer::RenderHex(const Hex& hex) const
{
	const int nsides = 6;
	const int pointySideAngle{ 30 };
	const int hexagonAngle{ 360 / nsides };

	const int radius{ hex.GetRadius() };
	const glm::vec2 pos{ hex.GetPos() };
	
	const glm::vec2 size{ radius * sin(60 * M_PI / 180) ,radius };
	RenderTexture(*LevelManager::GetInstance().GetHexTexture(hex.GetCurrentTexID()), pos.x - size.x, pos.y - radius, size.x * 2.0f,size.y * 2.0f);
	
	for (int i = 0; i < nsides; i++) 
	{
		//outer lines
		auto angle_deg = hexagonAngle * i - pointySideAngle; // the -30 is to make the hex point upwards
		auto angle_rad = M_PI / 180 * angle_deg;
		const glm::vec2 point(pos.x + radius * cos(angle_rad), pos.y + radius * sin(angle_rad));

		angle_deg = hexagonAngle * (i+1) - pointySideAngle;
		angle_rad = M_PI / 180 * angle_deg;
		const glm::vec2 point2(pos.x + radius * cos(angle_rad), pos.y + radius * sin(angle_rad));
		SDL_RenderDrawLine(m_Renderer, int(point.x), int(point.y), int(point2.x), int(point2.y));
		//inner lines
		if (i % 2 != 0)
		{
			SDL_RenderDrawLine(m_Renderer, int(pos.x), int(pos.y), int(point2.x), int(point2.y));
		}
	}
}

void dae::Renderer::RenderSquare(const SDL_Rect& square) const
{
	SDL_SetRenderDrawColor(GetSDLRenderer(), 255, 0, 0, 255);
	SDL_RenderDrawRect(m_Renderer, &square);
	SDL_SetRenderDrawColor(GetSDLRenderer(), 0, 0, 0, 255);
}
