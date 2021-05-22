#ifndef _SPRINGJOINT_H_
#define _SPRINGJOINT_H_

#include "Component.h"

enum SpringType
{
	FIXED_HEAD = 0,
	UNFIXED_HEAD = 1
};

enum SpringMode
{
	ATTRACT_AND_REPEL = 0,
	ATTRACT_ONLY = 1,
	REPEL_ONLY = 2
};

class SpringJoint : public Component {
public:
	SpringJoint(GameObject* ap_owner, GameObject* ap_connectedObject);
	~SpringJoint();

	inline GameObject* GetConnectedObject() { return mp_connectedObject; }
	void SetConnectedObject(GameObject* ap_connectedObject);

	inline float GetStrength() { return mf_strength; }
	void SetStrength(float af_strength);

	inline float GetDesiredLength() { return mf_desiredLength; }
	void SetDesiredLength(float af_desiredLength);

	inline SpringType GetSpringType() { return m_type; }
	void SetSpringType(SpringType a_type);

	inline SpringMode GetSpringMode() { return m_mode; }
	void SetSpringMode(SpringMode a_mode);

	void Update() override;

private:
	void ApplyFixedHeadSpringForce(float af_currentStretch);
	void ApplyNonFixedHeadSpringForce(float af_currentStretch);

	GameObject* mp_connectedObject;

	float mf_strength;
	float mf_desiredLength;

	SpringType m_type;
	SpringMode m_mode;
};

#endif