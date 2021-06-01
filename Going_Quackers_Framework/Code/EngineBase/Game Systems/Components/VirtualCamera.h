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

	Vector3 GetPosition();
	void SetPosition(Vector3 a_position);

	Vector3 GetRotation();
	void SetRotation(Vector3 a_rotation);

	void FollowObject(GameObject* ap_object);
	void StopFollowing();

	inline bool GetIsFollowing() { return isFollowing; }

private:
	Vector3 Position;
	Vector3 Rotation;
	bool ActiveCam;

	char* ObjectNameInput;
	std::string ObjectToFollowID;
	GameObject* ObjectToFollow;
	bool isFollowing;
};

#endif //_VIRTUALCAMERA_H_
