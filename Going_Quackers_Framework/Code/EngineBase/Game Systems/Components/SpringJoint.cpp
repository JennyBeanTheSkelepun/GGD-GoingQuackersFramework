#include "SpringJoint.h"
#include "../Debug.h"
#include "Physics/Rigidbody.h"
#include <algorithm>

SpringJoint::SpringJoint(GameObject* ap_owner) : Component(ap_owner, ComponentTypes::SPRINGJOINT, "Spring Joint") {
	mf_defaultDesiredLength = 1;
	mf_desiredLength = 1;
	mf_strength = 1;
}

SpringJoint::~SpringJoint() {
}

void SpringJoint::OnDestroy() {
	this->~SpringJoint();
}

void SpringJoint::SetConnectedObject(GameObject* ap_connectedObject) {
	if (ap_connectedObject != nullptr && ap_connectedObject->GetComponent<Rigidbody>() == nullptr) {
		Debug::getInstance()->LogWarning("Cannot add \"" + ap_connectedObject->GetName() + "\" to " + GetOwner()->GetName() + " as a spring joint without a RigidBody.");
		return;
	}

	mp_connectedObject = ap_connectedObject;

	if (mp_connectedObject != nullptr) {
		mp_connectedObject->GetName().copy(m_jointObjectNameField, mp_connectedObject->GetName().size());
	}
	else
		std::fill(std::begin(m_jointObjectNameField), std::end(m_jointObjectNameField), '\0');
}

void SpringJoint::SetStrength(float af_strength) {
	mf_strength = af_strength;
	mi_typeField = (int)m_type;
}

void SpringJoint::SetDefaultDesiredLength(float af_desiredLength) {
	mf_defaultDesiredLength = af_desiredLength;
}

void SpringJoint::SetCurrentDesiredLength(float af_desiredLength) {
	mf_desiredLength = af_desiredLength;
}

void SpringJoint::SetSelfAdjustDesiredLength(bool ab_selfAdjusting) {
	mb_selfAdjustDesiredLength = ab_selfAdjusting;
}

void SpringJoint::SetSpringType(SpringType a_type) {
	m_type = a_type;
}

void SpringJoint::SetSpringMode(SpringMode a_mode) {
	m_mode = a_mode;
}

void SpringJoint::Update() {
	if (mb_checkForConnectedObject) {
		mp_connectedObject = ms_connectedObjectID == "NULL" ? nullptr : SceneManager::GetInstance()->GetCurrentScene()->GetObjectByID(ms_connectedObjectID);

		if (mp_connectedObject != nullptr)
			mp_connectedObject->GetName().copy(m_jointObjectNameField, mp_connectedObject->GetName().size());

		mb_checkForConnectedObject = false;
	}

	if (!EngineGuiClass::getInstance()->IsInPlayMode()) {
		mf_desiredLength = mf_defaultDesiredLength;
		return;
	}

	if (mp_connectedObject == nullptr)
		return;

	Vector2 tailposition = mp_connectedObject->GetTransform()->GetPosition();
	Vector2 tailOffset = mp_connectedObject->GetTransform()->GetPosition() - GetOwner()->GetTransform()->GetPosition();
	Rigidbody* tailObjectRb = mp_connectedObject->GetComponent<Rigidbody>();
	float currentLength = tailOffset.Length();
	
	if (m_type == SpringType::UNFIXED_HEAD && currentLength != mf_defaultDesiredLength) {
		ApplyNonFixedHeadSpringForce(currentLength);
		return;
	}

	if (m_mode == SpringMode::NEITHER && currentLength != mf_desiredLength) {
		SnapTailToDesiredDistance();
		return;
	}

	float nextTailPositionLength = (tailOffset + tailObjectRb->GetVelocity()).Length();

	if (mb_selfAdjustDesiredLength) {
		if (nextTailPositionLength < currentLength && m_mode == SpringMode::REPEL_ONLY ||
			nextTailPositionLength > currentLength && m_mode == SpringMode::ATTRACT_ONLY) {
			mf_desiredLength = currentLength - MovementFromHeadBetweenTwoPositions(tailposition, tailposition - tailObjectRb->GetVelocity()).Length();
			SnapTailToDesiredDistance();
			return;
		}
	}

	if (nextTailPositionLength < mf_desiredLength) {
		if (m_mode == SpringMode::REPEL_ONLY) {
			SnapTailToDesiredDistance();
		}
		else
			ApplyFixedHeadSpringForce(currentLength);
	}
	else if (nextTailPositionLength > mf_desiredLength) {
		if (m_mode == SpringMode::ATTRACT_ONLY) {
			SnapTailToDesiredDistance();
		}
		else
			ApplyFixedHeadSpringForce(currentLength);
	}
}

