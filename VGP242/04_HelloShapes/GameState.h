#pragma once
#include <MAGEEngine/Inc/MAGE.h>

class GameState : public MAGE::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;

private: 
	struct Vertex
	{
		MAGE::Math::Vector3 Position; 
		MAGE::Graphics::Color Color; 
	};
	int mSelection = 0;

	std::vector<Vertex> mVertices; 
	std::vector<Vertex> mHeart;
	std::vector<Vertex> mTriforce; 
	std::vector<Vertex> mShip;
	//Add 04_HelloShapes project
	//Copy code from 03_HelloTriangle
	//Update to use DoSomething.fx shaders
	//Update Vertex to have Color as well 
	//Make sure inputlayout is initialized with correct description 
	//Create multiple meshes and vertex buffers
	//HW Draw multiple shapes
	//draw a heart
	//draw a triforce
	//draw something with more than 5 triangles
	//Use input check so you can change between them 
	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11Buffer* mVertexHeartBuffer = nullptr;
	ID3D11Buffer* mVertexShipBuffer = nullptr;


	ID3D11VertexShader* mVertexShader = nullptr;
	ID3D11InputLayout* mInputLayout = nullptr;
	ID3D11PixelShader* mPixelShader = nullptr;

	void InitTriforce();
	void InitHeart();
	void InitShip();
};

