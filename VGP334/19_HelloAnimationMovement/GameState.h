#pragma once

#include <MAGEEngine\Inc\MAGE.h>

class GameState : public MAGE::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;
	void Move(float deltaTime);

private:
	void DrawScene();
	void PostProcess();
	void DrawDepthMap();
	//void OpenFile();
private:
	MAGE::Graphics::Camera mDefaultCamera;
	MAGE::Graphics::Camera mLightCamera;
	MAGE::Graphics::Camera* mActiveCamera = nullptr;

	MAGE::Graphics::Model mModel;
	
	

	MAGE::Graphics::Mesh mWarrokMesh;
	MAGE::Graphics::MeshBuffer mWarrokMeshBuffer;

	MAGE::Graphics::Mesh mGroundMesh;
	MAGE::Graphics::MeshBuffer mGroundMeshBuffer;
	
	MAGE::Math::Vector3 mModelPos = MAGE::Math::Vector3::Zero;
	MAGE::Math::Vector3 mModelDir = MAGE::Math::Vector3::One;

	MAGE::Skydome mSkydome; 	
	MAGE::GameWorld mWorld;

	struct TransformData
	{
		MAGE::Math::Matrix4 world;
		MAGE::Math::Matrix4 wvp;
		MAGE::Math::Vector3 viewPosition;
		float padding;
	};
	struct SettingsData
	{
		//Data from HLSLmust be 16
		float specularMapWeight = 1.0f;
		float bumpMapWeight = 1.0f;
		float normalMapWeight = 1.0f;
		float aoMapWeight = 1.0f;

		float brightness = 1.0f;
		int useShadow = 1;
		//Depth bias is used to have the object cast shadow on itself as the data gets truncated 
		//(Ie. a point is 10.1 gets truncated to 10, so we add a bias to account for the truncation) 
		float depthBias = 0.0f;
		//Padding is used to make the next set up data 16 bytes
		int useSkinning = 1;
	};
	

	struct PostProcessSettingsData
	{
		float screenWidth = 0.0f;
		float screenHeight = 0.0f;
		float time = 0.0f;
		float padding;
	};

	struct BoneTransformBuffer
	{
		MAGE::Math::Matrix4 boneTransforms[128];
	};

	using BoneBuffer = MAGE::Graphics::TypedConstantBuffer<BoneTransformBuffer>;
	using TransformBuffer = MAGE::Graphics::TypedConstantBuffer<TransformData>;
	using LightBuffer = MAGE::Graphics::TypedConstantBuffer<MAGE::Graphics::DirectionalLight>;
	using MaterialBuffer = MAGE::Graphics::TypedConstantBuffer<MAGE::Graphics::Material>;
	using SettingsBuffer = MAGE::Graphics::TypedConstantBuffer<SettingsData>;
	using PostProcessingSettingsBuffer = MAGE::Graphics::TypedConstantBuffer<PostProcessSettingsData>;
	using DepthMapConstantBuffer = MAGE::Graphics::TypedConstantBuffer<MAGE::Math::Matrix4>;
	using ShadowConstantBuffer = MAGE::Graphics::TypedConstantBuffer<MAGE::Math::Matrix4>;

	
	TransformBuffer mTransformBuffer;
	LightBuffer mLightBuffer;
	MaterialBuffer mMaterialBuffer;
	SettingsBuffer mSettingsBuffer;
	PostProcessingSettingsBuffer mPostProcessingSettingsBuffer;

	MAGE::Graphics::DirectionalLight mDirectionalLight;
	MAGE::Graphics::Material mMaterial;

	MAGE::Graphics::VertexShader mVertexShader;
	MAGE::Graphics::PixelShader mPixelShader;

	MAGE::Graphics::Sampler mSampler;
	MAGE::Graphics::Texture mDiffuseMap;
	MAGE::Graphics::Texture mSpecularMap;
	MAGE::Graphics::Texture mDisplacementMap;
	MAGE::Graphics::Texture mNormalMap;
	MAGE::Graphics::Texture mAOMap;
	
	MAGE::Graphics::Texture mGroundDiffuseMap;

	MAGE::Math::Vector3 mWarrokPosition = 0.0f;
	MAGE::Math::Vector3 mWarrokRotation = 0.0f;

	SettingsData mSettings;

	//Shadow
	MAGE::Graphics::RenderTarget mDepthMapRenderTarget; 
	MAGE::Graphics::VertexShader mDepthMapVertexShader;
	MAGE::Graphics::PixelShader mDepthMapPixelShader; 	
	DepthMapConstantBuffer mDepthMapConstantBuffer; 
	ShadowConstantBuffer mShadowConstantBuffer; 

	// Post processing
	MAGE::Graphics::RenderTarget mRenderTarget;
	MAGE::Graphics::MeshPX mScreenQuad;
	MAGE::Graphics::MeshBuffer mScreenQuadBuffer;
	MAGE::Graphics::VertexShader mPostProcessingVertexShader;
	MAGE::Graphics::PixelShader mPostProcessingPixelShader;
	PostProcessSettingsData mPostProcessSettings;

	MAGE::Terrain mTerrain;	

	//Animation
	BoneTransformBuffer boneTransformData;
	BoneTransformBuffer boneWarrockTransformData;
	BoneTransformBuffer bonePaladinTransformData;
	BoneBuffer mBoneBuffer;
	BoneBuffer mWarrockBoneBuffer;
	BoneBuffer mPaladinBoneBuffer;
	std::vector<MAGE::Math::Matrix4> boneMatrices;
	std::vector<MAGE::Math::Matrix4> boneWarrockMatrices;
	std::vector<MAGE::Math::Matrix4> bonePaladinMatrices;

	MAGE::Graphics::Animation mAnimation;
	MAGE::Graphics::Animator mAnimator;
	MAGE::Graphics::AnimationClip mAnimClips;
	bool mShowSkeleton = false;
	bool stopAnim = true;
	bool playAnim = false;
	bool loopAnim = true;
	float mStoppedTime;
	float mCurrentTime;
	float mTime;
	std::string fileName;

	
	bool showWarrock;
	bool showPaladin;
	bool showSamba;
};