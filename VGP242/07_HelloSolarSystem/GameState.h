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

private: 
	MAGE::Graphics::Camera mCamera;
	 
	ConstantBuffer mConstantBuffer;
	MeshBuffer mMeshBuffer;
	MeshPX mMesh;
	MeshBuilder mMeshBuilder;
	Sampler mSampler;
	Texture mTexture;
	VertexShader mVertexShader;
	PixelShader mPixelShader;
	UINT indiceCount;
	float mRotation = 0.0f;
	bool rotateUp = false;
};

