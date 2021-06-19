#pragma once
#include <MAGEEngine/Inc/MAGE.h>

using namespace MAGE;
using namespace MAGE::Graphics;
using namespace MAGE::Math;

class GameState : public MAGE::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private: 
	MAGE::Graphics::Camera mCamera;
	 
	Mesh mMesh;
	MeshBuffer mMeshBuffer;

	struct TransformData
	{
		MAGE::Math::Matrix4 world;
		MAGE::Math::Matrix4 wvp;
		MAGE::Math::Vector3 viewPosition;
		float padding; 
	};

	struct SettingsData
	{
		float SpecularWeight = 1.0f; 
		float BumpMapWeight = 1.0f; 
		float normalMapWeight = 1.0f; 
		float padding;
	};

	using TransformBuffer = MAGE::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = MAGE::Graphics::TypedConstantBuffer<DirectionalLight>;
	using MaterialBuffer = MAGE::Graphics::TypedConstantBuffer<Material>;
	using SettingsBuffer = MAGE::Graphics::TypedConstantBuffer<SettingsData>;

	SettingsData mSettings;
	Sampler mSampler; 
	Texture mTexture;
	Texture mDiffuseMap;
	Texture mSpecularMap; 
	Texture mDisplacementMap;
	Texture mNormalMap;
	Texture mCloudMap;
	Texture mNightLightMap;
	BlendState mBlendState; 
	
	SettingsBuffer mSettingsBuffer;
	TransformBuffer mTransformBuffer; 
	LightBuffer mLightBuffer; 
	MaterialBuffer mMaterialBuffer;

	DirectionalLight mDireciontionalLight; 
	Material mMaterial; 

	VertexShader mEarthVertexShader;
	PixelShader mEarthPixelShader;

	VertexShader mCloudVertexShader;
	PixelShader mCloudPixelShader;

	VertexShader mNightLightVertexShader;
	PixelShader mNightLightPixelShader;
	
	Vector3 mRotation = 0.0f;	
	float mCloudRotation = 0.0f;
};

