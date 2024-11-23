#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Vector3 : public XMFLOAT3
{
	Vector3() = default;
	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	//コンストラクタ
	explicit Vector3(const XMVECTOR& other) :XMFLOAT3()
	{
		const XMVECTOR temp = other;
		XMStoreFloat3(this, temp);
	}

	static const XMVECTOR RIGHT;
	static const XMVECTOR UP;
	static const XMVECTOR FORWARD;

	inline bool operator == (const Vector3& r) const { return x == r.x && y == r.y && z == r.z; }
	inline bool operator != (const Vector3& r) const { return x != r.x || y != r.y || z != r.z; }
	inline XMVECTOR operator *(const float r) const { return Vector3(x * r, y * r, z * r); }
	inline XMVECTOR operator /(const float r) const { return Vector3(x / r, y / r, z / r); }

	// ベクトルの内積
	float VDot(const Vector3 in) const { return x * in.x + y * in.y + z * in.z; }
	// ベクトルの外積
	Vector3 VCross(const Vector3 in) const { return { y * in.z - z * in.y, z * in.x - x * in.z, x * in.y - y * in.x }; }
	// ベクトルのスケーリング
	Vector3 VScale(const float scale) const { return  { x * scale, y * scale, z * scale }; }

	//代入
	Vector3& operator=(const XMVECTOR& other)
	{
		const XMVECTOR temp = other;
		XMStoreFloat3(this, temp);
		return *this;
	}

	void operator+=(const Vector3& other)
	{
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
	}

	void operator-=(const Vector3& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
	}

	//キャスト
	operator XMVECTOR() const
	{
		return XMLoadFloat3(this);
	}

	//長さ
	float Length() const
	{
		return (static_cast<Vector3>(XMVector3Length(XMVECTOR(*this)))).x;
	}

	//正規化
	void Normalize()
	{
		*this = XMVector3Normalize(XMVECTOR(*this));
	}
};


