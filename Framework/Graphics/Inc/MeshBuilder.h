#pragma once
#include "Mesh.h"
namespace MAGE::Graphics
{
	class MeshBuilder
	{
	public:
		static MeshPX CreatePlanePX(int height, int width);
		static MeshPX CreateCylinderPX(float radius, int height, int width);
		static MeshPX CreateSpherePX(float radius, int rings, int slices, bool isInside);

		static MeshPN CreateCube();
		static MeshPN CreateCylinder();
		static MeshPN CreateSpherePN(float radius, int rings, int slices);

		static Mesh CreatePlane(float size, int rows = 16, int columns = 16);
		static Mesh CreateSphere(float radius, int rings, int slices);
		static Mesh ComputeNormals(Mesh &mesh);

		static MeshPX CreateNDCQuad();

		

	};

}