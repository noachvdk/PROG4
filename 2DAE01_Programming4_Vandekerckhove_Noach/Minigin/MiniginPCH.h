#pragma once

#include <stdio.h>
#include <iostream> // std::cout
#include <sstream> // stringstream
#include <memory> // smart pointers
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "imgui.h"
#include "../imgui-1.81/backends/imgui_impl_sdl.h"
#include "../imgui-1.81/backends/imgui_impl_opengl2.h"
//Imgui_widgets.cpp altered line 6190
//added a pragma warning disable to prevent Unreferenced parameter error

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

# define M_PI 3.14159265358979323846f 

template <class T>
void safeDelete(T& pointer)
{
	if (pointer != nullptr)
	{
		delete pointer;
		pointer = nullptr;
	}
}