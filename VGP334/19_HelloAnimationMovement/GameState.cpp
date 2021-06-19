#include "GameState.h"

#include <ImGui/Inc/ImGuiFileDialog.h>
#include <commdlg.h>
#include <string>
#include <algorithm>
using namespace MAGE::Graphics;
using namespace MAGE::Input;
using namespace MAGE::Math;

static int choice;
static int toggleplay;
static bool toggleMovement;
static int movementSpeed = 150;
static int index;

void GameState::Initialize()
{
	index = 0;
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mDefaultCamera.SetNearPlane(0.1f);
	
	mDefaultCamera.SetPosition({ 150.0f, 350.0f, -500.0f });
	mDefaultCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mLightCamera.SetDirection(Normalize({ 1.0f, -1.0f, 1.0f }));
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(500.0f);
	mLightCamera.SetFov(1.0f);
	mLightCamera.SetAspectRatio(1.0f);
	
	mActiveCamera = &mDefaultCamera;

	mWorld.Initialize(100);
	mWorld.Create("../../Assets/Templates/TallBox.json", "model");

	mSkydome.Initialize("../../Assets/Skybox/EveningSky.jpg");
	//mModel.Initialize("../../Assets/Models/Samba/SambaDancing.model");
	mModel.Initialize("../../Assets/Models/Paladin/paladinIdle.model");
	mAnimator.Initialize(mModel);
	
	boneMatrices.resize(mModel.skeleton.bones.size());
	UpdatePose(mModel.skeleton.root, boneMatrices);

	//Animator needs to compute pose first before playing. 
	//mAnimator.ComputeBindPose();
	//Sets the animation clip at the beginning
	//mAnimator.PlayAnimation(0);
	//mAnimator.BlendTo(0, 2.0f);
	mGroundMesh = MeshBuilder::CreatePlane(1000.0f);
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


	mGroundDiffuseMap.Initialize("../../Assets/Images/grass_2048.jpg");

	mVertexShader.Initialize("../../Assets/Shaders/Standard.fx", BoneVertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Standard.fx");

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
		RenderTarget::Format::RGBA_U32);

	mScreenQuad = MeshBuilder::CreateNDCQuad();
	mScreenQuadBuffer.Initialize(mScreenQuad);

	mPostProcessingVertexShader.Initialize("../../Assets/Shaders/PostProcessing.fx", VertexPX::Format);
	mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcessing.fx", "PSNoProcessing");

	//Method Chaining(Syntactic Sugar) Similar to a recursive function as it calls itself and 
	//adds on to the chain. 

	mModelPos = { 200.f, 20.f, 400.f };
	//simple 90 degree rotation 
	mAnimation = AnimationBuilder()
		.SetTime(0.0f)
		.AddScaleKey(Vector3(1.f, 1.f, 1.f))
		.AddPositionKey(mModelPos)
		/*.SetTime(10.0f)
		.AddPositionKey(Vector3(0.0f, 0.0f, 0.0f))
		.AddRotationKey(Quaternion::RotationAxis(Vector3(0.0f, 1.0f, 0.0f), 90.0f))
		.SetTime(15.0f)
		.AddPositionKey(Vector3(5.0f, 5.0f, 3.0f))
		.SetTime(20.0f)
		.AddPositionKey(Vector3(0.0f, 0.0f, 0.0f))
		.AddRotationKey(Quaternion::Identity)*/
		.Get();
	mTerrain.Initialize(1000, 1000, 20.0f);
	mTerrain.SetPosition( {-800.0f,0.0f,-500.0f });	
	mWorld.Find("model").Get()->GetComponent<MAGE::TransformComponent>()->mPosition = mModelPos;

	//mTerrain.SetHeightScale(0.f);
	//mTerrain.LoadHeightmap("../../Assets/Heightmaps/heightmap_200x200.raw");

}

void GameState::Terminate()
{
	mSkydome.Terminate();
	mBoneBuffer.Terminate();
	mModel.Terminate();
	mAnimator.Terminate();

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
}

