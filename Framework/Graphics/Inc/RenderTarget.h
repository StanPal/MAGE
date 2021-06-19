#pragma once

namespace MAGE::Graphics
{
	class RenderTarget
	{
	public:
		enum class Format
		{
			RGBA_U8,
			RGBA_F16,
			RGBA_U32,
			R_F16,
			R_S32
		};

		RenderTarget() = default;
		~RenderTarget();

		//Shader and buffer should have these lines (Anytime a class should not be copied) 
		RenderTarget(const RenderTarget&) = delete;
		RenderTarget& operator=(const RenderTarget&) = delete;

		void Initialize(uint32_t width, uint32_t height, Format format);
		void Terminate();

		void BeginRender();
		void EndRender();

		void BindPS(uint32_t slot);
		void UnbindPS(uint32_t slot); 

		void* GetShaderResourceView() const { return mShaderResourceView; }

	private:
		//Pointer to use as a texture
		ID3D11ShaderResourceView* mShaderResourceView = nullptr;

		//Pointers to use as a backbuffer depth
		ID3D11RenderTargetView* mRenderTargetView = nullptr;
		ID3D11DepthStencilView* mDepthStencilView = nullptr;
		D3D11_VIEWPORT mViewport;
		D3D11_MAPPED_SUBRESOURCE mSubresource; 
	};
}