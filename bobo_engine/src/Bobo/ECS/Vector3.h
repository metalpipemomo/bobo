#pragma once
class Vector3
{
	// public:

	// 	Vector3(float xVal, float yVal, float zVal)
	// 	{
	// 		x = xVal;
	// 		y = yVal;
	// 		z = zVal;
	// 	}

	// 	Vector3()
	// 	{
	// 		x = 0;
	// 		y = 0;
	// 		z = 0;
	// 	}

	// 	friend Vector3 operator+(Vector3 const& lhs, Vector3 const& rhs)
	// 	{
	// 		Vector3 temp;
	// 		temp.x = lhs.x + rhs.x;
	// 		temp.y = lhs.y + rhs.y;
	// 		temp.z = lhs.z + rhs.z;
	// 		return temp;
	// 	}

	// 	friend Vector3 operator+=(Vector3 const& lhs, Vector3 const& rhs)
	// 	{
	// 		return lhs + rhs;
	// 	}

	// 	friend Vector3 operator-(Vector3 const& lhs, Vector3 const& rhs)
	// 	{
	// 		Vector3 temp;
	// 		temp.x = lhs.x - rhs.x;
	// 		temp.y = lhs.y - rhs.y;
	// 		temp.z = lhs.z - rhs.z;
	// 		return temp;
	// 	}

	// 	friend Vector3 operator-=(Vector3 const& lhs, Vector3 const& rhs)
	// 	{
	// 		return lhs - rhs;
	// 	}

	// 	friend Vector3 operator*(Vector3 const& lhs, float a)
	// 	{
	// 		Vector3 temp;
	// 		temp.x = lhs.x * a;
	// 		temp.y = lhs.y * a;
	// 		temp.z = lhs.z * a;
	// 		return temp;
	// 	}

	// 	friend Vector3 operator*(Vector3 const& lhs, Vector3 const& rhs)
	// 	{
	// 		Vector3 temp;
	// 		temp.x = lhs.x * rhs.x;
	// 		temp.y = lhs.y * rhs.y;
	// 		temp.z = lhs.z * rhs.z;
	// 		return temp;
	// 	}

	// 	friend Vector3 operator/(Vector3 const& lhs, float a)
	// 	{
	// 		Vector3 temp;
	// 		temp.x = lhs.x / a;
	// 		temp.y = lhs.y / a;
	// 		temp.z = lhs.z / a;
	// 		return temp;
	// 	}

	// 	friend Vector3 operator/(Vector3 const& lhs, Vector3 const& rhs)
	// 	{
	// 		Vector3 temp;
	// 		temp.x = lhs.x / rhs.x;
	// 		temp.y = lhs.y / rhs.y;
	// 		temp.z = lhs.z / rhs.z;
	// 		return temp;
	// 	}

	// 	friend bool operator==(Vector3 const& lhs,Vector3 const& rhs)
	// 	{
	// 		if (lhs.x == rhs.x)
	// 			if (lhs.y == rhs.y)
	// 				if (lhs.z == rhs.z)
	// 					return true;
	// 		return false;
	// 	}

	// 	friend bool operator!=(Vector3 const& lhs, Vector3 const& rhs)
	// 	{
	// 		return !(lhs == rhs);
	// 	}

	// 	float GetX() {
	// 		return x;
	// 	}

	// 	float GetY() {
	// 		return y;
	// 	}

	// 	float GetZ() {
	// 		return z;
	// 	}
	// private:
	// 	float x;
	// 	float y;
	// 	float z;
};