void GameState::Update(float deltaTime)
{
	float kMoveSpeed = 50.0f;
	const float kTurnSpeed = 1.0f;
	
	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::LSHIFT))
		kMoveSpeed = 250.0f;

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
		mDefaultCamera.SetPosition({ 0.0f, 200.0f, -500.0f });


	mModelPos = mWorld.Find("model").Get()->GetComponent<MAGE::TransformComponent>()->mPosition;
	if (inputSystem->IsKeyDown(KeyCode::C))
	{
		if (!toggleMovement)
			toggleMovement = true;
		else
			toggleMovement = false;
	}

	if (inputSystem->IsKeyDown(KeyCode::P))
	{
		if (toggleplay == 0)
		{
			toggleplay = 1;
		}
		else
		{
			toggleplay = 0;
		}
	}	 
	if (toggleMovement)
	{
		mDefaultCamera.SetPosition(mModelPos - mModelDir * 2);
		mDefaultCamera.SetYPosition(mModelPos.y, 200.0f, 150.0f);

		Move(deltaTime);
		if (inputSystem->IsKeyDown(KeyCode::LEFT))
		{
			MAGE::Math::Quaternion rotation = Quaternion::RotationAxis(MAGE::Math::Vector3::YAxis, 90.0f * Constants::DegToRad);
			mWorld.Find("model").Get()->GetComponent<MAGE::TransformComponent>()->mPosition.x -= movementSpeed * deltaTime;
			mWorld.Find("model").Get()->GetComponent<MAGE::TransformComponent>()
				->SetQuaternion(rotation);
			index = 2;
		}

		if (inputSystem->IsKeyDown(KeyCode::RIGHT))
		{
			MAGE::Math::Quaternion rotation = Quaternion::RotationAxis(MAGE::Math::Vector3::YAxis, -90.0f * Constants::DegToRad);
			mWorld.Find("model").Get()->GetComponent<MAGE::TransformComponent>()->mPosition.x += movementSpeed * deltaTime;
			mWorld.Find("model").Get()->GetComponent<MAGE::TransformComponent>()
				->SetQuaternion(rotation);
			//mModelDir = TransformNormal(mModelDir, Matrix4::RotationY(-90.0f * Constants::DegToRad));
			index = 2;
		}

		if (inputSystem->IsKeyDown(KeyCode::UP))
		{
			MAGE::Math::Quaternion rotation = Quaternion::RotationAxis(MAGE::Math::Vector3::YAxis, 180.0f * Constants::DegToRad);
			mWorld.Find("model").Get()->GetComponent<MAGE::TransformComponent>()->mPosition.z += movementSpeed * deltaTime;
			mWorld.Find("model").Get()->GetComponent<MAGE::TransformComponent>()
				->SetQuaternion(rotation);
			index = 2;
		}

		if (inputSystem->IsKeyDown(KeyCode::DOWN))
		{
			MAGE::Math::Quaternion rotation = Quaternion::RotationAxis(MAGE::Math::Vector3::YAxis, 0);
			mWorld.Find("model").Get()->GetComponent<MAGE::TransformComponent>()->mPosition.z -= movementSpeed * deltaTime;
			mWorld.Find("model").Get()->GetComponent<MAGE::TransformComponent>()
				->SetQuaternion(rotation);						
			mModelDir = TransformNormal(mModelDir, Matrix4::RotationY(0));
			index = 2;
		}

		if (inputSystem->IsKeyDown(KeyCode::X))
		{
			index = 1;
		}	

		//if(mtime > 0.1f && stopAnimation == false)
		//return;
		if (inputSystem->IsKeyDown(KeyCode::SPACE))
		{
			index = 3;
		}

		if (inputSystem->IsKeyDown(KeyCode::K))
		{
			index = 4;
		}
	}	

	mPostProcessSettings.time += deltaTime;
	
	mLightCamera.SetPosition(mWarrokPosition - mLightCamera.GetDirection() * 50.0f);
	if (toggleplay == 1)
	{
		mTime += deltaTime * mModel.animationSet.clips[index].get()->ticksPerSecond;
		if (mTime > mModel.animationSet.clips[index].get()->duration)
		{			
			if (mModel.animationSet.clips[index] == 0)
			{
				mTime = deltaTime * mModel.animationSet.clips[index].get()->ticksPerSecond;
			}
			else
			{
				playAnim = false;
			}
			if (loopAnim)
				mTime = deltaTime * mModel.animationSet.clips[index].get()->ticksPerSecond;
			else
			{
				mTime -= deltaTime * mModel.animationSet.clips[index].get()->ticksPerSecond;
				playAnim = false;
			}

		}
	}
		UpdateAnimationRecursive(boneMatrices, mModel.skeleton.root, *mModel.animationSet.clips[index].get(), mTime);
	mSkydome.Update(mDefaultCamera);		
}

