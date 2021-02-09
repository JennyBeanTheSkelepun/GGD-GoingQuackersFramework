#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Component.h"
#include "../../../Maths/Vector3.h"

class Transform : public Component
{
public:
	Transform(GameObject* owner);
	~Transform();

	void Initialize() override;

	Vector3 GetPosition() { return m_position; }
	double GetRotation() { return m_rotation; }
	Vector3 GetScale() { return m_scale; }
private:
	Vector3 m_position;
	double m_rotation;
	Vector3 m_scale;
};

#endif