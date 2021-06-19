#include "GameState.h"

using namespace MAGE::Graphics;
using namespace MAGE::Input;
using namespace MAGE::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	

	mDefaultCamera.SetNearPlane(0.001f);
	mDefaultCamera.SetPosition({ 0.0f, 20.0f, -40.0f });
	mDefaultCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mLightCamera.SetDirection(Normalize({ 1.0f, -1.0f, 1.0f }));
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(500.0f);
	mLightCamera.SetFov(1.0f);
	mLightCamera.SetAspectRatio(1.0f);

	mActiveCamera = &mDefaultCamera;

	//mModel.Initialize("../../Assets/Models/Warrok/warrok.model");	
	//mModel.Initialize("../../Assets/Models/Mutant/mutant.skeleton");
	mModel.Initialize("../../Assets/Models/Parasite/Parasite.model");
	boneMatrices.resize(mModel.skeleton.bones.size());

	UpdatePose(mModel.skeleton.root, boneMatrices);

	mGroundMesh = MeshBuilder::CreatePlane(100.0f);
	mGroundMeshBuffer.Initialize(mGroundMesh);

	mBoneBuffer.Initialize();
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mPostProcessingSettingsBuffer.Initialize();

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.diffuse = { 0.75f, 0.75f, 0.75f, 1.0f };
	mDirectionalLight.specular = { 0.5f, 0.5f, 0.5f, 1.0f };

	mMaterial.ambient = { 0.8f, 0.8f, 0.8f, 1.0f };
	mMaterial.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mMaterial.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	mMaterial.power = 40.0f;

	mSettings.specularMapWeight = 1.0f;
	mSettings.bumpMapWeight = 0.0f;
	mSettings.normalMapWeight = 0.0f;
	mSettings.aoMapWeight = 1.0f;
	mSettings.brightness = 0.5f;
	mSettings.useShadow = 1;
	mSettings.depthBias = 0.001f;

	mVertexShader.Initialize("../../Assets/Shaders/Standard.fx", BoneVertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Standard.fx");

	mGroundDiffuseMap.Initialize("../../Assets/Images/grass_2048.jpg");

	auto graphicsSystem = GraphicsSystem::Get();

	constexpr uint32_t depthMapSize = 1024;
	mDepthMapRenderTarget.Initialize(depthMapSize, depthMapSize, RenderTarget::Format::RGBA_U32);
	mDepthMapVertexShader.Initialize("../../Assets/Shaders/DepthMap.fx", Vertex::Format);
	mDepthMapPixelShader.Initialize("../../Assets/Shaders/DepthMap.fx");
	mDepthMapConstantBuffer.Initialize();
	mShadowConstantBuffer.Initialize();

	mRenderTarget.Initialize(
		graphicsSystem->GetBackBufferWidth(),
		graphicsSystem->GetBackBufferHeight(),
		RenderTarget::Format::RGBA_U8);

	mScreenQuad = MeshBuilder::CreateNDCQuad();
	mScreenQuadBuffer.Initialize(mScreenQuad);

	mPostProcessingVertexShader.Initialize("../../Assets/Shaders/PostProcessing.fx", VertexPX::Format);
	mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcessing.fx", "PSNoProcessing");

	//Method Chaining(Syntactic Sugar) Similar to a recursive function as it calls itself and 
	//adds on to the chain. 

	 //mAnimation = AnimationBuilder()
		//.SetTime(0.0f)
		//	.AddPositionKey(Vector3(-10.0f,0.0f,0.0f))
		//	.AddRotationKey(Quaternion::Identity)
		//	.AddScaleKey(Vector3::One)
		//.SetTime(5.0f)
		//	.AddPositionKey(Vector3(-10.0f,5.0f,0.0f))
		//	.AddRotationKey(Quaternion::RotationAxis(Vector3(-1.0f,1.0f,-1.0f),90.0f))
		//.SetTime(10.0f)
		//	.AddPositionKey(Vector3(-5.0f,5.0f,2.0f))
		//	.AddRotationKey(Quaternion::RotationAxis(Vector3(0.0f,1.0f,0.0f),-90.0f))
		//.SetTime(15.0f)
		//	.AddPositionKey(Vector3(-10.0f,0.0f,0.0f))
		//.AddRotationKey(Quaternion::RotationAxis(Vector3(1.0f, 1.0f, 1.0f), 90.0f))
		// .Get(mAnimation);
	 
	//simple 90 degree rotation 
	mAnimation = AnimationBuilder()
		.SetTime(0.0f)
		.AddPositionKey(Vector3(0.0f, 0.0f, 0.0f))
		.AddRotationKey(Quaternion::Identity)
		.AddScaleKey(Vector3(0.1f, 0.1f, 0.1f))
		/*.SetTime(10.0f)
		.AddPositionKey(Vector3(0.0f, 0.0f, 0.0f))
		.AddRotationKey(Quaternion::RotationAxis(Vector3(0.0f, 1.0f, 0.0f), 90.0f))
		.SetTime(15.0f)
		.AddPositionKey(Vector3(5.0f, 5.0f, 3.0f))
		.SetTime(20.0f)
		.AddPositionKey(Vector3(0.0f, 0.0f, 0.0f))
		.AddRotationKey(Quaternion::Identity)*/
		.Get();
		

}