void GameState::Move(float deltaTime)
{
	mModelPos += mModelDir * 2.0f * deltaTime;
}

//void GameState::Turn(Quaternion& rotation)
//{
//	mDirection = TransformNormal(mWorld.Find("model").Get()->GetComponent<MAGE::TransformComponent>()->GetPos(), rotation),
//}

void GameState::Render()
{
	mRenderTarget.BeginRender();
	DrawScene();
	mRenderTarget.EndRender();

	mRenderTarget.BindPS(0);
	PostProcess();
	mRenderTarget.UnbindPS(0);
}

void GameState::DebugUI()
{
	bool toolActive = true;
	bool open = false;

	ImGui::Begin("Animation Settings", &toolActive, ImGuiWindowFlags_MenuBar);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("Open"))
				{
					std::string filepath = "C:\\Users\\Stan Gerard Palisoc\\Desktop\\MAGE\\Assets\\Models\\";
					//ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".model", ".");
					ImGuiFileDialog::Instance()->OpenModal("ChooseFileDlgKey", "Choose File", ".model", filepath);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (ImGui::CollapsingHeader("File", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("File Name: %s", fileName.c_str());
			
		}
		if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
		{	
			ImGui::Checkbox("Control Animation", &toggleMovement);
			ImGui::SliderInt("Movement Speed", &movementSpeed, 100, 200, "%d");
			ImGui::Checkbox("Show Skeleton", &mShowSkeleton);
			if (ImGui::RadioButton("Stop Animation", &toggleplay, 0))
			{
				mStoppedTime = mTime;
				mCurrentTime = mStoppedTime;
			}
			if (ImGui::RadioButton("Play Animation", &toggleplay, 1))
			{
				mTime = mTime ? mStoppedTime : mCurrentTime;

			}	
			ImGui::Checkbox("Loop", &loopAnim);
			ImGui::SliderFloat("Rewind Time", &mTime, 0.0f, mStoppedTime, "%.01f");
			//ImGui::Text("PosX %f, PosY = %f, PosZ = %f", mAnimation.GetPosition(mTime).x, mAnimation.GetPosition(mTime).y, mAnimation.GetPosition(mTime).z);
			//ImGui::Text("Rotx %f, RotY = %f, RotZ = %f", mAnimation.GetRotation(mTime).x * (180 / Constants::Pi), mAnimation.GetRotation(mTime).y * (180 / Constants::Pi), mAnimation.GetRotation(mTime).z);
			//ImGui::Text("Camera PosX = %d, Camera PosY = %d, Camera PosZ = %d", mDefaultCamera.GetPosition().x, mDefaultCamera.GetPosition().y, mDefaultCamera.GetPosition().z);
			//ImGui::Text("ModelDir Posy = %d", mModelDir.y);

		}

		ImGui::End();
		ImGui::Begin("Light Settings", &toolActive, ImGuiWindowFlags_MenuBar);
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
		}
		if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			static bool specularMap = mSettings.specularMapWeight > 0.0f;
			static bool normalMap = mSettings.normalMapWeight > 0.0f;
			static bool aoMap = mSettings.aoMapWeight > 0.0f;
			static bool useShadow = mSettings.useShadow == 1;
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
			ImGui::SliderFloat("Brightness", &mSettings.brightness, 1.0f, 10.0f);
		}
		ImGui::End();


		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
				
				boneMatrices.clear();
				mModel.Terminate();
				mBoneBuffer.Terminate();
				std::string splitfile = filePathName;				
				std::replace(splitfile.begin(), splitfile.end(),'\\', '/');
				mModel.Initialize(splitfile);
				std::size_t botDirPos = splitfile.find_last_of("/");
				std::string dir = splitfile.substr(0, botDirPos);
				std::string file = splitfile.substr(botDirPos + 1, splitfile.length());
				fileName = file;
				mCurrentTime = 0;
				//mAnimator.Initialize(mModel);
				boneMatrices.resize(mModel.skeleton.bones.size());
				UpdatePose(mModel.skeleton.root, boneMatrices);
				mBoneBuffer.Initialize();
				//Animator needs to compute pose first before playing. 
				//mAnimator.ComputeBindPose();
				//Sets the animation clip at the beginning
				//mAnimator.PlayAnimation(0);
			}

			ImGuiFileDialog::Instance()->Close();
		}
	}
}

