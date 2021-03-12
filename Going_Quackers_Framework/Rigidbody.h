#include "Collision.h"
#include "Code/EngineBase/Game Systems/Time.h"
#include "Code/EngineBase/Game Systems/Components/Component.h"

class Rigidbody : public Collision
{
public:
	void Update() override;

	void AddForce(Vector2 force) { m_forces.push_back(force); }
	void PhysicsCollide();

	void SetMass(float mass) { m_mass = mass; }
	float GetMass() { return m_mass; }

private:
	Vector2 m_velocity;
	Vector2 m_acceleration;
	
	float m_mass;

	std::vector<Vector2> m_forces;

	Collision* mp_collision;

	void CalculateVelocity();
};

