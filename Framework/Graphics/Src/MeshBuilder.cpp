#include "Precompiled.h"
#include "MeshBuilder.h"
using namespace MAGE::Graphics;

MeshPX MeshBuilder::CreatePlanePX(int height, int width)
{
	MeshPX Plane;
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			Math::Vector3 vec = { static_cast<float>(x), static_cast<float>(y),0.0f };
			Plane.mVertices.emplace_back(VertexPX{ vec, {static_cast<float>(1.0f * y / height) , static_cast<float>(1.0f * x / width)} });
		}

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				Plane.mIndices.push_back(y * height + x);
				Plane.mIndices.push_back(y * height + x + 1);
				Plane.mIndices.push_back((y + 1) * height + x);

				Plane.mIndices.push_back((y + 1) * height + x);
				Plane.mIndices.push_back((y * height + x + 1));
				Plane.mIndices.push_back((y * height + x));
			}
		}
	}
	return Plane; 
}

MeshPX MeshBuilder::CreateCylinderPX(float radius,int height, int width )
{
	MeshPX Cylinder;
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			//Math::Vector3 vec = {radius * sin(height) * cos(width), radius * cos(radius), radius * sin(width) * sin(height) };
			//Cylinder.mVertices.emplace_back(VertexPX{ vec, static_cast<float>(y / height), static_cast<float>(x / height) });
		}
	}

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			Cylinder.mIndices.push_back(y * height + x);
			Cylinder.mIndices.push_back(y * height + x + 1);
			Cylinder.mIndices.push_back((y + 1) * height + x);
			
			Cylinder.mIndices.push_back((y + 1) * height + x);
			Cylinder.mIndices.push_back((y * height + x + 1));
			Cylinder.mIndices.push_back((y + 1) * height + x + 1);

		}
	}
	return Cylinder;
}

MeshPX MeshBuilder::CreateSpherePX(float radius, int rings, int slices, bool isInside)
{
	MeshPX mesh;

	const float thetaStep = Math::Constants::Pi / (rings - 1);
	const float phiStep = Math::Constants::TwoPi / slices;
	const float uStep = 1.0f / slices;
	const float vStep = 1.0f / (rings - 1);

	for (int i = 0; i < rings; ++i)
	{
		for (int j = 0; j <= slices; ++j)
		{
			float theta = thetaStep * i;
			float phi = phiStep * j;
			float r = sinf(theta);
			float x = cosf(phi) * r * radius;
			float z = sinf(phi) * r * radius;
			float y = -cosf(theta) * radius;

			Math::Vector3 position = { x, y, z };
			Math::Vector2 uv = { j * uStep, 1.0f - i * vStep };

			mesh.mVertices.emplace_back(VertexPX{ position, uv });
		}
	}

	uint32_t a, b, c, d;

	for (int x = 0; x < rings; ++x)
	{
		for (int y = 0; y <= slices; ++y)
		{
			a = (x % (slices + 1));
			b = ((x + 1) % (slices + 1));
			c = (y * (slices + 1));
			d = ((y + 1) * (slices + 1));

			if (!isInside)
			{
				mesh.mIndices.push_back(a + c);
				mesh.mIndices.push_back(b + c);
				mesh.mIndices.push_back(a + d);

				mesh.mIndices.push_back(b + c);
				mesh.mIndices.push_back(b + d);
				mesh.mIndices.push_back(a + d);
			}
			else
			{
				mesh.mIndices.push_back(a + d);
				mesh.mIndices.push_back(b + c);
				mesh.mIndices.push_back(a + c);

				mesh.mIndices.push_back(a + d);
				mesh.mIndices.push_back(b + d);
				mesh.mIndices.push_back(b + c);
			}
		}
	}
	return mesh;
}

MeshPN MAGE::Graphics::MeshBuilder::CreateSpherePN(float radius, int rings, int slices)
{
	MeshPN Sphere;

	//Set of the vertex go up a level 
	for (float phi = 0.0f; phi <= Math::Constants::Pi; phi += (Math::Constants::Pi) / (static_cast<float>(rings)))
	{
		//Draws the rings 
		for (float theta = 0.0f; theta <= Math::Constants::TwoPi; theta += (Math::Constants::TwoPi) / static_cast<float>(slices))
		{
			Math::Vector3 vertices = Math::Vector3
			{ radius * sinf(phi) * cosf(theta),
			radius * cosf(phi),
			radius * sinf(phi) * sinf(theta) };
			Sphere.mVertices.emplace_back(VertexPN{
				vertices, Math::Normalize(vertices) });
		}
	}
	//float slice2 = slices - 1;
	for (int y = 0; y <= rings; ++y)
	{
		for (int x = 0; x < slices; ++x)
		{
			Sphere.mIndices.push_back((y * slices + x));
			Sphere.mIndices.push_back((y + 1) * slices + x + 1);
			Sphere.mIndices.push_back(((y + 1) * slices + x));

			Sphere.mIndices.push_back(((y)* slices + x));
			Sphere.mIndices.push_back((y * slices + x + 1));
			Sphere.mIndices.push_back(((y + 1) * slices + x + 1));
		}
	}
	return Sphere;
}

