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
	 
	ConstantBuffer mConstantBuffer;
	MeshBuffer mMeshBuffer;
	VertexShader mVertexShader;
	PixelShader mPixelShader;
	MeshPC mMesh;
	UINT indiceCount;
	float mRotation = 0.0f;

};

