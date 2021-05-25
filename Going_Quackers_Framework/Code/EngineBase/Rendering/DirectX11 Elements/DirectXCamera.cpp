#include "DirectXCamera.h"
#include "../../Game Systems/Components/VirtualCamera.h"


DirectXCamera::DirectXCamera()
{
	m_position = Vector3();
	m_rotation = Vector3();
	CurrentVirtualCamera = nullptr;
}

DirectXCamera::~DirectXCamera()
{
}

void DirectXCamera::SetPosition(Vector3 pos)
{
	m_position = pos;
}

void DirectXCamera::SetRotation(Vector3 rotation)
{
	m_rotation = rotation;
}

Vector3 DirectXCamera::GetPosition()
{
	return m_position;
}

Vector3 DirectXCamera::GetRotation()
{
	return m_position;
}

void DirectXCamera::Update()
{
	if (CurrentVirtualCamera != nullptr)
	{
		m_position = CurrentVirtualCamera->GetPosition();
		m_rotation = CurrentVirtualCamera->GetRotation();
	}
	else
	{
		m_position = Vector3(0.0f,0.0f,-5.0f);
		m_rotation = Vector3();
	}
}

void DirectXCamera::Render()
{
	DirectX::XMFLOAT3 temp;
	Vector3 up, position, lookAt;
	DirectX::XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	DirectX::XMMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up = Vector3(0.0f, 1.0f, 0.0f);

	// Load it into a XMVECTOR structure.
	temp = up.ConvertToXMFLOAT3();
	upVector = DirectX::XMLoadFloat3(&temp);

	// Setup the position of the camera in the world.
	position = m_position;

	// Load it into a XMVECTOR structure.
	temp = position.ConvertToXMFLOAT3();
	positionVector = DirectX::XMLoadFloat3(&temp);

	// Setup where the camera is looking by default.
	lookAt = Vector3(0.0f, 0.0f, 1.0f);

	// Load it into a XMVECTOR structure.
	temp = lookAt.ConvertToXMFLOAT3();
	lookAtVector = DirectX::XMLoadFloat3(&temp);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotation.X * 0.0174532925f;
	yaw = m_rotation.Y * 0.0174532925f;
	roll = m_rotation.Z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = DirectX::XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = DirectX::XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

}

void DirectXCamera::GetViewMatrix(DirectX::XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}

void DirectXCamera::SetNewVirtualCamera(VirtualCamera* newCam)
{
	if (CurrentVirtualCamera != nullptr)
	{
		CurrentVirtualCamera->CamDeselected();
	}

	CurrentVirtualCamera = newCam;
	CurrentVirtualCamera->CamSelected();
}

VirtualCamera* DirectXCamera::GetVirtualCamera()
{
	return CurrentVirtualCamera;
}