Mesh MeshBuilder::CreatePlane(float size, int rows, int columns)
{
	Mesh mPlane;
	const float offsetX = size * -0.5f;
	const float offsetZ = size * -0.5f;
	const float xStep = size / static_cast<float>(columns - 1);
	const float zStep = size / static_cast<float>(rows - 1);
	const float uStep = size / static_cast<float>(columns - 1);
	const float vStep = size / static_cast<float>(rows - 1);
	for (int z = 0; z < rows; ++z)
	{
		for (int x = 0; x < columns; ++x)
		{
			float xx = xStep * x + offsetX;
			float zz = zStep * z + offsetZ;
			float y = -5.0f;
			auto vec = Math::Vector3{
				xx,
				y,
				zz
			};
			auto vectorTX = Math::Vector2{ x* uStep, z*vStep }; 
			auto vectorNorm = Math::Vector3::YAxis; 
			auto vectorTan = Math::Vector3::XAxis; 
			mPlane.mVertices.emplace_back(
				Vertex{
					vec,
					vectorNorm,
					vectorTan,
					vectorTX
				});
		}
	}
	int index = 0;
	for (int z = 0; z < rows - 1; ++z)
	{
		for (int x = 0; x < columns - 1; ++x)
		{
			mPlane.mIndices.push_back((x + 0) + ((z + 0) * columns));
			mPlane.mIndices.push_back((x + 0) + ((z + 1) * columns));
			mPlane.mIndices.push_back((x + 1) + ((z + 1) * columns));
			mPlane.mIndices.push_back((x + 0) + ((z + 0) * columns));
			mPlane.mIndices.push_back((x + 1) + ((z + 1) * columns));
			mPlane.mIndices.push_back((x + 1) + ((z + 0) * columns));
		}
	}
	return mPlane;
}

Mesh MAGE::Graphics::MeshBuilder::ComputeNormals(Mesh &mesh)
{

	return mesh;
}

Mesh MAGE::Graphics::MeshBuilder::CreateSphere(float radius, int rings, int slices)
{
	Mesh Sphere;

	//Set of the vertex go up a level 
	for (float phi = 0.0f; phi <= Math::Constants::Pi; phi += (Math::Constants::Pi) / (static_cast<float>(rings)))
	{
		//Draws the rings 
		for (float theta = 0.0f; theta <= Math::Constants::TwoPi; theta += (Math::Constants::TwoPi) / static_cast<float>(slices))
		{
			Math::Vector3 vertices = Math::Vector3
			{ radius * sinf(phi) * cosf(theta),
			radius * cosf(phi),
			radius * sinf(phi) * sinf(theta) };

			Sphere.mVertices.emplace_back(
				Vertex{
				vertices,
				Math::Normalize(vertices),
				//**Calculate tangent here**
				{-vertices.z, 0.0f, vertices.x},
			 Math::Vector2{theta / Math::Constants::TwoPi , phi / Math::Constants::Pi}
				});
		}
	}
	//float slice2 = slices - 1;
	for (int y = 0; y <= rings; ++y)
	{
		for (int x = 0; x < slices; ++x)
		{
			Sphere.mIndices.push_back((y * slices + x));
			Sphere.mIndices.push_back((y + 1) * slices + x + 1);
			Sphere.mIndices.push_back(((y + 1) * slices + x));

			Sphere.mIndices.push_back(((y)* slices + x));
			Sphere.mIndices.push_back((y * slices + x + 1));
			Sphere.mIndices.push_back(((y + 1) * slices + x + 1));
		}
	}
	return Sphere;
}

MeshPX MAGE::Graphics::MeshBuilder::CreateNDCQuad()
{
	MeshPX Quad; 

	Quad.mVertices.emplace_back(VertexPX{ {-1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } });
	Quad.mVertices.emplace_back(VertexPX{ {-1.0f, +1.0f, 0.0f }, { 0.0f, 0.0f } });
	Quad.mVertices.emplace_back(VertexPX{ {+1.0f, +1.0f, 0.0f }, { 1.0f, 0.0f } });
	Quad.mVertices.emplace_back(VertexPX{ {+1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } });

	Quad.mIndices.push_back(0);
	Quad.mIndices.push_back(1);
	Quad.mIndices.push_back(2);

	Quad.mIndices.push_back(0);
	Quad.mIndices.push_back(2);
	Quad.mIndices.push_back(3);


	return Quad;
}



