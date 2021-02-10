#include "Vector2.h"

Vector2::Vector2()
{
	//- Setting All elements to 0 -//
	x = y = z = 0;
}

Vector2::Vector2(float af_x, float af_y)
{
	//- set all elements -//
	x = af_x;
	y = af_y;
	z = 0;
}

Vector2::~Vector2()
{
}

DirectX::XMFLOAT3 Vector2::ConvertToXMFLOAT3()
{
	//- Converting to XMFLOAT -//
	return DirectX::XMFLOAT3(x, y, z);
}

ImVec2 Vector2::ConvertToImGuiVec2()
{
	//- Converting ImGuiVec2 -//
	return ImVec2(x, y);
}

ImVec4 Vector2::ConvertToImGuiVec4()
{
	//- Converting to ImVec4 -//
	return ImVec4(x, y, z, 0);
}

float Vector2::Avrage()
{
	float Mag = x + y + z;
	Mag /= 3;
	return Mag;
}

float Vector2::Avrage(float ai_x, float ai_y, float ai_z)
{
	float Mag = ai_x + ai_y + ai_z;
	Mag /= 3;
	return Mag;
}
