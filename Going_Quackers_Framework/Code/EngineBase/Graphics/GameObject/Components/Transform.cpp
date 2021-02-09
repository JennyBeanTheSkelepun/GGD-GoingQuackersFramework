#include "Transform.h"

Transform::Transform(GameObject* owner) : Component(owner)
{
}

Transform::~Transform()
{
}

void Transform::Initialize()
{
	m_position = Vector3(0, 0, 0);
	m_scale = Vector3(0, 0, 0);
}