void GameState::Terminate()
{
	mBoneBuffer.Terminate();
	mModel.Terminate();

	mPostProcessingPixelShader.Terminate();
	mPostProcessingVertexShader.Terminate();
	mScreenQuadBuffer.Terminate();

	mRenderTarget.Terminate();
	mShadowConstantBuffer.Terminate();
	mDepthMapConstantBuffer.Terminate();
	mDepthMapPixelShader.Terminate();
	mDepthMapVertexShader.Terminate();
	mDepthMapRenderTarget.Terminate();
	mGroundDiffuseMap.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mPostProcessingSettingsBuffer.Terminate();

	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mGroundMeshBuffer.Terminate();
	mWarrokMeshBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
	 float kMoveSpeed = 10.0f;
	const float kTurnSpeed = 1.0f;

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::LSHIFT))
		kMoveSpeed = 50.0f;

	//Left right up down
	if (inputSystem->IsKeyDown(KeyCode::W))
		mDefaultCamera.Walk(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mDefaultCamera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mDefaultCamera.Strafe(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mDefaultCamera.Strafe(-kMoveSpeed * deltaTime);

	
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
		mDefaultCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);	
	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
		mDefaultCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);


	if (inputSystem->IsKeyDown(KeyCode::R))
		mDefaultCamera.SetPosition({ 0.0f, 20.0f, -40.0f });

	mPostProcessSettings.time += deltaTime;

	mLightCamera.SetPosition(mWarrokPosition - mLightCamera.GetDirection() * 50.0f);
	if(!stopAnim)
	mTime += deltaTime;
}

void GameState::Render()
{
	mDepthMapRenderTarget.BeginRender();
	DrawDepthMap();
	mDepthMapRenderTarget.EndRender();

	mRenderTarget.BeginRender();
	DrawScene();
	mRenderTarget.EndRender();

	mRenderTarget.BindPS(0);
	PostProcess();
	mRenderTarget.UnbindPS(0);

}

void GameState::DebugUI()
{
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Show Skeleton", &mShowSkeleton);
		if (ImGui::Checkbox("Stop Animation", &stopAnim))
		{
			playAnim = false;
			mStoppedTime = mTime;
			mCurrentTime = mStoppedTime;
		}
		if (ImGui::Checkbox("Playing Animation", &playAnim))
		{
			stopAnim = false;
			mTime = mTime ? mStoppedTime : mCurrentTime;
		}
		ImGui::SliderFloat("Rewind Time", &mTime, 0.0f, mStoppedTime, "%.01f");
	
		ImGui::Text("PosX %f, PosY = %f, PosZ = %f", mAnimation.GetPosition(mTime).x, mAnimation.GetPosition(mTime).y, mAnimation.GetPosition(mTime).z);
		ImGui::Text("Rotx %f, RotY = %f, RotZ = %f", mAnimation.GetRotation(mTime).x * (180 / Constants::Pi), mAnimation.GetRotation(mTime).y * (180 / Constants::Pi), mAnimation.GetRotation(mTime).z);
		/*ImGui::Text("._11 = %f, ._12 = %f, ._13 = %f, ._14 = %f\n"
			"._21 = %f, ._22 = %f, ._23 = %f, .24 = %f\n"
			"._31 = %f, ._32 = %f, ._33 = %f, .34 = %f\n",
			mAnimation.GetTransformation(currentTime)., mAnimation.GetTransformation(currentTime)._12, mAnimation.GetTransformation(currentTime)._13);*/
	}

	ImGui::End();
}

