#include "DirectXCamera.h"
#include "../../Game Systems/Components/VirtualCamera.h"


DirectXCamera::DirectXCamera()
{
	m_position = Vector3();
	m_rotation = Vector3();
	mp_currentVirtualCamera = nullptr;
}

DirectXCamera::~DirectXCamera()
{
}

void DirectXCamera::SetPosition(Vector3 a_pos)
{
	m_position = a_pos;
}

void DirectXCamera::SetRotation(Vector3 a_rotation)
{
	m_rotation = a_rotation;
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
	if (mp_currentVirtualCamera != nullptr)
	{
		m_position = mp_currentVirtualCamera->GetPosition();
		m_rotation = mp_currentVirtualCamera->GetRotation();
	}
	else
	{
		m_position = Vector3(0.0f,0.0f,-5.0f);
		m_rotation = Vector3();
	}
}

void DirectXCamera::Render()
{
	DirectX::XMFLOAT3 l_temp;
	Vector3 l_up, l_position, l_lookAt;
	DirectX::XMVECTOR l_upVector, l_positionVector, l_lookAtVector;
	float lf_yaw, lf_pitch, lf_roll;
	DirectX::XMMATRIX l_rotationMatrix;

	// Setup the vector that points upwards.
	l_up = Vector3(0.0f, 1.0f, 0.0f);

	// Load it into a XMVECTOR structure.
	l_temp = l_up.ConvertToXMFLOAT3();
	l_upVector = DirectX::XMLoadFloat3(&l_temp);

	// Setup the l_position of the camera in the world.
	l_position = m_position;

	// Load it into a XMVECTOR structure.
	l_temp = l_position.ConvertToXMFLOAT3();
	l_positionVector = DirectX::XMLoadFloat3(&l_temp);

	// Setup where the camera is looking by default.
	l_lookAt = Vector3(0.0f, 0.0f, 1.0f);

	// Load it into a XMVECTOR structure.
	l_temp = l_lookAt.ConvertToXMFLOAT3();
	l_lookAtVector = DirectX::XMLoadFloat3(&l_temp);

	// Set the lf_yaw (Y axis), lf_pitch (X axis), and lf_roll (Z axis) rotations in radians.
	lf_pitch = m_rotation.X * 0.0174532925f;
	lf_yaw = m_rotation.Y * 0.0174532925f;
	lf_roll = m_rotation.Z * 0.0174532925f;

	// Create the a_rotation matrix from the lf_yaw, lf_pitch, and lf_roll values.
	l_rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(lf_pitch, lf_yaw, lf_roll);

	// Transform the l_lookAt and l_up vector by the a_rotation matrix so the view is correctly rotated at the origin.
	l_lookAtVector = XMVector3TransformCoord(l_lookAtVector, l_rotationMatrix);
	l_upVector = XMVector3TransformCoord(l_upVector, l_rotationMatrix);

	// Translate the rotated camera l_position to the location of the viewer.
	l_lookAtVector = DirectX::XMVectorAdd(l_positionVector, l_lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = DirectX::XMMatrixLookAtLH(l_positionVector, l_lookAtVector, l_upVector);

}

void DirectXCamera::GetViewMatrix(DirectX::XMMATRIX& ar_viewMatrix)
{
	ar_viewMatrix = m_viewMatrix;
}

void DirectXCamera::SetNewVirtualCamera(VirtualCamera* ap_newCam)
{
	if (mp_currentVirtualCamera != nullptr)
	{
		mp_currentVirtualCamera->CamDeselected();
	}

	mp_currentVirtualCamera = ap_newCam;
	mp_currentVirtualCamera->CamSelected();
}

VirtualCamera* DirectXCamera::GetVirtualCamera()
{
	return mp_currentVirtualCamera;
}
