#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

#include "Collision.h"
#include "GravityEmitter.h"
#include "Trigger.h"
#include "../Component.h"
#include "../../Time.h"
#include "../../../SceneManager/SceneManager.h"
#include "../../Debug.h"

enum class PhysicsTypes
{
	Trig = 0, //Trigger
	RB = 1, //Rigidbody
	GE = 2 //Gravity Emitter
};

enum class MovementIgnore
{
	NONE = 0,
	ACCEL = 1,
	MASS = 2,
	MASSACCEL = 3,
};

class Rigidbody : public Component
{
public:
	Rigidbody(GameObject* owner);
	~Rigidbody();

	//- Basic Loops -//
	void OnDestroy() override;
	void Update() override;

	//- ImGui UpdateLoop -//
	void ImGUIUpdate() override;

	//- Scene Save and Load -//
	json* SceneSave() override;
	void SceneLoad(json* componentJSON) override;

	/// <summary>
	/// Adds the force to the next frames Rigidbody calculation
	/// </summary>
	/// <param name="force"></param>
	void AddForce(Vector2 force) { m_forces.push_back(force); }
	void PhysicsCollide();

	void SetMass(float mass) { m_mass = mass <= 0 ? 1.0f : mass; }
	float GetMass() { return m_mass; }

	void setType(PhysicsTypes type) { m_physicsType = type; }
	PhysicsTypes getType() { return m_physicsType; }

	void RigidbodyCollide(std::vector<GameObject*>* collidingObjects);

	void SetRadius(float radius) { m_radius = radius; }
	float GetRadius() { return m_radius; }

	void SetAABBRect(float width, float height) { m_AABBRect = Vector2(width, height); }
	Vector2 GetAABBRect() { return m_AABBRect; } ///returns vec2 with x = width, y = height

	void SetCollisionType(CollisionTypes colType) { m_collisionType = colType; } /// s = Spherical, a = AABB
	CollisionTypes GetCollisionType() { return m_collisionType; }

	bool getCollideFlag() { return physicsChecked; }
	void resetCollideFlag() { physicsChecked = false; }

	void setUseAccel(MovementIgnore IgnoreFlag) { m_MoveIgnoreFlag = IgnoreFlag; }

	std::vector<GameObject*> getCollidedObjects() { return collidingObjects; }

private:
	PhysicsTypes m_physicsType = PhysicsTypes::RB;

	Vector2 m_velocity;
	Vector2 m_acceleration;
	
	float m_mass = 1.0f;

	std::vector<Vector2> m_forces;
	MovementIgnore m_MoveIgnoreFlag = MovementIgnore::NONE;

	Trigger* mp_trigger;
	GravityEmitter* mp_gravEmitter;

	bool m_isStatic = false;

	float m_radius = 0.0f;
	Vector2 m_AABBRect = Vector2();

	CollisionTypes m_collisionType = CollisionTypes::Sphere;

	void CalculateVelocity();

	bool physicsChecked = false;

	std::vector<GameObject*> collidingObjects;

	std::string m_physicsTypeDropDown[3] = {"Rigidbody", "Trigger", "Gravity Zone"};
	std::string  m_colliderShapeDropDown[2] = { "Sphere", "AABB" };

	std::string  m_DropdownPhysicsTypeSelected = "Rigidbody";
	std::string  m_DropdownColliderShapeSelected = "Sphere";
};

#endif _RIGIDBODY_H_