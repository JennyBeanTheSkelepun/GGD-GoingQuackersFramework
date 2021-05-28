#include "../../Rendering/Graphics.h"
#include "VirtualCamera.h"

//- Constructors -//
VirtualCamera::VirtualCamera(GameObject* owner) : Component(owner, ComponentTypes::VIRTUALCAMERA, "Virtual Camera")
{
	Position = Vector3(0, 0, -5);
	Rotation = Vector3(0, 0, 0);
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
}

void VirtualCamera::CamDeselected()
{
	ActiveCam = false;
}

void VirtualCamera::CamSelected()
{
	ActiveCam = true;
}

Vector3 VirtualCamera::GetGlobalPosition()
{
	return Position;
}

void VirtualCamera::SetGlobalPosition( Vector3 a_pos)
{
	Position = a_pos;
}

Vector3 VirtualCamera::GetGlobalRotation()
{
	return Rotation;
}

void VirtualCamera::SetGlobalRotation(Vector3 a_rotation)
{
	Rotation = a_rotation;
}

json* VirtualCamera::SceneSave()
{
	json* returnObj = new json({
		{"Position", {{"X", Position.X },{"Y", Position.Y },{"Z", Position.Z}}},
		{"Rotation", {{"X", Rotation.X },{"Y", Rotation.Y },{"Z", Rotation.Z}}}
		});

	return returnObj;
}