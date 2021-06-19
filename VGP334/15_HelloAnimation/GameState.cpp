#include "GameState.h"

using namespace MAGE::Graphics;
using namespace MAGE::Input;
using namespace MAGE::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	
	mPigPosition = { 0.0f, 0.0f, 0.0f };

	mDefaultCamera.SetNearPlane(0.001f);
	mDefaultCamera.SetPosition({ 0.0f, 20.0f, -40.0f });
	mDefaultCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mLightCamera.SetDirection(Normalize({ 1.0f, -1.0f, 1.0f }));
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(500.0f);
	mLightCamera.SetFov(1.0f);
	mLightCamera.SetAspectRatio(1.0f);

	mActiveCamera = &mDefaultCamera;

	mPigMesh = OBJLoader::Load("../../Assets/Models/Pig/Pig.obj", 0.1f);
	mPigMeshBuffer.Initialize(mPigMesh);

	mGroundMesh = MeshBuilder::CreatePlane(100.0f);
	mGroundMeshBuffer.Initialize(mGroundMesh);

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
	mSettings.brightness = 3.5f;
	mSettings.useShadow = 1;
	mSettings.depthBias = 0.001f;

	mVertexShader.Initialize("../../Assets/Shaders/Standard.fx", Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Standard.fx");

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
	mDiffuseMap.Initialize("../../Assets/Models/Tank/tank_diffuse.jpg");
	mSpecularMap.Initialize("../../Assets/Models/Tank/tank_specular.jpg");
	mNormalMap.Initialize("../../Assets/Models/Tank/tank_normal.jpg");
	mAOMap.Initialize("../../Assets/Models/Tank/tank_ao.jpg");

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
		.AddScaleKey(Vector3::One)
		.SetTime(5.0f)
		.AddPositionKey(Vector3(0.0f, 0.0f, 0.0f))
		.AddRotationKey(Quaternion::RotationAxis(Vector3(0.0f, 1.0f, 0.0f), 90.0f))
		.Get();
		

}

void GameState::Terminate()
{
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
	mAOMap.Terminate();
	mNormalMap.Terminate();
	mDisplacementMap.Terminate();
	mSpecularMap.Terminate();
	mDiffuseMap.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mPostProcessingSettingsBuffer.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mGroundMeshBuffer.Terminate();
	mPigMeshBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 10.0f;
	const float kTurnSpeed = 1.0f;

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
		mDefaultCamera.Walk(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mDefaultCamera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mDefaultCamera.Strafe(kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mDefaultCamera.Strafe(-kMoveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mDefaultCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
	}

	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
	{
		mDefaultCamera.Pitch(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);

	}

	if (inputSystem->IsKeyDown(KeyCode::UP))
		mPigRotation.x += deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
		mPigRotation.x -= deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::LEFT))
		mPigRotation.y += deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
		mPigRotation.y -= deltaTime;

	mPostProcessSettings.time += deltaTime;

	mLightCamera.SetPosition(mPigPosition - mLightCamera.GetDirection() * 50.0f);
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
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool lightCamera = mActiveCamera == &mLightCamera;
		if (ImGui::Checkbox("Use Light Camera", &lightCamera))
		{
			mActiveCamera = lightCamera ? &mLightCamera : &mDefaultCamera;
		}

		ImGui::Image(
			mDepthMapRenderTarget.GetShaderResourceView(),
			{ 150.0f, 150.0f },
			{ 0.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);
	}
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &mDirectionalLight.direction.x, 0.01f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &mDirectionalLight.direction.y, 0.01f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &mDirectionalLight.direction.z, 0.01f, -1.0f, 1.0f);
		if (directionChanged)
		{
			mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.x);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.x);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.x);
	}
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.x);
		ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.x);
		ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.x);
		ImGui::DragFloat("Power##Material", &mMaterial.power, 1.0f, 1.0f, 100.0f);
	}
	if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		static bool specularMap = mSettings.specularMapWeight > 0.0f;
		static bool normalMap = mSettings.normalMapWeight > 0.0f;
		static bool aoMap = mSettings.aoMapWeight > 0.0f;
		static bool useShadow = mSettings.useShadow == 1;
		ImGui::SliderFloat("Displacement", &mSettings.bumpMapWeight, 0.0f, 1.0f);
		if (ImGui::Checkbox("Specular Map", &specularMap))
		{
			mSettings.specularMapWeight = specularMap ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Normal Map", &normalMap))
		{
			mSettings.normalMapWeight = normalMap ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Ambient Occlusion Map", &aoMap))
		{
			mSettings.aoMapWeight = aoMap ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Use Shadow", &useShadow))
		{
			mSettings.useShadow = useShadow ? 1 : 0;
		}
		ImGui::SliderFloat("Depth Bias", &mSettings.depthBias, 0.0f, 0.01f, "%.4f");
		ImGui::SliderFloat("Brightness", &mSettings.brightness, 1.0f, 10.0f);
	}
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Translation##Transform", &mPigPosition.x, 0.01f);
		ImGui::DragFloat3("Rotation##Transform", &mPigRotation.x, 0.01f);
	}
	ImGui::End();
}

void GameState::DrawDepthMap()
{
	mDepthMapVertexShader.Bind();
	mDepthMapPixelShader.Bind();

	auto matViewLight = mLightCamera.GetViewMatrix();
	auto matProjLight = mLightCamera.GetPerspectiveMatrix();

	auto matTrans = Matrix4::Translation(mPigPosition);
	auto matRot = Matrix4::RotationX(mPigRotation.x) * Matrix4::RotationY(mPigRotation.y);
	auto matWorld = matRot * matTrans;
	auto wvp = mAnimation.GetTransformation(mTime);
	mDepthMapConstantBuffer.Update(wvp);
	mDepthMapConstantBuffer.BindVS(0);

	mPigMeshBuffer.Draw();
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

	auto matTrans = Matrix4::Translation(mPigPosition);
	auto matRot = Matrix4::RotationX(mPigRotation.x) * Matrix4::RotationY(mPigRotation.y);
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

	mPigMeshBuffer.Draw();

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
