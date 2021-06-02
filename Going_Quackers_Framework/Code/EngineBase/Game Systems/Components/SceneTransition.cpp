#include "SceneTransition.h"
#include "../../SceneManager/SceneManager.h"

//- Constructors -//
SceneTransition::SceneTransition(GameObject* owner) : Component(owner, ComponentTypes::SCENETRANSITION, "Scene Transition")
{
	m_SceneIDInput = new char[100]{ "" };
	m_NextSceneID = "default";
}

//- Deconstructors -//
SceneTransition::~SceneTransition()
{
	delete[] m_SceneIDInput;
}

void SceneTransition::OnDestroy()
{
	this->~SceneTransition();
}

//- Update Function -//
void SceneTransition::Update()
{
	// when player collides, go to next level
	if (false) {
		OnTrigger();
	}
}

//- imGUI -//
void SceneTransition::ImGUIUpdate()
{
	ImGui::Text("Target Scene: ");
	ImGui::SameLine();
	ImGui::Text(m_NextSceneID.c_str());
	ImGui::Separator();

	ImGui::InputText("Target Scene ID: ", m_SceneIDInput, 100);


	if (ImGui::Button("Set Target Scene")) {

		std::string ls_id(m_SceneIDInput);

		// Check File Exists
		struct stat buffer;
		std::string path = "SceneConfig/" + ls_id + ".json";
		if (stat(path.c_str(), &buffer) != 0) {
			Debug::getInstance()->LogError("SceneConfig file for " + ls_id + " does not exist.");
		}
		else {
			m_NextSceneID = ls_id;
		}
	}

}

//- Scene Save / Load -//
json* SceneTransition::SceneSave()
{
	json* returnObj = new json({
		{"TargetSceneID", m_NextSceneID}
		});
	return returnObj;
}

void SceneTransition::SceneLoad(json* componentJSON)
{
	m_NextSceneID = (*componentJSON)["TargetSceneID"];
}

void SceneTransition::OnTrigger()
{
	SceneManager::GetInstance()->ChangeScene(m_NextSceneID, false);
}

//- Other Functions -//