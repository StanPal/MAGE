#pragma once
#include <MAGEEngine/Inc/MAGE.h>

namespace MAGE
{
	class Skydome
	{
	public:
		Skydome() = default;
		~Skydome() = default;

		void Initialize(const std::filesystem::path& texturePath);
		void SetSize(float radius, int rings, int slices);
		void Render(const MAGE::Graphics::Camera& camera);

		void Update(const MAGE::Graphics::Camera& camera);

		void Terminate();

	private: 
		MAGE::Graphics::MeshBuffer mMeshBufferSkyDome;
		 
		MAGE::Graphics::Texture mSkyDomeTexture;
		MAGE::Graphics::Sampler mSamplers;
		 
		MAGE::Graphics::ConstantBuffer mConstantBufferSkyDome;
		 
		MAGE::Graphics::VertexShader mVSSkyDome;
		MAGE::Graphics::PixelShader mPSSkyDome;
		 
		MAGE::Math::Vector3 mSkyDomePos = MAGE::Math::Vector3::Zero;
	};
}