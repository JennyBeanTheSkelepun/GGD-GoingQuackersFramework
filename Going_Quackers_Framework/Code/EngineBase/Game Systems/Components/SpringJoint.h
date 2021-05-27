#ifndef _SPRINGJOINT_H_
#define _SPRINGJOINT_H_

#include "Component.h"

enum class SpringType
{
	FIXED_HEAD = 0,
	UNFIXED_HEAD = 1
};

enum class SpringMode
{
	ATTRACT_AND_REPEL = 0,
	ATTRACT_ONLY = 1,
	REPEL_ONLY = 2
};

class SpringJoint : public Component {
public:
	SpringJoint(GameObject* ap_owner);
	~SpringJoint();

	inline GameObject* GetConnectedObject() { return mp_connectedObject; }
	void SetConnectedObject(GameObject* ap_connectedObject);

	inline float GetStrength() { return mf_strength; }
	void SetStrength(float af_strength);

	inline float GetDefaultDesiredLength() { return mf_defaultDesiredLength; }
	void SetDefaultDesiredLength(float af_desiredLength);

	inline float GetCurrentDesiredLength() { return mf_desiredLength; }
	void SetCurrentDesiredLength(float af_desiredLength);

	inline bool GetSelfAdjustDesiredLength() { return mb_selfAdjustDesiredLength; }
	void SetSelfAdjustDesiredLength(bool ab_selfAdjusting);

	inline SpringType GetSpringType() { return m_type; }
	void SetSpringType(SpringType a_type);

	inline SpringMode GetSpringMode() { return m_mode; }
	void SetSpringMode(SpringMode a_mode);

	void OnDestroy() override;
	void Update() override;
	void ImGUIUpdate() override;
	json* SceneSave() override;
	void SceneLoad(json* componentJSON) override;

private:
	void ApplyForce(float af_currentStretch);
	void ApplyFixedHeadSpringForce(float af_currentStretch);
	void ApplyNonFixedHeadSpringForce(float af_currentStretch);

	GameObject* mp_connectedObject;
	std::string ms_connectedObjectID;
	bool mb_checkForConnectedObject;

	float mf_strength;

	float mf_defaultDesiredLength;
	float mf_desiredLength;
	bool mb_selfAdjustDesiredLength;

	SpringType m_type;
	SpringMode m_mode;

	char m_jointObjectNameField[128]{ "" };
	int mi_typeField;
	int mi_modeField;
};

#endif