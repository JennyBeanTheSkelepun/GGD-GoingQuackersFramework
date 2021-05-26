#include "SpringJoint.h"
#include "../GameObject.h"
#include "../Input.h"
#include "../Debug.h"
#include "Physics/Rigidbody.h"

SpringJoint::SpringJoint(GameObject* ap_owner) : Component(ap_owner, ComponentTypes::SPRINGJOINTS, "Spring Joint") {
	mp_jointObjectNameField = new char[100]{ "" };

	mf_desiredLength = 1;
	mp_desiredLengthField = new char[100]{ "1" };

	mf_strength = 1;
	mp_strengthField = new char[100]{ "1" };
}

SpringJoint::~SpringJoint() {
	delete[] mp_jointObjectNameField;
	delete[] mp_desiredLengthField;
	delete[] mp_strengthField;
}

void SpringJoint::OnDestroy() {
	this->~SpringJoint();
}

void SpringJoint::SetConnectedObject(GameObject* ap_connectedObject) {
	if (ap_connectedObject != nullptr && ap_connectedObject->GetComponent<Rigidbody>() == nullptr) {
		Debug::getInstance()->LogWarning("Cannot add \"" + ap_connectedObject->GetName() + "\" to " + GetOwner()->GetName() + "as a spring joint without a RigidBody.");
		return;
	}

	mp_connectedObject = ap_connectedObject;
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
	if (!EngineGuiClass::getInstance()->IsInPlayMode())
		return;

	if (mp_connectedObject == nullptr)
		return;

	//Testing
	if (Input::getInstance()->isKeyPressedDown(KeyCode::D))
		mp_connectedObject->GetComponent<Rigidbody>()->AddForce(Vector2(0.1f, 0) / Time::GetDeltaTime());

	if (Input::getInstance()->isKeyPressedDown(KeyCode::A)) {
		Debug::getInstance()->Log("Keydown                                                        " + std::to_string(Time::GetTime()));
		mp_connectedObject->GetComponent<Rigidbody>()->setUseAccel(MovementIgnore::MASSACCEL);
		mp_connectedObject->GetComponent<Rigidbody>()->AddForce(Vector2(-0.1f, 0) / Time::GetDeltaTime());
	}

	if (Input::getInstance()->isKeyPressedDown(KeyCode::W))
		mp_connectedObject->GetComponent<Rigidbody>()->AddForce(Vector2(0, 0.1f) / Time::GetDeltaTime());

	if (Input::getInstance()->isKeyPressedDown(KeyCode::S))
		mp_connectedObject->GetComponent<Rigidbody>()->AddForce(Vector2(0, -0.1f) / Time::GetDeltaTime());
	//End of testing

	float currentLength = GetOwner()->GetTransform()->GetPosition().Distance(mp_connectedObject->GetTransform()->GetPosition());
	Debug::getInstance()->Log("current length " + std::to_string(currentLength) + "                                            " + std::to_string(Time::GetTime()));

	if (currentLength < mf_desiredLength && m_mode != SpringMode::REPEL_ONLY ||
		currentLength > mf_desiredLength && m_mode != SpringMode::ATTRACT_ONLY) {
		if (m_type == SpringType::FIXED_HEAD) {
			Debug::getInstance()->Log("Applying force                                                        " + std::to_string(Time::GetTime()));
			ApplyFixedHeadSpringForce(currentLength);
		}
		else
			ApplyNonFixedHeadSpringForce(currentLength);
	}
}

void SpringJoint::ImGUIUpdate() {
	ImGui::InputText(((std::string)"Enter joint object name").c_str(), mp_jointObjectNameField, 128);
	ImGui::InputText(((std::string)"Enter desired length").c_str(), mp_desiredLengthField, 128);
	ImGui::InputText(((std::string)"Enter strength").c_str(), mp_strengthField, 128);

	ImGui::Combo("Joint type", &mi_typeField, "Fixed\0Non fixed\0");
	ImGui::Combo("Mode type", &mi_modeField, "Attract & Repel\0Attract\0Repel\0");

	if (ImGui::Button("Update joint")) {
		m_type = static_cast<SpringType>(mi_typeField);
		m_mode = static_cast<SpringMode>(mi_modeField);

		//Strength and length
		SetDesiredLength(std::stof(mp_desiredLengthField));
		SetStrength(std::stof(mp_strengthField));

		//Connected joint
		std::string objectNameField(mp_jointObjectNameField);

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
	//TODO - Make it load the connected Object properly
	//mp_connectedObject = (*componentJSON)["ConnectedObject"];
	/*mf_desiredLength = (*componentJSON)["DesierdLength"];
	mf_strength = (*componentJSON)["Strength"];
	m_type = (*componentJSON)["Type"];
	m_mode = (*componentJSON)["Mode"];*/
}

json* SpringJoint::SceneSave() {
	//TODO - Make it save the connected Object properly
	json* returnObj = new json({
		//{"ConnectedObject", mp_connectedObject},
		/*{"DesierdLength", mf_desiredLength},
		{"Strength", mf_strength},
		{"Type", m_type},
		{"Mode", m_mode}*/
		});

	return returnObj;
}

void SpringJoint::ApplyFixedHeadSpringForce(float af_currentStretch) {
	Vector2 headPosition = GetOwner()->GetTransform()->GetPosition();
	Vector2 tailPosition = mp_connectedObject->GetTransform()->GetPosition();
	float distanceFromDesired = (mf_desiredLength - af_currentStretch);
	//Debug::getInstance()->Log("distance from desired (" + std::to_string(distanceFromDesired) + ")                               " + std::to_string(Time::GetTime()));


	Vector2 directionFromHead = (tailPosition - headPosition).Normalize();

	Vector2 directiontoDesiredDistance = ((headPosition + directionFromHead * mf_desiredLength) - tailPosition).Normalize();
	//Debug::getInstance()->Log("test " + std::to_string((headPosition + directionFromHead * distanceFromDesired).X) + "                               " + std::to_string(Time::GetTime()));
	//Debug::getInstance()->Log("direction to desired (" + std::to_string(directiontoDesiredDistance.X) + ", " + std::to_string(directiontoDesiredDistance.Y) + ")                               " + std::to_string(Time::GetTime()));

	float force = (mf_strength * distanceFromDesired) / Time::GetDeltaTime();
	//Debug::getInstance()->Log("force " + std::to_string(force) + "                                                    " + std::to_string(Time::GetTime()) + "\n\n");
	Debug::getInstance()->Log("Final force (" + std::to_string((directiontoDesiredDistance * force).X) + ", " + std::to_string((directiontoDesiredDistance * force).Y) + ")                               " + std::to_string(Time::GetTime()));

	mp_connectedObject->GetComponent<Rigidbody>()->setUseAccel(MovementIgnore::MASSACCEL);
	mp_connectedObject->GetComponent<Rigidbody>()->AddForce(directiontoDesiredDistance * force);
}

void SpringJoint::ApplyNonFixedHeadSpringForce(float af_currentStretch) {
}