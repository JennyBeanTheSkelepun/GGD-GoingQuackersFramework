#include "SpringJoint.h"
#include "../GameObject.h"

SpringJoint::SpringJoint(GameObject* ap_owner, GameObject* ap_connectedObject) : Component(ap_owner, ComponentTypes::SPRINGJOINT) {
	mp_connectedObject = ap_connectedObject;
	GetOwner()->AddChild(mp_connectedObject);
}

SpringJoint::~SpringJoint() {
	mp_connectedObject->SetParent(nullptr);
}

void SpringJoint::SetConnectedObject(GameObject* ap_connectedObject) {
	if (mp_connectedObject != nullptr)
		mp_connectedObject->SetParent(nullptr);

	mp_connectedObject = ap_connectedObject;
	GetOwner()->AddChild(mp_connectedObject);
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
