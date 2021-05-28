#ifndef _VIRTUALCAMERA_H_
#define _VIRTUALCAMERA_H_

#include "Component.h"
#include "../../Data Structures/Vectors.h"

class VirtualCamera : public Component
{
public:
	VirtualCamera(GameObject* owner);
	~VirtualCamera();

	//- Basic Loops -//
	void OnDestroy() override;
	void Update() override;

	//- ImGui UpdateLoop -//
	void ImGUIUpdate() override;

	//- Scene Save and Load -//
	json* SceneSave() override;
	void SceneLoad(json* componentJSON) override;

	void CamDeselected();
	void CamSelected();

	Vector3 GetGlobalPosition();
	void SetGlobalPosition(Vector3 a_position);

	Vector3 GetGlobalRotation();
	void SetGlobalRotation(Vector3 a_rotation);

private:
	Vector3 Position;
	Vector3 Rotation;
	bool ActiveCam;

};

#endif //_VIRTUALCAMERA_H_
