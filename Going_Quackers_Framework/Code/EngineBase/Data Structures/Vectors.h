#ifndef _VECTORS_H
#define _VECTORS_H

#include <cmath>
#include <iostream>
#include <DirectXMath.h>

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


	friend std::ostream& operator<<(std::ostream& os, const Vector2& vec) { os << "(" << vec.X << ", " << vec.Y << ")"; return os; }
};

struct Vector3
{
	float X;
	float Y;
	float Z;

	Vector3() : X(0.0f), Y(0.0f), Z(0.0f) {}
	Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}

	//Utils Functions
	void Set(float x, float y, float z) { this->X = x; this->Y = y; this->Z = z; }
	float Length() const { return sqrtf(X * X + Y * Y + Z * Z); }
	float Distance(const Vector3& vec) const { return sqrtf((vec.X - X) * (vec.X - X) + (vec.Y - Y) * (vec.Y - Y) + (vec.Z - Z) * (vec.Z - Z)); }
	Vector3& Normalize()
	{
		float xxyyzz = X * X + Y * Y + Z * Z;

		float invLength = 1.0f / sqrtf(xxyyzz);
		X *= invLength;
		Y *= invLength;
		Z *= invLength;
		return *this;
	}
	float Dot(const Vector3& vec) const { return (X * vec.X + Y * vec.Y + Z * vec.Z ); }

	Vector3 operator-() const { return Vector3(-X, -Y, -Z); }
	Vector3 operator+(const Vector3& rhs) const { return Vector3(X + rhs.X, Y + rhs.Y, Z + rhs.Z); }
	Vector3 operator-(const Vector3& rhs) const { return Vector3(X - rhs.X, Y - rhs.Y, Z + rhs.Z); }
	Vector3& operator+=(const Vector3& rhs) { X += rhs.X; Y += rhs.Y; Z += rhs.Z; return *this; }
	Vector3& operator-=(const Vector3& rhs) { X -= rhs.X; Y -= rhs.Y; Z -= rhs.Z; return *this; }
	Vector3 operator*(const float scale) const { return Vector3(X * scale, Y * scale, Z * scale); }
	Vector3 operator*(const Vector3& rhs) const { return Vector3(X * rhs.X, Y * rhs.Y, Z * rhs.Z); }
	Vector3& operator*=(const float scale) { X *= scale; Y *= scale; Z *= scale; return *this; }
	Vector3& operator*=(const Vector3& rhs) { X *= rhs.X; Y *= rhs.Y;  Z *= rhs.Z; return *this; }
	Vector3 operator/(const float scale) const { return Vector3(X / scale, Y / scale, Z / scale); }
	Vector3& operator/=(const float scale) { X /= scale; Y /= scale; Z /= scale; return *this; }
	bool operator==(const Vector3& rhs) const { return (X == rhs.X) && (Y == rhs.Y) && (Z == rhs.Z); }
	bool operator!=(const Vector3& rhs) const { return (X != rhs.X) && (Y != rhs.Y) && (Z == rhs.Z); }

	DirectX::XMFLOAT3 ConvertToXMFLOAT3() { return DirectX::XMFLOAT3(X, Y, Z); }

	friend std::ostream& operator<<(std::ostream& os, const Vector3& vec) { os << "(" << vec.X << ", " << vec.Y << ", " << vec.Z << ")"; return os; }
};

#endif