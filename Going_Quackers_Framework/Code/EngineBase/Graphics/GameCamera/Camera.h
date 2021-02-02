#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <directxmath.h>

class Camera
{
public:
	Camera();
	Camera(const Camera& other);
	~Camera();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();

	void Update();
	void Render();
	void GetViewMatrix(DirectX::XMMATRIX& viewMatrix);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	DirectX::XMMATRIX m_viewMatrix;
};


#endif // !_CAMERA_H_
