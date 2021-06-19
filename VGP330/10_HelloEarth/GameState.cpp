#include "GameState.h"

using namespace MAGE::Input;
using namespace MAGE::Graphics;
using namespace MAGE::Math;
void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	mCamera.SetPosition({ 0.0f,0.0f,-50.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });

	mMesh = MeshBuilder::CreateSphere(10.0f, 20, 60);
	mMeshBuffer.Initialize(mMesh);

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();

	mDireciontionalLight.direction = Math::Normalize({ 1.0f,-1.0f,1.0f });
	mDireciontionalLight.ambient = { 0.3f };
	mDireciontionalLight.diffuse = { 0.7f };
	mDireciontionalLight.specular = { 0.5f };

	mMaterial.ambient = { 0.0f, 0.6f, 0.7f, 1.0f };
	mMaterial.diffuse = { 1.0f };
	mMaterial.specular = { 1.0f };
	mMaterial.power = { 1.0f };


	std::filesystem::path earthFXFileName = "../../Assets/Shaders/Earth.fx";

	mEarthVertexShader.Initialize(earthFXFileName, "VSEarth",Vertex::Format);
	mEarthPixelShader.Initialize(earthFXFileName, "PSEarth");

	mCloudVertexShader.Initialize(earthFXFileName, "VSCloud", Vertex::Format);
	mCloudPixelShader.Initialize(earthFXFileName, "PSCloud");


	mSampler.Initialize(MAGE::Graphics::Sampler::Filter::Anisotropic, MAGE::Graphics::Sampler::AddressMode::Wrap);
	mDiffuseMap.Initialize("../../Assets/Images/8k_earth.jpg");
	mSpecularMap.Initialize("../../Assets/Images/earth_spec.jpg");
	mDisplacementMap.Initialize("../../Assets/Images/earth_bump.jpg");
	//mNormalMap.Initialize("../../Assets/Images/8k_earth_normal.jpg");
	mNormalMap.Initialize("../../Assets/Images/earth_normal.jpg");
	mCloudMap.Initialize("../../Assets/Images/8k_earth_clouds.jpg"); 
	mNightLightMap.Initialize("../../Assets/Images/8k_earth_lights.jpg");
	mBlendState.Initialize(BlendState::Mode::AlphaPremultiplied);

}
void GameState::Terminate()
{
	mBlendState.Terminate();
	mNormalMap.Terminate();
	mDisplacementMap.Terminate();
	mSpecularMap.Terminate();
	mDiffuseMap.Terminate();

	mNightLightPixelShader.Terminate();
	mNightLightVertexShader.Terminate();
	mCloudPixelShader.Terminate();
	mCloudVertexShader.Terminate();
	mEarthPixelShader.Terminate();
	mEarthVertexShader.Terminate();
	mMeshBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mSettingsBuffer.Terminate();
	mSampler.Terminate();
}
void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 50.5f;
	const float kTurnSpeed = 0.5f;
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::UP))
	{
		mCamera.Walk(kMoveSpeed*deltaTime);
	}
	if (inputSystem->IsKeyDown(KeyCode::DOWN))
	{
		mCamera.Walk(-kMoveSpeed * deltaTime);
	}

	if (inputSystem->IsKeyDown(KeyCode::LEFT))
	{
		mCamera.Strafe(kMoveSpeed * deltaTime);

	}
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
	{
		mCamera.Strafe(-kMoveSpeed * deltaTime);
	}

	if (inputSystem->IsMouseDown(MouseButton::LBUTTON))
	{
		
	}
	
	//Rotate the texture of the class by the earths rotation that you input in ImGUI; 
	 mCloudRotation += 0.4f * 0.005f;
}
void GameState::Render()
{
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	TransformData mTransformData;

	auto matTrans = Matrix4::Translation({10.25f,0.0f,0.0f });
	auto matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
	auto matWorld = matRot * matTrans;

	mTransformData.world = Math::Transpose(matWorld);
	mTransformData.wvp = Math::Transpose(matWorld * matView * matProj);
	mTransformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Update(mTransformData);
	mTransformBuffer.BindVS(0);
	mTransformBuffer.BindPS(0);

	mLightBuffer.Update(mDireciontionalLight);
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

	mDiffuseMap.BindVS();
	mDiffuseMap.BindPS();

	mSpecularMap.BindVS(1);
	mSpecularMap.BindPS(1);

	mDisplacementMap.BindVS(2);
	mDisplacementMap.BindPS(2);

	mNormalMap.BindPS(3);

	mEarthVertexShader.Bind();
	mEarthPixelShader.Bind();

	mMeshBuffer.Draw();


#pragma region CloudBind and Render
	matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y + mCloudRotation);
	matWorld = matRot * matTrans;
	mTransformData.world = Transpose(matWorld);
	mTransformData.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(mTransformData);

	mCloudMap.BindVS(5);
	mCloudMap.BindPS(5);

	mCloudVertexShader.Bind();
	mCloudPixelShader.Bind();
#pragma endregion 
	//When drawing another shader onto a mesh set the blendstate draw meshbuffer then clearstate
	mBlendState.Set();
	mMeshBuffer.Draw();
	BlendState::ClearState();

	mNightLightMap.BindVS(4);
	mNightLightMap.BindPS(4);

	

	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &mDireciontionalLight.direction.x, 0.01f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &mDireciontionalLight.direction.y, 0.01f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &mDireciontionalLight.direction.z, 0.01f, -1.0f, 1.0f);
		if (directionChanged)
		{
			mDireciontionalLight.direction = Math::Normalize(mDireciontionalLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mDireciontionalLight.ambient.x);
		ImGui::ColorEdit4("Diffuse##Light", &mDireciontionalLight.diffuse.x);
		ImGui::ColorEdit4("Specular##Light", &mDireciontionalLight.specular.x);
		//ImGui::DragFloat("Vertex", &mDireciontionalLight.ambient.w);

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
		static bool specularMap = true; 
		static bool normalMap = true; 
		ImGui::SliderFloat("Displacement", &mSettings.BumpMapWeight, 0.0f, 25.0f);
		if (ImGui::Checkbox("Specular Map", &specularMap))
		{
			mSettings.SpecularWeight = specularMap ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Normal Map", &normalMap))
		{
			mSettings.normalMapWeight = normalMap ? 1.0f : 0.0f;
		}

	}

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Rotation##Transform", &mRotation.x, 0.01f);
		ImGui::DragFloat("RotationY##Transform", &mRotation.y, 0.01f);
		ImGui::DragFloat("RotationZ##Transform", &mRotation.z, 0.01f);
	}

	ImGui::End();
}