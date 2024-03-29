#include "../../Rendering/Graphics.h"
#include "../../SceneManager/SceneManager.h"
#include "VirtualCamera.h"

//- Constructors -//
VirtualCamera::VirtualCamera(GameObject* owner) : Component(owner, ComponentTypes::VIRTUALCAMERA, "Virtual Camera")
{
	Position = Vector3(0, 0, -5);
	Rotation = Vector3(0, 0, 0);
	ObjectToFollow = nullptr;
	ObjectToFollowID = "NONE";
	isFollowing = false;
	ActiveCam = false;
	ObjectIDInput = new char[100] { "" };
}

//- Deconstructors -//
VirtualCamera::~VirtualCamera()
{
}

void VirtualCamera::OnDestroy()
{
	this->~VirtualCamera();
}

//- Update Function -//
void VirtualCamera::Update()
{
	if (isFollowing && ObjectToFollow != nullptr) {
		Position = Vector3(
			ObjectToFollow->GetTransform()->GetPosition().X,
			ObjectToFollow->GetTransform()->GetPosition().Y,
			Position.Z);
	}
	else if (isFollowing && ObjectToFollow == nullptr && ObjectToFollowID != "NONE") {
		ObjectToFollow = SceneManager::GetInstance()->GetCurrentScene()->GetObjectByID(ObjectToFollowID);
		if (ObjectToFollow == nullptr) {
			Debug::getInstance()->LogError("Virtual Camera: Error when finding object to follow, could not find object with ID: " + ObjectToFollowID);
			isFollowing = false;
		}
	}
}

//- ImGui -//
void VirtualCamera::ImGUIUpdate()
{
	ImGui::InputFloat3("Position", Position.ToFloatArray());
	ImGui::InputFloat3("Rotation", Rotation.ToFloatArray());

	if (ImGui::Button("Make Active Camera"))
	{
		ActiveCam = true;
		Graphics::getInstance()->SetNewActiveCamera(this);
	}
	ImGui::SameLine();
	ImGui::Text((ActiveCam) ? "This is the active Camera" : "This isn't the active Camera" );


	ImGui::InputText("Target Object ID: ", ObjectIDInput, 100);
	if (ImGui::Button("Follow Object"))
	{
		std::vector<GameObject*> sceneObjects = SceneManager::GetInstance()->GetCurrentScene()->GetSceneObjects();
		std::string objectIDInputString(ObjectIDInput);

		for (size_t i = 0; i < sceneObjects.size(); i++) {
			std::string objectID = sceneObjects[i]->GetID();
			int garbageStart = objectID.find('\0');

			if (garbageStart != std::string::npos)
				objectID.erase(objectID.find('\0'));

			if (objectID == objectIDInputString) {
				if (ObjectToFollow != sceneObjects[i]) {
					ObjectToFollow = sceneObjects[i];

				}
				ObjectToFollowID = sceneObjects[i]->GetID();
				isFollowing = true;
				return;
			}
		}

	}
	ImGui::SameLine();
	if (ImGui::Button("Stop Following"))
	{
		StopFollowing();
	}
}

//- Scene Save / Load -//
void VirtualCamera::SceneLoad(json* componentJSON)
{
	Position.X = (*componentJSON)["Position"]["X"];
	Position.Y = (*componentJSON)["Position"]["Y"];
	Position.Z = (*componentJSON)["Position"]["Z"];

	Rotation.X = (*componentJSON)["Rotation"]["X"];
	Rotation.Y = (*componentJSON)["Rotation"]["Y"];
	Rotation.Z = (*componentJSON)["Rotation"]["Z"];

	ObjectToFollowID = (*componentJSON)["FollowID"];
	isFollowing = (*componentJSON)["isFollowing"];
	ActiveCam = (*componentJSON)["isActive"];

	if (ObjectToFollowID != "NONE") {
		isFollowing = true;
	}
	else {
		isFollowing = false;
	}

	if (ActiveCam) {
		Graphics::getInstance()->SetNewActiveCamera(this);
	}
}

json* VirtualCamera::SceneSave()
{
	json* returnObj = new json({
		{"Position", {{"X", Position.X },{"Y", Position.Y },{"Z", Position.Z}}},
		{"Rotation", {{"X", Rotation.X },{"Y", Rotation.Y },{"Z", Rotation.Z}}},
		});

	(*returnObj)["isActive"] = ActiveCam;
	(*returnObj)["isFollowing"] = isFollowing;

	if (isFollowing) {
		(*returnObj)["FollowID"] = ObjectToFollow->GetID();
	}
	else {
		(*returnObj)["FollowID"] = "NONE";
	}

	return returnObj;
}

//- Other Functions -//
void VirtualCamera::CamDeselected()
{
	ActiveCam = false;
}

void VirtualCamera::CamSelected()
{
	ActiveCam = true;
}

Vector3 VirtualCamera::GetPosition()
{
	return Position;
}

void VirtualCamera::SetPosition( Vector3 a_pos)
{
	Position = a_pos;
}

Vector3 VirtualCamera::GetRotation()
{
	return Rotation;
}

void VirtualCamera::SetRotation(Vector3 a_rotation)
{
	Rotation = a_rotation;
}

void VirtualCamera::FollowObject(GameObject* ap_object)
{
	ObjectToFollow = ap_object;
	ObjectToFollowID = ap_object->GetID();
	isFollowing = true;
}

void VirtualCamera::StopFollowing()
{
	ObjectToFollow = nullptr;
	ObjectToFollowID = "NONE";
	isFollowing = false;
}
