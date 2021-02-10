#ifndef _VECTORS_H
#define _VECTORS_H

#include <cmath>

#include "../ImGui/ImGui SourceCode/imgui.h"
#include "../ImGui/ImGui SourceCode/imgui_impl_win32.h"
#include "../ImGui/ImGui SourceCode/imgui_impl_dx11.h"

struct Vector2
{
	float X;
	float Y;

	Vector2() : X(0.0f), Y(0.0f) {}
	Vector2(float x, float y) : X(x), Y(y) {}

	//Utils Functions
	void Set(float x, float y) { this->X = x; this->Y = y; }
	float Length() const { return sqrtf(X * X + Y * Y); }
	float Distance(const Vector2& vec) const { return sqrtf((vec.X - X) * (vec.X - X) + (vec.Y - Y) * (vec.Y - Y)); }
	Vector2& Normalize()
	{
		float xxyy = X * X + Y * Y;

		float invLength = 1.0f / sqrtf(xxyy);
		X *= invLength;
		Y *= invLength;
		return *this;
	}
	float Dot(const Vector2& vec) const { return (X * vec.X + Y * vec.Y); }

	Vector2 operator-() const { return Vector2(-X, -Y); }
	Vector2 operator+(const Vector2& rhs) const { return Vector2(X + rhs.X, Y + rhs.Y); }
	Vector2 operator-(const Vector2& rhs) const { return Vector2(X - rhs.X, Y - rhs.Y); }
	Vector2& operator+=(const Vector2& rhs) { X += rhs.X; Y += rhs.Y; return *this; }
	Vector2& operator-=(const Vector2& rhs) { X -= rhs.X; Y -= rhs.Y; return *this; }
	Vector2 operator*(const float scale) const { return Vector2(X * scale, Y * scale); }
	Vector2 operator*(const Vector2& rhs) const { return Vector2(X * rhs.X, Y * rhs.Y); }
	Vector2& operator*=(const float scale) { X *= scale; Y *= scale; return *this; }
	Vector2& operator*=(const Vector2& rhs) { X *= rhs.X; Y *= rhs.Y; return *this; }
	Vector2 operator/(const float scale) const { return Vector2(X / scale, Y / scale); }
	Vector2& operator/=(const float scale) { X /= scale; Y /= scale; return *this; }
	bool operator==(const Vector2& rhs) const { return (X == rhs.X) && (Y == rhs.Y); }
	bool operator!=(const Vector2& rhs) const { return (X != rhs.X) && (Y != rhs.Y); }

	DirectX::XMFLOAT2 ConvertToXMFLOAT2() { return DirectX::XMFLOAT2(X, Y); }
	ImVec2 ConvertToImGuiVector2() { return ImVec2(X, Y); }
};

#endif