#include "SpringJoint.h"
#include "../GameObject.h"
#include "Physics/Rigidbody.h"

SpringJoint::SpringJoint(GameObject* ap_owner, GameObject* ap_connectedObject) : Component(ap_owner, ComponentTypes::SPRINGJOINT) {
	mp_connectedObject = ap_connectedObject;
	GetOwner()->AddChild(mp_connectedObject);

	//TODO - check the child has a valid rigidbody before adding it
}

SpringJoint::~SpringJoint() {
	mp_connectedObject->SetParent(nullptr);
}

void SpringJoint::SetConnectedObject(GameObject* ap_connectedObject) {
	if (mp_connectedObject != nullptr)
		mp_connectedObject->SetParent(nullptr);

	mp_connectedObject = ap_connectedObject;
	GetOwner()->AddChild(mp_connectedObject);

	//TODO - check the child has a valid rigidbody before adding it
}

void SpringJoint::SetStrength(float af_strength) {
	mf_strength = af_strength;
}

void SpringJoint::SetDesiredLength(float af_desiredLength) {
	mf_desiredLength = af_desiredLength;
}

void SpringJoint::SetSpringType(SpringType a_type) {
	m_type = a_type;
}

void SpringJoint::SetSpringMode(SpringMode a_mode) {
	m_mode = a_mode;
}

void SpringJoint::Update() {
	float currentLength = GetOwner()->GetTransform()->GetPosition().Distance(mp_connectedObject->GetTransform()->GetPosition());

	if (currentLength < mf_desiredLength && m_mode != SpringMode::REPEL_ONLY ||
		currentLength > mf_desiredLength && m_mode != SpringMode::ATTRACT_ONLY) {
		if (m_type == SpringType::FIXED_HEAD) {
			ApplyFixedHeadSpringForce(currentLength);
		}
		else
			ApplyNonFixedHeadSpringForce(currentLength);
	}
}

void SpringJoint::ApplyFixedHeadSpringForce(float af_currentStretch) {
	Vector2 position = GetOwner()->GetTransform()->GetPosition();
	Vector2 direction = (position - mp_connectedObject->GetTransform()->GetPosition()).Normalize(); //TODO - check this is the right way around

	float force = mf_strength * af_currentStretch;

	mp_connectedObject->GetComponent<Rigidbody>()->AddForce(direction * force);
}

void SpringJoint::ApplyNonFixedHeadSpringForce(float af_currentStretch) {
}
