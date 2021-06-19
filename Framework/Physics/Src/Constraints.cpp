#include "Precompiled.h"
#include "Constraints.h"

using namespace MAGE;
using namespace MAGE::Physics;



Fixed::Fixed(Particle* p) 
	:Fixed(p, p->position)
{

}

Fixed::Fixed(Particle* p, const Math::Vector3& position)
	:mParticle(p), mPosition(position)
{

}

void Fixed::Apply() const 
{
	mParticle->SetPosition(mPosition);
}
void Fixed::DebugDraw() const 
{
	Graphics::SimpleDraw::AddAABB(mPosition, mParticle->radius, Graphics::Colors::Cyan);
}
	 
void Fixed::SetPosition(const Math::Vector3& position)
{
	mPosition = position;
}

MAGE::Physics::Spring::Spring(Particle * a, Particle * b, float restLength)
	:mParticleA(a),mParticleB(b),mRestLength(restLength)
{
	if (restLength <= 0.0f)
		mRestLength = Math::Magnitude(a->position - b->position);
}

void MAGE::Physics::Spring::Apply() const
{
	Math::Vector3 delta = mParticleB->position - mParticleA->position;
	float dist = Math::Magnitude(delta);
	float diff = (dist - mRestLength) / (dist * (mParticleA->invMass + mParticleB->invMass));
	mParticleA->position += delta /dist * diff * mParticleA->invMass;
	mParticleB->position -= delta /dist * diff * mParticleB->invMass;
}

void MAGE::Physics::Spring::DebugDraw() const
{
	Graphics::SimpleDraw::AddLine(
		mParticleA->position, mParticleB->position, Graphics::Colors::Green);
}

//For Homework:
// -Add tetrahedrons!!
// -Cubes! (how many constraints do you need??)
// -Ball and chain 
// -Cloth
// - And if you are up for the Challenge, build a figure (may need more contraint)
// -Build whatever you want
