#ifndef _VECTOR_3_
#define _VECTOR_3_

#include <d3d11.h>
#include <DirectXMath.h>

#include "../../ImGui/ImGui SourceCode/imgui.h"
#include "../../ImGui/ImGui SourceCode/imgui_impl_win32.h"
#include "../../ImGui/ImGui SourceCode/imgui_impl_dx11.h"

class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);
	~Vector2();

	DirectX::XMFLOAT3 ConvertToXMFLOAT3();
	ImVec2 ConvertToImGuiVec2();
	ImVec4 ConvertToImGuiVec4();

	float Avrage();
	float Avrage(float x, float y, float z);

	float x = 0.0f, y = 0.0f, z = 0.0f;

	//- Declaring Operators -//
	inline Vector2 operator++()	{ return Vector2(x++, y++); }
	inline Vector2 operator--()	{ return Vector2(x--, y--); }
	
	inline friend Vector2 operator+(Vector2 lhs, const Vector2& rhs) { return Vector2(lhs.x + rhs.x, lhs.y + rhs.y); }
	inline friend Vector2 operator-(Vector2 lhs, const Vector2& rhs) { return Vector2(lhs.x - rhs.x, lhs.y - rhs.y); }
	inline friend Vector2 operator*(Vector2 lhs, const Vector2& rhs) { return Vector2(lhs.x * rhs.x, lhs.y * rhs.y); }
	inline friend Vector2 operator/(Vector2 lhs, const Vector2& rhs) { return Vector2(lhs.x / rhs.x, lhs.y / rhs.y); }
	
	inline bool operator==(const Vector2& rhs) { return (x == rhs.x && y == rhs.y && z == rhs.y); }
	inline bool operator!=(const Vector2& rhs) { return (x != rhs.x && y != rhs.y && z != rhs.y); }

	inline bool operator< (const Vector2& rhs) { return (this->Avrage() < Avrage(rhs.x, rhs.y, rhs.z)); }
	inline bool operator> (const Vector2& rhs) { return (this->Avrage() > Avrage(rhs.x, rhs.y, rhs.z)); }
	inline bool operator<=(const Vector2& rhs) { return (this->Avrage() <= Avrage(rhs.x, rhs.y, rhs.z)); }
	inline bool operator>=(const Vector2& rhs) { return (this->Avrage() >= Avrage(rhs.x, rhs.y, rhs.z)); }
};

#endif // !_VECTOR_3_