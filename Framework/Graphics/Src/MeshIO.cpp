#include "Precompiled.h"
#include "MeshIO.h"

using namespace MAGE::Graphics;
using namespace MAGE::Math;

void MAGE::Graphics::MeshIO::Write(FILE* file, const SkinnedMesh& mesh)
{

	fprintf(file, "VertexCount: %d\n", static_cast<uint32_t>(mesh.mVertices.size()));
	for (auto& v : mesh.mVertices)
	{
		//fprintf(file, "%f %f %f %f %f %f %f %f %f %f %f\n",
		//	v.position.x, v.position.y, v.position.z,
		//	v.normal.x, v.normal.y, v.normal.z,
		//	v.tangent.x, v.tangent.y, v.tangent.z,
		//	v.texcoord.x, v.texcoord.y);
		fprintf(file, "%f %f %f %f %f %f %f %f %f %f %f %d %d %d %d %f %f %f %f\n",
			v.position.x, v.position.y, v.position.z,
			v.normal.x, v.normal.y, v.normal.z,
			v.tangent.x, v.tangent.y, v.tangent.z,
			v.texcoord.x, v.texcoord.y,
			v.boneIndicies[0], v.boneIndicies[1], v.boneIndicies[2], v.boneIndicies[3],
			v.boneWeights[0], v.boneWeights[1], v.boneWeights[2], v.boneWeights[3]);
	}

	fprintf(file, "IndexCount: %d\n", static_cast<uint32_t>(mesh.mIndices.size()));

	for (size_t i = 0; i < mesh.mIndices.size(); i += 3)
	{
		fprintf(file, "%d %d %d\n",
			mesh.mIndices[i + 0],
			mesh.mIndices[i + 1],
			mesh.mIndices[i + 2]);
	}

}

void MAGE::Graphics::MeshIO::Write(FILE* file, const Material& mat)
{
	fprintf(file, "Ambient %f %f %f %f\n", mat.ambient.x, mat.ambient.y, mat.ambient.z, mat.ambient.w);
	fprintf(file, "Diffuse %f %f %f %f\n", mat.diffuse.x, mat.diffuse.y, mat.diffuse.z, mat.diffuse.w);
	fprintf(file, "Specular %f %f %f %f\n", mat.specular.x, mat.specular.y, mat.specular.z, mat.specular.w);
	fprintf(file, "Power %f\n", mat.power);
}

void MAGE::Graphics::MeshIO::Read(FILE* file, Material& mat)
{

	fscanf_s(file, "Ambient %f %f %f %f\n", &mat.ambient.x, &mat.ambient.y, &mat.ambient.z, &mat.ambient.w);
	fscanf_s(file, "Diffuse %f %f %f %f\n", &mat.diffuse.x, &mat.diffuse.y, &mat.diffuse.z, &mat.diffuse.w);
	fscanf_s(file, "Specular %f %f %f %f\n", &mat.specular.x, &mat.specular.y, &mat.specular.z, &mat.specular.w);
	fscanf_s(file, "Power %f\n", &mat.power);
}

void MAGE::Graphics::MeshIO::Read(FILE* file, SkinnedMesh& mesh)
{
	uint32_t numVertices = 0;
	fscanf_s(file, "VertexCount: %d\n", &numVertices);
	mesh.mVertices.resize(numVertices);
	for (auto& v : mesh.mVertices)
	{
		//fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f\n",
		//	&v.position.x, &v.position.y, &v.position.z,
		//	&v.normal.x, &v.normal.y, &v.normal.z,
		//	&v.tangent.x, &v.tangent.y, &v.tangent.z,
		//	&v.texcoord.x, &v.texcoord.y);

		fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %d %d %d %d %f %f %f %f\n",
			&v.position.x, &v.position.y, &v.position.z,
			&v.normal.x, &v.normal.y, &v.normal.z,
			&v.tangent.x, &v.tangent.y, &v.tangent.z,
			&v.texcoord.x, &v.texcoord.y, 
			&v.boneIndicies[0], &v.boneIndicies[1], &v.boneIndicies[2], &v.boneIndicies[3],
			&v.boneWeights[0], &v.boneWeights[1], &v.boneWeights[2], &v.boneWeights[3]);
	}

	uint32_t numIndices = 0;
	fscanf_s(file, "IndexCount: %d\n", &numIndices);
	mesh.mIndices.resize(numIndices);
	for (size_t i = 0; i < mesh.mIndices.size(); i += 3)
	{
		uint32_t a, b, c;
		fscanf_s(file, "%d %d %d\n", &a, &b, &c);
		mesh.mIndices[i + 0] = a;
		mesh.mIndices[i + 1] = b;
		mesh.mIndices[i + 2] = c;
	}
}