void GameState::DrawDepthMap()
{

}

void GameState::DrawScene()
{
	auto matView = mActiveCamera->GetViewMatrix();
	auto matProj = mActiveCamera->GetPerspectiveMatrix();
	auto matViewLight = mLightCamera.GetViewMatrix();
	auto matProjLight = mLightCamera.GetPerspectiveMatrix();

	mSkydome.Render(*mActiveCamera);	
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


	auto matTrans = Matrix4::Translation(mWarrokPosition);
	//auto matRot = Matrix4::RotationX(mWarrokRotation.x) * Matrix4::RotationY(mWarrokRotation.y);
	auto matRot = Matrix4::RotationQuaternion(mWorld.Find("model").Get()->GetComponent<MAGE::TransformComponent>()->GetQuaternion());	

	auto matWorld =  Matrix4::Translation(mWorld.Find("model").Get()->GetComponent<MAGE::TransformComponent>()->GetPos());

	//auto matWorld = mAnimation.GetTransformation(mTime);

	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matRot * matWorld * matView * matProj);
	transformData.viewPosition = mActiveCamera->GetPosition();
	mTransformBuffer.Update(transformData);
	mTransformBuffer.BindVS(0);

	auto wvpLight = Transpose(matWorld * matViewLight * matProjLight);
	mShadowConstantBuffer.Update(wvpLight);
	mShadowConstantBuffer.BindVS(4);

	mVertexShader.Bind();
	mPixelShader.Bind();

	mBoneBuffer.BindVS(5);
	//fileName = "Lady.model";
	if (!mShowSkeleton)
	{
		for (size_t i = 0; i < boneMatrices.size(); ++i)
		{
			//Transpose to righthand
			boneTransformData.boneTransforms[i] = MAGE::Math::Transpose(mModel.skeleton.bones[i].get()->offsetTransform * boneMatrices[i]);
		}
		mBoneBuffer.Update(boneTransformData);
		mModel.Draw();
	}
	else
	{
		DrawSkeleton(mModel.skeleton.root, boneMatrices, mWorld.Find("model").Get()->GetComponent<MAGE::TransformComponent>()->GetPos());
	}

	matWorld = Matrix4::Identity;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(transformData);

	wvpLight = Transpose(matWorld * matViewLight * matProjLight);
	mShadowConstantBuffer.Update(wvpLight);
	mShadowConstantBuffer.BindVS(4);

	SettingsData settings;
	settings.specularMapWeight = 0.0f;
	settings.bumpMapWeight = 0.0f;
	settings.normalMapWeight = 0.0f;
	settings.aoMapWeight = 0.0f;
	settings.useShadow = 1;
	mSettingsBuffer.Update(settings);

	mGroundMeshBuffer.Draw();

	mTerrain.SetDirectionalLight(mDirectionalLight);
	mTerrain.Render(*mActiveCamera);

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


/*
Model Loader Command Argument
"C:\Users\Stan Gerard Palisoc\Desktop\MAGE\Assets\Models\Samba\SambaDancing.fbx" "C:\Users\Stan Gerard Palisoc\Desktop\MAGE\Assets\Models\Samba\SambaDancing.model"
*/