#include "../../Rendering/Graphics.h"
#include "VirtualCamera.h"

//- Constructors -//
VirtualCamera::VirtualCamera(GameObject* owner) : Component(owner, ComponentTypes::VIRTUALCAMERA, "Virtual Camera")
{
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
	ImGui::InputFloat3("Roation", Roatation.ToFloatArray());

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

	Roatation.X = (*componentJSON)["Rotation"]["X"];
	Roatation.Y = (*componentJSON)["Rotation"]["Y"];
	Roatation.Z = (*componentJSON)["Rotation"]["Z"];
}

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
	return Roatation;
}

void VirtualCamera::SetRotation(Vector3 a_rotation)
{
	Roatation = a_rotation;
}

json* VirtualCamera::SceneSave()
{
	json* returnObj = new json({
		{"Position", {{"X", Position.X },{"Y", Position.Y },{"Z", Position.Z}}},
		{"Rotation", {{"X", Roatation.X },{"Y", Roatation.Y },{"Z", Roatation.Z}}}
		});

	return returnObj;
}