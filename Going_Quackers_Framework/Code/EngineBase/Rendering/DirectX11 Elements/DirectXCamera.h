#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <directxmath.h>

#include "../../Data Structures/Vectors.h"

class DirectXCamera
{
public:
	DirectXCamera();
	~DirectXCamera();

	void SetPosition(Vector3 pos);
	void SetRotation(Vector3 pos);

	Vector3 GetPosition();
	Vector3 GetRotation();

	void Update();
	void Render();
	void GetViewMatrix(DirectX::XMMATRIX& viewMatrix);

private:
	Vector3 m_position;
	Vector3 m_rotation;
	DirectX::XMMATRIX m_viewMatrix;
};


#endif // !_CAMERA_H_
