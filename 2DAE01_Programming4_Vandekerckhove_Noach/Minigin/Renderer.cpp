#include "MiniginPCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"
#include "LevelManager.h"
#include "ResourceManager.h"
#include "GameSettings.h"

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

	SceneManager::GetInstance().Render();
	//Demo window
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_Window);
	ImGui::NewFrame();

	if(SceneManager::GetInstance().GetCurrentSceneName() == "MainMenu")
	{
		const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
		const int size{ 200 };
		ImGui::SetNextWindowPos(ImVec2(main_viewport->GetCenter().x - (size / 2), main_viewport->GetCenter().y - (size / 2)));
		ImGui::SetNextWindowSize(ImVec2(size, size));
		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoResize;
		windowFlags |= ImGuiWindowFlags_NoCollapse;
		windowFlags |= ImGuiWindowFlags_NoNav;
		windowFlags |= ImGuiWindowFlags_NoBackground;
		if (!ImGui::Begin("buttons",&m_ShowButtons, windowFlags))
			ImGui::End();

		ImGui::Indent(50.0f);
		const ImVec2 buttonSize{ 100,20 };
		if (ImGui::Button("Single player", buttonSize))
		{
			GameSettings::GetInstance().SetGameMode(GameMode::SinglePlayer);
			SceneManager::GetInstance().SetCurrentSceneName("SinglePlayerLevel");
		}
		if (ImGui::Button("Co-op", buttonSize))
		{
			GameSettings::GetInstance().SetGameMode(GameMode::Coop);
			SceneManager::GetInstance().SetCurrentSceneName("CoopLevel");
		}
		if (ImGui::Button("Versus", buttonSize))
		{
			GameSettings::GetInstance().SetGameMode(GameMode::Versus);
			SceneManager::GetInstance().SetCurrentSceneName("SinglePlayerLevel");
		}
		ImGui::Unindent();
		ImGui::End();
	}

	if(m_ShowDemo)
		ImGui::ShowDemoWindow(&m_ShowDemo);
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
	
	const glm::vec2 size{ hex.Radius * sin(60 * M_PI / 180) ,hex.Radius };
	RenderTexture(*LevelManager::GetInstance().GetHexTexture(hex.currentTexID), hex.pos.x - size.x, hex.pos.y - hex.Radius, size.x * 2.0f,size.y * 2.0f);
	
	for (int i = 0; i < nsides; i++) 
	{
		//outer lines
		auto angle_deg = hexagonAngle * i - pointySideAngle; // the -30 is to make the hex point upwards
		auto angle_rad = M_PI / 180 * angle_deg;
		const glm::vec2 point(hex.pos.x + hex.Radius * cos(angle_rad), hex.pos.y + hex.Radius * sin(angle_rad));

		angle_deg = hexagonAngle * (i+1) - pointySideAngle;
		angle_rad = M_PI / 180 * angle_deg;
		const glm::vec2 point2(hex.pos.x + hex.Radius * cos(angle_rad), hex.pos.y + hex.Radius * sin(angle_rad));
		SDL_RenderDrawLine(m_Renderer, int(point.x), int(point.y), int(point2.x), int(point2.y));
		//inner lines
		if (i % 2 != 0)
		{
			SDL_RenderDrawLine(m_Renderer, int(hex.pos.x), int(hex.pos.y), int(point2.x), int(point2.y));
		}
	}
}
