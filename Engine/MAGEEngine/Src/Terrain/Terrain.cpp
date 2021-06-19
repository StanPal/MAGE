#include "Input/Src/Precompiled.h"
#include "Terrain/Terrain.h"

using namespace MAGE;

void MAGE::Terrain::Initialize(uint32_t numRows, uint32_t numCols, float cellSize)
{
	mNumRows = numRows;
	mNumCols = numCols;
	mNumCellsInRow = mNumRows - 1;
	mNumCellsInCol = mNumCols - 1;
	mNumCells = mNumCellsInCol * mNumCellsInRow;
	mCellSize = cellSize;


	mConstantBuffer.Initialize();
	mTerrainVertexShader.Initialize(L"../../Assets/Shaders/Terrain.fx", Graphics::Vertex::Format);
	mTerrainPixelShader.Initialize(L"../../Assets/Shaders/Terrain.fx");

	mSampler.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mGrassTexture.Initialize("../../Assets/Images/grass_2048.jpg");

	GenerateIndices();
	GenerateVertices();

	mMeshBuffer.Initialize(mMesh, true);
}

void MAGE::Terrain::Terminate()
{
	mMeshBuffer.Terminate();
	mGrassTexture.Terminate();
	mSampler.Terminate();

	mTerrainPixelShader.Terminate();
	mTerrainVertexShader.Terminate();
	mConstantBuffer.Terminate();
}

void MAGE::Terrain::LoadHeightmap(const std::filesystem::path& filePath)
{
	FILE* file = nullptr; 
	fopen_s(&file, filePath.u8string().c_str(), "rb");
	fseek(file, 0L, SEEK_END);
	uint32_t fileSize = ftell(file);
	uint32_t dimension = (uint32_t)sqrtf((float)fileSize);
	fseek(file, 0L, SEEK_SET);


	for (uint32_t z = 0; z < mNumRows && z < dimension; ++z)
	{
		for (uint32_t x = 0; x < mNumCols && x < dimension; ++x)
		{
			float height = fgetc(file) / 255.0f * mHeightScale;
			mMesh.mVertices[x + ((mNumCols - z - 1) * mNumCols)].position.y = height;
		}
	}

	fclose(file);

	//Graphics::MeshBuilder::ComputerNormals(mMesh);

	//Grab 3 indices at a time (subtract by 2) get the cross product of those indices. 
	//Create a normals array with that many vertices of vertices.size()
	//Loop through normals array again then normalize them then put it back in. 
	//After you raise all the vertices you need to apply it to the mesh.
	mMeshBuffer.Update(mMesh.mVertices.data(), static_cast<uint32_t>(mMesh.mVertices.size()));
}

void Terrain::SetDirectionalLight(const Graphics::DirectionalLight& light)
{
	mConstantData.directionalLight = light;
}

void MAGE::Terrain::SetHeightScale(float scale)
{
	mHeightScale = scale;
}

void MAGE::Terrain::SetPosition(const Math::Vector3 &pos)
{
	mTerrainPosition = pos;
}

void MAGE::Terrain::Render(const Graphics::Camera& camera)
{
	//auto world = Math::Matrix4::Identity;
	auto world = Math::Matrix4::Translation(mTerrainPosition);
	auto view = camera.GetViewMatrix();
	auto projection = camera.GetPerspectiveMatrix();

	mConstantData.world = Math::Transpose(world);
	mConstantData.wvp = Math::Transpose(world * view * projection);
	mConstantData.viewPosition = camera.GetPosition();
	mConstantBuffer.Update(mConstantData);
	mConstantBuffer.BindVS();
	mConstantBuffer.BindPS();

	mSampler.BindPS();
	mGrassTexture.BindPS(0);

	mTerrainVertexShader.Bind();
	mTerrainPixelShader.Bind();

	mMeshBuffer.Draw();

}

void MAGE::Terrain::DrawEditorUI()
{
}

void MAGE::Terrain::GenerateVertices()
{
	mMesh.mVertices.clear();
	mMesh.mVertices.reserve(mNumRows * mNumCols);

	//More Cache Friendly 
	for (uint32_t z = 0; z < mNumRows; ++z)
	{
		for (uint32_t x = 0; x < mNumCols; ++x)
		{
			auto& vertex = mMesh.mVertices.emplace_back();
			vertex.position ={x * mCellSize, 0.0f , z * mCellSize};
			vertex.normal = { 0.0f, 1.0f, 0.0f };
			vertex.tangent = { 1.0f, 0.0f, 0.0f };
			vertex.texcoord = { (float)x / mNumCellsInCol, 1.0f - ((float)z / mNumCellsInRow)};
		}
	}
}

void MAGE::Terrain::GenerateIndices()
{
	mMesh.mIndices.clear();
	mMesh.mIndices.reserve(mNumCells * 6);
	uint32_t i = 0;
	for (uint32_t z = 0; z < mNumCellsInRow; ++z)
	{
		for (uint32_t x = 0; x < mNumCellsInCol; ++x)
		{
			//(lefttop - righttop)
			// lt ------rt
			//	|  \	|
			//	|   \	|
			//	|	 \	|
			//	|	  \ |
			// lb ------rb
			// (leftbottom - rightbottom)

			uint32_t lb = x +			(z * mNumCols);
			uint32_t lt = x +			((z + 1) * mNumCols);
			uint32_t rb = (x + 1) +		(z * mNumCols);
			uint32_t rt = (x + 1) +		((z + 1) * mNumCols);

			mMesh.mIndices.push_back(lb);
			mMesh.mIndices.push_back(lt);
			mMesh.mIndices.push_back(rb);

			mMesh.mIndices.push_back(lt);
			mMesh.mIndices.push_back(rt);
			mMesh.mIndices.push_back(rb);
		}
	}
}
