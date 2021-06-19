#pragma once
#include "Colors.h"
namespace MAGE::Graphics {

class GraphicsSystem
{
public:
	static void StaticInitialize(HWND window, bool fullscreen);
	static void StaticTerminate();
	static GraphicsSystem* Get();

public:
	GraphicsSystem() = default;
	~GraphicsSystem();

	GraphicsSystem(const GraphicsSystem&) = delete;
	GraphicsSystem& operator=(const GraphicsSystem&) = delete;

	void Initialize(HWND window, bool fullscreen);
	void Terminate();

	void BeginRender();
	void EndRender();

	void ToggleFullscreen();
	void Resize(uint32_t width, uint32_t height);

	void ResetRenderTarget();
	void ResetViewport();

	void SetClearColor(Color clearColor) { mClearColor = clearColor; }
	void SetVSync(bool vSync) { mVSync = vSync ? 1 : 0; }

	uint32_t GetBackBufferWidth() const;
	uint32_t GetBackBufferHeight() const;

	// TODO -- Remove these eventually
	ID3D11Device* GetDevice() const { return mD3DDevice; }
	ID3D11DeviceContext* GetContext() const { return mImmediateContext; }

private:
	friend LRESULT CALLBACK GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	friend ID3D11Device* GetDevice();
	friend ID3D11DeviceContext* GetContext();

	//Memory management pointer
	ID3D11Device* mD3DDevice = nullptr;
	//Everything we call we directly send to the graphics card 
	ID3D11DeviceContext* mImmediateContext = nullptr;

	//DXI = DirectX Graphics Infrastructure (Cross version stuff that never change) 
	IDXGISwapChain* mSwapChain = nullptr;
	//This interface allows us to use the back buffer. 
	ID3D11RenderTargetView* mRenderTargetView = nullptr;

	//Store temporary information, aka the depth every time we spray pixel on the back buffer 
	//we need to know the z-axis the depth of the pixel. 
	ID3D11Texture2D* mDepthStencilBuffer = nullptr;
	//Depth 24bit, Stencil 8bit 
	ID3D11DepthStencilView* mDepthStencilView = nullptr;

	//Swap chain description, it is a struct of all the information we want to have
	DXGI_SWAP_CHAIN_DESC mSwapChainDesc{};
	D3D11_VIEWPORT mViewport{};

	Color mClearColor = Colors::White;
	UINT mVSync = 1;
};

} // namespace MAGE::Graphics