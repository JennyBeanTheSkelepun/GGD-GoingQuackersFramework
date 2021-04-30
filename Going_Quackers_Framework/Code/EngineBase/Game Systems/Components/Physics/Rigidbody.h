#ifndef _RIGIDBODY_
#define _RIGIDBODY_

#include "Collision.h"
#include "GravityEmitter.h"
#include "Trigger.h"
#include "../Component.h"
#include "../../Time.h"

enum class PhysicsTypes
{
	Trig, //Trigger
	RB, //Rigidbody
	GE //Gravity Emitter
};

class Rigidbody : public Component
{
public:
	Rigidbody(GameObject* owner);
	~Rigidbody();

	void Update() override;

	void AddForce(Vector2 force) { m_forces.push_back(force); }
	void PhysicsCollide();

	void SetMass(float mass) { m_mass = mass <= 0 ? 1.0f : mass; }
	float GetMass() { return m_mass; }

	void setType(PhysicsTypes type) { m_physicsType = type; }
	PhysicsTypes getType() { return m_physicsType; }

	Collision* getCollider() { return m_collider; }

	void RigidbodyCollide(std::vector<GameObject*>* collidingObjects);

private:
	PhysicsTypes m_physicsType = PhysicsTypes::RB;

	Vector2 m_velocity;
	Vector2 m_acceleration;
	
	float m_mass = 1.0f;

	std::vector<Vector2> m_forces;

	Collision* m_collider;
	Trigger* m_trigger;
	GravityEmitter* m_gravEmitter;

	void CalculateVelocity();
};

#endif _RIGIDBODY_