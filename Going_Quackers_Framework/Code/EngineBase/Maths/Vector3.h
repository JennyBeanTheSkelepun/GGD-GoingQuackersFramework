#ifndef _VECTOR_3_
#define _VECTOR_3_

#include <d3d11.h>
#include <DirectXMath.h>

#include "../ImGui/ImGui SourceCode/imgui.h"
#include "../ImGui/ImGui SourceCode/imgui_impl_win32.h"
#include "../ImGui/ImGui SourceCode/imgui_impl_dx11.h"

class Vector3
{
public:
	Vector3();
	Vector3(float x, float y);
	Vector3(float x, float y, float z);
	~Vector3();

	DirectX::XMFLOAT3 ConvertToXMFLOAT3();
	ImVec2 ConvertToImGuiVec2();
	ImVec4 ConvertToImGuiVec4();

	float Avrage();
	float Avrage(float x, float y, float z);

	float x = 0.0f, y = 0.0f, z = 0.0f;

	//- Declaring Operators -//
	inline Vector3 operator++()	{ return Vector3(x++, y++, z++); }
	inline Vector3 operator--()	{ return Vector3(x--, y--, z--); }
	
	inline friend Vector3 operator+(Vector3 lhs, const Vector3& rhs) { return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
	inline friend Vector3 operator-(Vector3 lhs, const Vector3& rhs) { return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
	inline friend Vector3 operator*(Vector3 lhs, const Vector3& rhs) { return Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z); }
	inline friend Vector3 operator/(Vector3 lhs, const Vector3& rhs) { return Vector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z); }
	
	inline bool operator==(const Vector3& rhs) { return (x == rhs.x && y == rhs.y && z == rhs.y); }
	inline bool operator!=(const Vector3& rhs) { return (x != rhs.x && y != rhs.y && z != rhs.y); }

	inline bool operator< (const Vector3& rhs) { return (this->Avrage() < Avrage(rhs.x, rhs.y, rhs.z)); }
	inline bool operator> (const Vector3& rhs) { return (this->Avrage() > Avrage(rhs.x, rhs.y, rhs.z)); }
	inline bool operator<=(const Vector3& rhs) { return (this->Avrage() <= Avrage(rhs.x, rhs.y, rhs.z)); }
	inline bool operator>=(const Vector3& rhs) { return (this->Avrage() >= Avrage(rhs.x, rhs.y, rhs.z)); }
};

#endif // !_VECTOR_3_