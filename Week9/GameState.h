#pragma once

#include <PCEngine/Inc/PCEngine.h>

class GameState : public PCEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void DrawDepthMap();
	void DrawScene();
	void PostProcess();

private:
	PCEngine::Graphics::Camera mDefaultCamera;
	PCEngine::Graphics::Camera mDebugCamera;
	PCEngine::Graphics::Camera mLightCamera;
	PCEngine::Graphics::Camera* mActiveCamera = nullptr;

	PCEngine::Math::Matrix4 mLightProjectMatrix;
	std::vector<PCEngine::Math::Vector3> mViewFrustumVertices;

	PCEngine::Graphics::Mesh mTankMesh;
	PCEngine::Graphics::MeshBuffer mTankMeshBuffer;

	PCEngine::Graphics::Mesh mGroundMesh;
	PCEngine::Graphics::MeshBuffer mGroundMeshBuffer;

	struct TransformData
	{
		PCEngine::Math::Matrix4 world;
		PCEngine::Math::Matrix4 wvp;
		PCEngine::Math::Vector3 viewPosition;
		float padding;
	};
	struct SettingsData
	{
		float specularMapWeight = 1.0f;
		float bumpMapWeight = 1.0f;
		float normalMapWeight = 1.0f;
		float aoMapWeight = 1.0f;
		float brightness = 1.0f;
		int useShadow = 1;
		float depthBias = 0.0f;
		float padding;
	};
	struct PostProcessSettingsData
	{
		float screenWidth = 0.0f;
		float screenHeight = 0.0f;
		float time = 0.0f;
		float padding;
	};

	using TransformBuffer = PCEngine::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = PCEngine::Graphics::TypedConstantBuffer<PCEngine::Graphics::DirectionalLight>;
	using MaterialBuffer = PCEngine::Graphics::TypedConstantBuffer<PCEngine::Graphics::Material>;
	using SettingsBuffer = PCEngine::Graphics::TypedConstantBuffer<SettingsData>;
	using PostProcessingSettingsBuffer = PCEngine::Graphics::TypedConstantBuffer<PostProcessSettingsData>;
	using DepthMapConstantBuffer = PCEngine::Graphics::TypedConstantBuffer<PCEngine::Math::Matrix4>;
	using ShadowConstantBuffer = PCEngine::Graphics::TypedConstantBuffer<PCEngine::Math::Matrix4>;

	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	SettingsBuffer mSettingsBuffer;
	PostProcessingSettingsBuffer mPostProcessingSettingsBuffer;

	PCEngine::Graphics::DirectionalLight mDirectionalLight;
	PCEngine::Graphics::Material mMaterial;

	PCEngine::Graphics::VertexShader mVertexShader;
	PCEngine::Graphics::PixelShader mPixelShader;

	PCEngine::Graphics::Sampler mSampler;
	PCEngine::Graphics::Texture mDiffuseMap;
	PCEngine::Graphics::Texture mSpecularMap;
	PCEngine::Graphics::Texture mDisplacementMap;
	PCEngine::Graphics::Texture mNormalMap;
	PCEngine::Graphics::Texture mAOMap;

	PCEngine::Graphics::Texture mGroundDiffuseMap;

	std::vector<PCEngine::Math::Vector3> mTankPositions;
	PCEngine::Math::Vector3 mTankRotation = 0.0f;
	float mTankSpacing = 20.0f;

	SettingsData mSettings;

	// Shadow
	PCEngine::Graphics::RenderTarget mDepthMapRenderTarget;
	PCEngine::Graphics::VertexShader mDepthMapVertexShader;
	PCEngine::Graphics::PixelShader mDepthMapPixelShader;
	DepthMapConstantBuffer mDepthMapConstantBuffer;
	ShadowConstantBuffer mShadowConstantBuffer;

	// Post processing
	PCEngine::Graphics::RenderTarget mRenderTarget;
	PCEngine::Graphics::MeshPX mScreenQuad;
	PCEngine::Graphics::MeshBuffer mScreenQuadBuffer;
	PCEngine::Graphics::VertexShader mPostProcessingVertexShader;
	PCEngine::Graphics::PixelShader mPostProcessingPixelShader;
	PostProcessSettingsData mPostProcessSettings;
};