void SpringJoint::ImGUIUpdate() {
	ImGui::InputText("Enter joint object name", m_jointObjectNameField, IM_ARRAYSIZE(m_jointObjectNameField));
	ImGui::Spacing();
	ImGui::InputFloat("Enter desired length", &mf_defaultDesiredLength);
	ImGui::InputFloat("Enter strength", &mf_strength, 0.1f);
	ImGui::Spacing();
	ImGui::Combo("Joint type", &mi_typeField, "Fixed\0Non fixed\0\0");

	if (m_type == SpringType::FIXED_HEAD)
		ImGui::Combo("Mode type", &mi_modeField, "Attract & Repel\0Attract\0Repel\0Neither\0\0");
	
	ImGui::Spacing();
	ImGui::Checkbox("Self adjust desired distance", &mb_selfAdjustDesiredLength);
	ImGui::Spacing();

	if (ImGui::Button("Update joint")) {
		m_type = static_cast<SpringType>(mi_typeField);

		if (m_type == SpringType::UNFIXED_HEAD && GetOwner()->GetComponent<Rigidbody>() == nullptr) {
			Debug::getInstance()->LogWarning("Added the Rigidbody required for a non fixed head type");
			GetOwner()->AddComponent<Rigidbody>();
		}

		m_mode = static_cast<SpringMode>(mi_modeField);

		SetCurrentDesiredLength(mf_desiredLength);

		//Connected joint
		std::string objectNameField(m_jointObjectNameField);

		if (objectNameField == GetOwner()->GetName()) {
			Debug::getInstance()->LogWarning("Cannot join \"" + objectNameField + "\" to itself");
			return;
		}

		std::vector<GameObject*> sceneObjects = SceneManager::GetInstance()->GetCurrentScene()->GetSceneObjects();

		for (size_t i = 0; i < sceneObjects.size(); i++) {
			//TODO - Remove string cleaning section when name GetName bug is fixed
			std::string objectName = sceneObjects[i]->GetName();
			int garbageStart = objectName.find('\0');

			if (garbageStart != std::string::npos)
				objectName.erase(objectName.find('\0'));
			//End of string cleaning

			if (objectName == objectNameField) {
				if (mp_connectedObject != sceneObjects[i])
					SetConnectedObject(sceneObjects[i]);

				return;
			}
		}

		Debug::getInstance()->LogWarning("No match for \"" + objectNameField + "\" found in this scene");
		SetConnectedObject(nullptr);
	}
}

void SpringJoint::SceneLoad(json* componentJSON) {
	ms_connectedObjectID = (*componentJSON)["ConnectedObjectID"];
	mb_checkForConnectedObject = true;

	mf_defaultDesiredLength = (*componentJSON)["DefaultDesiredLength"];
	mf_desiredLength = mf_defaultDesiredLength;

	mf_strength = (*componentJSON)["Strength"];

	m_type = (*componentJSON)["Type"];
	mi_typeField = (int)m_type;

	m_mode = (*componentJSON)["Mode"];
	mi_modeField = (int)m_mode;

	mb_selfAdjustDesiredLength = (*componentJSON)["SelfAdjustDesiredLength"];
}

json* SpringJoint::SceneSave() {
	std::string objectID = mp_connectedObject == nullptr ? "NULL" : mp_connectedObject->GetID();

	json* returnObj = new json({
		{"ConnectedObjectID", objectID},
		{"DefaultDesiredLength", mf_defaultDesiredLength},
		{"Strength", mf_strength},
		{"Type", m_type},
		{"Mode", m_mode},
		{"SelfAdjustDesiredLength", mb_selfAdjustDesiredLength}
		});

	return returnObj;
}

Vector2 SpringJoint::MovementFromHeadBetweenTwoPositions(Vector2 a_oldPosition, Vector2 a_newPosition) {
	Vector2 headPosition = GetOwner()->GetTransform()->GetPosition();

	Vector2 newPositionOffset = a_newPosition - headPosition;
	//Because .Normalize will also modify newPositionOffset
	Vector2 newPositionOffsetNormalized = (a_newPosition - headPosition).Normalize();

	float currentLength = (a_oldPosition - headPosition).Length();

	return newPositionOffset - newPositionOffsetNormalized * currentLength;
}

void SpringJoint::SnapTailToDesiredDistance() {
	Vector2 tailPosition = mp_connectedObject->GetTransform()->GetPosition();
	Rigidbody* tailObjectRb = mp_connectedObject->GetComponent<Rigidbody>();

	Vector2 tailOffset = tailPosition - GetOwner()->GetTransform()->GetPosition();
	tailOffset = tailOffset.Normalize() * mf_desiredLength;

	mp_connectedObject->GetTransform()->SetPosition(tailOffset);

	Vector2 nextTailOffset = tailOffset + tailObjectRb->GetVelocity();
	//Because .Normalize will also modify nextTailOffset
	Vector2 nextTailOffsetNormalised = (tailOffset + tailObjectRb->GetVelocity()).Normalize();

	tailObjectRb->AddForce(Force{ nextTailOffsetNormalised * mf_desiredLength - nextTailOffset, MovementIgnore::MASSACCEL });
}

void SpringJoint::ApplyFixedHeadSpringForce(float af_currentStretch) {
	Vector2 headPosition = GetOwner()->GetTransform()->GetPosition();
	Vector2 tailPosition = mp_connectedObject->GetTransform()->GetPosition();

	float distanceFromDesired = (mf_desiredLength - af_currentStretch);
	Vector2 directionFromHead = (tailPosition - headPosition).Normalize();

	mp_connectedObject->GetComponent<Rigidbody>()->AddForce(Force{ directionFromHead * (mf_strength * distanceFromDesired), MovementIgnore::MASSACCEL });
}

void SpringJoint::ApplyNonFixedHeadSpringForce(float af_currentStretch) {
	Vector2 headPosition = GetOwner()->GetTransform()->GetPosition();
	Vector2 tailPosition = mp_connectedObject->GetTransform()->GetPosition();

	Rigidbody* headObjectRb = GetOwner()->GetComponent<Rigidbody>();
	Rigidbody* tailObjectRb = mp_connectedObject->GetComponent<Rigidbody>();

	float distanceFromDesired = (mf_desiredLength - af_currentStretch);
	Vector2 directionFromHead = (tailPosition - headPosition).Normalize();
	Vector2 force = directionFromHead * (mf_strength * distanceFromDesired);

	float totalMass = headObjectRb->GetMass() + tailObjectRb->GetMass();

	headObjectRb->AddForce(Force{ -force * (tailObjectRb->GetMass() / totalMass), MovementIgnore::MASSACCEL });
	tailObjectRb->AddForce(Force{ force * (headObjectRb->GetMass() / totalMass), MovementIgnore::MASSACCEL });
}