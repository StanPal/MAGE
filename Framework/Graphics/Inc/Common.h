#pragma once

// Engine headers
#include <Core/Inc/Core.h>
#include <Math/Inc/EngineMath.h>

// DirectX headers
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

// External headers
#include <ImGui/Inc/imgui.h>

// DirectX libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

template <class T>
//Take any Directx pointer by pointer reference aka lending your key to the function 
void SafeRelease(T*& ptr)
{
	if (ptr)
	{
		ptr->Release();
		//Setting the original to null 
		ptr = nullptr;
	}
}