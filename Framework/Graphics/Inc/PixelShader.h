#pragma once
namespace MAGE::Graphics
{
	class PixelShader
	{
	public:
		void Initialize(const std::filesystem::path& filePath, const char* shadername = "PS");
		void Terminate();
		void Bind();
	private:
		ID3D11PixelShader* mPixelShader = nullptr;
	};
} //namespace MAGE::Graphics