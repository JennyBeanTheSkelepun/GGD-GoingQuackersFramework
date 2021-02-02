#include "Vector3.h"

Vector3::Vector3()
{
	//- Setting All elements to 0 -//
	x = y = z = 0;
}

Vector3::Vector3(float af_x, float af_y)
{
	//- set all elements -//
	x = af_x;
	y = af_y;
	z = 0;
}

Vector3::Vector3(float af_x, float af_y, float af_z)
{
	//- set all elements -//
	x = af_x;
	y = af_y;
	z = af_z;
}

Vector3::~Vector3()
{
}

DirectX::XMFLOAT3 Vector3::ConvertToXMFLOAT3()
{
	//- Converting to XMFLOAT -//
	return DirectX::XMFLOAT3(x, y, z);
}

ImVec2 Vector3::ConvertToImGuiVec2()
{
	//- Converting ImGuiVec2 -//
	return ImVec2(x, y);
}

ImVec4 Vector3::ConvertToImGuiVec4()
{
	//- Converting to ImVec4 -//
	return ImVec4(x, y, z, 0);
}
