#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <directxmath.h>
#include "../../Data Structures/Vectors.h"

class VirtualCamera;

class DirectXCamera
{
public:
	DirectXCamera();
	~DirectXCamera();

	void SetGlobalPosition(Vector3 pos);
	void SetGlobalRotation(Vector3 pos);

	Vector3 GetGlobalPosition();
	Vector3 GetGlobalRotation();

	void Update();
	void Render();
	void GetViewMatrix(DirectX::XMMATRIX& viewMatrix);

	void SetNewVirtualCamera(VirtualCamera* newCam);
	VirtualCamera* GetVirtualCamera();

private:
	Vector3 m_position;
	Vector3 m_rotation;
	DirectX::XMMATRIX m_viewMatrix;

	VirtualCamera* mp_currentVirtualCamera;
};


#endif // !_CAMERA_H_
