#include "Precompiled.h"
#include "PhysicsWorld.h"

using namespace MAGE;
using namespace MAGE::Physics;


void PhysicsWorld::Initialize(const Settings& settings)
{
	mSettings = settings;
}
void PhysicsWorld::Update(float deltaTime)
{
	mTimer += deltaTime;
	while (mTimer >= mSettings.timeStep)
	{
		mTimer -= mSettings.timeStep;
		AccumulateForces();
		Integrate();
		SatisfyConstraints();
	}
}
void PhysicsWorld::DebugDraw() const
{
	for (auto p : mParticles)
		Graphics::SimpleDraw::AddSphere(p->position, p->radius, Graphics::Colors::Cyan, 4, 4);
	for (auto c : mConstraints)
		c->DebugDraw();
	for (auto& obb : mOBBs)
		Graphics::SimpleDraw::AddOBB(obb, Graphics::Colors::Blue);
}
void PhysicsWorld::AddParticle(Particle* p)
{
	mParticles.push_back(p);
}
void MAGE::Physics::PhysicsWorld::AddStaticPlane(const Math::Plane& plane)
{
	mPlanes.push_back(plane);
}

void MAGE::Physics::PhysicsWorld::AddStaticOBB(const Math::OBB& obb)
{
	mOBBs.push_back(obb);
}

void MAGE::Physics::PhysicsWorld::AddConstraint(Constraint* c)
{
	mConstraints.push_back(c);
}
void PhysicsWorld::Clear(bool onlyDynamic)
{
	for (auto particle : mParticles)
		delete particle; 
	mParticles.clear();
	for (auto constraint : mConstraints)
		delete constraint;
	mConstraints.clear();

	if (!onlyDynamic)
	{
		mPlanes.clear();
		mOBBs.clear();
	}
}
void PhysicsWorld::AccumulateForces()
{
	for (auto p : mParticles)
		p->acceleration = mSettings.gravity;
}

//Every particle will update their position based on their acceleration
void PhysicsWorld::Integrate()
{
	const float timeStepSqr = Math::Sqr(mSettings.timeStep);
	for (auto p : mParticles)
	{
		Math::Vector3 displacement = (p->position - p->lastPosition) + (p->acceleration * timeStepSqr);
		p->lastPosition = p->position;
		p->position = p->position + displacement;
	}
}
void PhysicsWorld::SatisfyConstraints()
{
	for (int n = 0; n < mSettings.iterations; ++n)
	{
		for (auto c : mConstraints)
			c->Apply();
	}
	for (auto plane : mPlanes)
	{
		for (auto p : mParticles)
		{
			if (Math::Dot(p->position, plane.n) <= plane.d && 
				Math::Dot(p->lastPosition, plane.n) > plane.d)
			{
				auto velocity = p->position - p->lastPosition;
				auto velocityPerpendicular = plane.n * Math::Dot(velocity, plane.n);
				auto velocityParallel = velocity - velocityPerpendicular;
				auto newVelocity = (velocityParallel * (1.0f - mSettings.drag)) - (velocityPerpendicular * p->bounce);
				p->SetPosition(p->position - velocityPerpendicular);
				p->SetVelocity(newVelocity);

			}
		}
	}
	
	for (auto obb : mOBBs)
	{
		for (auto p : mParticles)
		{
			if (isContained(p->position, obb))
			{
				auto velocity = p->position - p->lastPosition;
				auto direction = Math::Normalize(velocity);

				Math::Ray ray{ p->lastPosition, direction };
				Math::Vector3 point, normal; 
				Math::GetContactPoint(ray, obb, point, normal);

				auto velocityPerpendicular = normal * Math::Dot(velocity, normal);
				auto velocityParallel = velocity - velocityPerpendicular;
				auto newVelocity = (velocityParallel * (1.0f - mSettings.drag)) - (velocityPerpendicular * p->bounce);
				p->SetPosition(p->position - velocityPerpendicular);
				p->SetVelocity(newVelocity);
			}
		}
	}

}
