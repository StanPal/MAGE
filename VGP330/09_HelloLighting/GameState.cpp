#include "GameState.h"

using namespace MAGE::Input;
using namespace MAGE::Graphics;
using namespace MAGE::Math;
void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	mCamera.SetPosition({ 0.0f,0.0f,-25.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });

	mMesh = MeshBuilder::CreateSphere(5.0f,8,16);
	mMeshBuffer.Initialize(mMesh);

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mDireciontionalLight.direction = Math::Normalize({ 1.0f,-1.0f,1.0f });
	mDireciontionalLight.ambient = { 0.3f };
	mDireciontionalLight.diffuse = { 0.7f };
	mDireciontionalLight.specular = { 0.5f };

	mMaterial.ambient = { 0.0f, 0.6f, 0.7f, 1.0f };
	mMaterial.diffuse = { 1.0f };
	mMaterial.specular = { 1.0f };
	mMaterial.power = { 1.0f };

	mGouraudShadingVertexShader.Initialize("../../Assets/Shaders/DoGouraudShading.fx", VertexPN::Format);
	mPhongShadingVertexShader.Initialize("../../Assets/Shaders/DoPhongShading.fx", VertexPN::Format);

	mGouraudShadingPixelShader.Initialize("../../Assets/Shaders/DoGouraudShading.fx");
	mPhongShadingPixelShader.Initialize("../../Assets/Shaders/DoPhongShading.fx");


}
void GameState::Terminate()
{
	mPhongShadingPixelShader.Terminate();
	mPhongShadingVertexShader.Terminate();
	mGouraudShadingPixelShader.Terminate();
	mGouraudShadingVertexShader.Terminate();

	mMeshBuffer.Terminate();
	mMaterialBuffer.Initialize();
	mLightBuffer.Initialize();
	mTransformBuffer.Initialize();
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
	//mCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
	//mCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::LEFT))
	{
		mCamera.Strafe(kMoveSpeed * deltaTime);

	}
	if (inputSystem->IsKeyDown(KeyCode::RIGHT))
	{
		mCamera.Strafe(-kMoveSpeed * deltaTime);

	}
	

}
void GameState::Render()
{
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();
	TransformData mTransformData;

	mLightBuffer.Update(mDireciontionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);


	//Left Sphere Gouraud Shading
	{
		auto matTrans = Matrix4::Translation({ -10.25f,0.0f,0.0f });
		auto matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
		auto matWorld = matRot * matTrans;
		mTransformData.world = Math::Transpose(matWorld);
		mTransformData.wvp = Math::Transpose(matWorld * matView * matProj);
		mTransformData.viewPosition = mCamera.GetPosition();

		mTransformBuffer.Update(mTransformData);
		mTransformBuffer.BindVS(0);

		mGouraudShadingVertexShader.Bind();
		mGouraudShadingPixelShader.Bind();
		
		mMeshBuffer.Draw();
	}

	//Right Sphere Phong Shading 
	{
		auto matTrans = Matrix4::Translation({ 10.25f,0.0f,0.0f });
		auto matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
		auto matWorld = matRot * matTrans;
		mTransformData.world = Math::Transpose(matWorld);
		mTransformData.wvp = Math::Transpose(matWorld * matView * matProj);
		mTransformData.viewPosition = mCamera.GetPosition();
		mTransformBuffer.Update(mTransformData);

		mPhongShadingVertexShader.Bind();
		mPhongShadingPixelShader.Bind();

		mMeshBuffer.Draw();
	}

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
	}
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.x);
		ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.x);
		ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.x);
		ImGui::DragFloat("Power##Material", &mMaterial.power, 1.0f, 1.0f, 100.0f);
	}

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("RotationX##Transform", &mRotation.x, 0.01f);
		ImGui::DragFloat("RotationY##Transform", &mRotation.y, 0.01f);
		ImGui::DragFloat("RotationZ##Transform", &mRotation.z, 0.01f);

	}

	ImGui::End();
}