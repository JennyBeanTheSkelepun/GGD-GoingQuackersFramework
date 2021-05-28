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

struct Force
{
	Vector2 force;
	MovementIgnore moveIgnore = MovementIgnore::NONE;
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
	void AddForce(Force force) { m_Forces.push_back(force); }

	void SetMass(float mass) { m_Mass = mass <= 0 ? 1.0f : mass; }
	float GetMass() { return m_Mass; }

	void SetType(PhysicsTypes type) { m_PhysicsType = type; }
	PhysicsTypes GetType() { return m_PhysicsType; }

	void RigidbodyCollide(std::vector<GameObject*>* collidingObjects);

	void SetRadius(float radius) { m_Radius = radius; }
	float GetRadius() { return m_Radius; }

	void SetAABBRect(float width, float height) { m_AABBRect = Vector2(width, height); }
	Vector2 GetAABBRect() { return m_AABBRect; } ///returns vec2 with x = width, y = height

	void SetCollisionType(CollisionTypes colType) { m_CollisionType = colType; } /// s = Spherical, a = AABB
	CollisionTypes GetCollisionType() { return m_CollisionType; }

	bool GetCollideFlag() { return m_PhysicsChecked; }
	void ResetCollideFlag() { m_PhysicsChecked = false; }

	Vector2 GetAcceleration() { return m_Acceleration; }
	Vector2 GetVelocity() { return m_Velocity; }

	std::vector<GameObject*> GetCollidedObjects() { return m_CollidingObjects; }

	bool m_IsStatic = false;

private:
	void PhysicsCollide();
	
	PhysicsTypes m_PhysicsType = PhysicsTypes::RB;

	Vector2 m_Velocity;
	Vector2 m_Acceleration;
	
	float m_Mass = 1.0f;

	std::vector<Force> m_Forces;

	Trigger* mp_Trigger;
	GravityEmitter* mp_GravEmitter;



	float m_Radius = 0.0f;
	Vector2 m_AABBRect = Vector2();

	CollisionTypes m_CollisionType = CollisionTypes::Sphere;

	void CalculateVelocity();

	bool m_PhysicsChecked = false;

	std::vector<GameObject*> m_CollidingObjects;

	std::string m_PhysicsTypeDropDown[3] = {"Rigidbody", "Trigger", "Gravity Zone"};
	std::string  m_ColliderShapeDropDown[2] = { "Sphere", "AABB" };

	std::string  m_DropdownPhysicsTypeSelected = "Rigidbody";
	std::string  m_DropdownColliderShapeSelected = "Sphere";
};

#endif _RIGIDBODY_H_