void GameState::DrawDepthMap()
{
	mDepthMapVertexShader.Bind();
	mDepthMapPixelShader.Bind();

	auto matViewLight = mLightCamera.GetViewMatrix();
	auto matProjLight = mLightCamera.GetPerspectiveMatrix();

	auto matTrans = Matrix4::Translation(mWarrokPosition);
	auto matRot = Matrix4::RotationX(mWarrokRotation.x) * Matrix4::RotationY(mWarrokRotation.y);
	//auto matWorld = matRot * matTrans;
	auto matWorld = mAnimation.GetTransformation(mTime);
	auto wvp = Transpose(matWorld * matViewLight * matProjLight);

	mDepthMapConstantBuffer.Update(wvp);
	mDepthMapConstantBuffer.BindVS(0);

	mModel.Draw();
}

void GameState::DrawScene()
{
	auto matView = mActiveCamera->GetViewMatrix();
	auto matProj = mActiveCamera->GetPerspectiveMatrix();
	auto matViewLight = mLightCamera.GetViewMatrix();
	auto matProjLight = mLightCamera.GetPerspectiveMatrix();

	mLightBuffer.Update(mDirectionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.Update(mSettings);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mSampler.BindVS();
	mSampler.BindPS();

	mDiffuseMap.BindPS(0);
	mSpecularMap.BindPS(1);
	mDisplacementMap.BindVS(2);
	mNormalMap.BindPS(3);
	mAOMap.BindPS(4);
	mDepthMapRenderTarget.BindPS(5);

	auto matTrans = Matrix4::Translation(mWarrokPosition);
	auto matRot = Matrix4::RotationX(mWarrokRotation.x) * Matrix4::RotationY(mWarrokRotation.y);
	auto matWorld = mAnimation.GetTransformation(mTime);

	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	transformData.viewPosition = mActiveCamera->GetPosition();
	mTransformBuffer.Update(transformData);
	mTransformBuffer.BindVS(0);

	auto wvpLight = Transpose(matWorld * matViewLight * matProjLight);
	mShadowConstantBuffer.Update(wvpLight);
	mShadowConstantBuffer.BindVS(4);

	mVertexShader.Bind();
	mPixelShader.Bind();

	

	mBoneBuffer.BindVS(5);
	
	for (size_t i = 0; i < boneMatrices.size(); ++i)
	{
		//Transpose to righthand
		boneTransformData.boneTransforms[i] = MAGE::Math::Transpose(boneMatrices[i] * mModel.skeleton.bones[i].get()->offsetTransform);
	}

	mBoneBuffer.Update(boneTransformData);
	if (mShowSkeleton)
	{
		for (auto& bones : mModel.skeleton.bones)
		{
			DrawSkeleton(bones.get(), boneMatrices);
		}
	}
	else
	{
		mModel.Draw();
	}

	matWorld = Matrix4::Identity;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(transformData);

	wvpLight = Transpose(matWorld * matViewLight * matProjLight);
	mShadowConstantBuffer.Update(wvpLight);
	mShadowConstantBuffer.BindVS(4);

	mGroundDiffuseMap.BindPS(0);

	SettingsData settings;
	settings.specularMapWeight = 0.0f;
	settings.bumpMapWeight = 0.0f;
	settings.normalMapWeight = 0.0f;
	settings.aoMapWeight = 0.0f;
	settings.useShadow = 1;
	mSettingsBuffer.Update(settings);

	mGroundMeshBuffer.Draw();

	SimpleDraw::Render(*mActiveCamera);
}

void GameState::PostProcess()
{
	mPostProcessingVertexShader.Bind();
	mPostProcessingPixelShader.Bind();

	mSampler.BindPS();

	auto graphicsSystem = GraphicsSystem::Get();
	mPostProcessSettings.screenWidth = (float)graphicsSystem->GetBackBufferWidth();
	mPostProcessSettings.screenHeight = (float)graphicsSystem->GetBackBufferHeight();
	mPostProcessingSettingsBuffer.Update(mPostProcessSettings);
	mPostProcessingSettingsBuffer.BindVS(0);
	mPostProcessingSettingsBuffer.BindPS(0);

	mScreenQuadBuffer.Draw();
}
