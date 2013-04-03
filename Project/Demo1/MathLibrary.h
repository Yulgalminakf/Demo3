#ifndef MATH_LIBRARY
#define MATH_LIBRARY
#include <math.h>

typedef float t_float;
typedef int t_int;
typedef unsigned int t_uint;

class Vector2;
class Vector3;
class Vector4;
class Matrix;

#pragma region Definitions

#define MATRIX_SIZE 4

#define M_PI			3.1415926f
#define HALF_PI			1.5707963f
#define PI_TIMES_TWO	6.2831852f

#define DEGTORAD(A)	((A * M_PI) / 180.0f)
#define RADTODEG(A)	((A * 180.0f) / M_PI)
#define MAX(a,b) ((a > b) ? a : b)
#define MIN(a,b)   ((a < b) ? a : b)
#define CLAMP(src, min, max)  (MAX(MIN(src,max),min))	// clamps the value to min-max
#define ABS(a) (((a) < 0) ? -(a) : (a))						// absolute value of t_int

#pragma endregion

#pragma region Vector2

Vector2 operator*(const float f, const Vector2 &v);

class Vector2
{
public:
	Vector2(void) : x(0.0f), y(0.0f) {}
	Vector2(t_float _x, t_float _y) : x(_x), y(_y) {}
	~Vector2(void);
	
	t_float x,y;

	void Set(t_float _x, t_float _y)			{ x = _x; y = _y; }

	static Vector2 one;
	static Vector2 zero;

	Vector2 operator+(const Vector2 &v) const;
	void operator+=(const Vector2 &v);

	Vector2 operator-(const Vector2 &v) const;
	void operator-=(const Vector2 &v);

	Vector2 operator*(const t_float &f) const;
	void operator*=(const t_float &f);

	Vector2 operator/(const t_float &f) const;
	void operator/=(const t_float &f);

	Vector2 operator-() const				{return Vector2(-x,-y); }

	t_float Dot(const Vector2 &v) const;
	void Reflect(Vector2 normal);

	void Normalize();
	Vector2 Perpendicular();
	t_float Length() const;
	t_float LengthSqr() const;
	void SetLength(t_float length);

	static bool Compare(const Vector2 &v1, const Vector2 &v2, const float tolerance = 0.0f);
	static Vector2 Clamp(const Vector2 &v, const Vector2 min, const Vector2 max);
	static Vector2 Max(const Vector2 &v, const Vector2 max);
	static Vector2 Min(const Vector2 &v, const Vector2 min);
	static Vector2 LimitLength(const Vector2 &v, const float &f);
	static float GetAngle(const Vector2 &v1, const Vector2 &v2);

	Vector3 ToVector3(const float z = 0.0f) const;
	Vector4 ToVector4(const float z = 0.0f, const float w = 1.0f) const;
};

#pragma endregion

#pragma region Vector3

Vector3 operator*(const float f, const Vector3 &v);

class Vector3
{
public:
	Vector3(void) : x(0.0f), y(0.0f), z(0.0f) {};
	Vector3(const t_float& _x, const t_float& _y, const t_float& _z) : x(_x), y(_y), z(_z) {}
	Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}
	~Vector3(void) {};

	t_float x,y,z;

	void Set(t_float _x, t_float _y, t_float _z)		{ x = _x; y = _y; z = _z; }

	static Vector3 one;
	static Vector3 zero;

	t_float operator()(unsigned char index) const;
	t_float& operator()(unsigned char index);

	Vector3 operator+(const Vector3& rhs) const;
	void operator+=(const Vector3& rhs);

	Vector3 operator-(const Vector3& rhs) const;
	void operator-=(const Vector3& rhs);

	Vector3 operator*(const t_float& value) const;
	void operator*=(const t_float& value);

	Vector3 operator/(const t_float& value) const;
	void operator/=(const t_float& value);

	bool operator==(const Vector3& rhs) const;
	bool operator!=(const Vector3& rhs) const;

	Vector3 operator-() const			{return Vector3(-x,-y,-z); }

	void Reflect(Vector3 normal);

	static Vector3 Cross(const Vector3& lhs, const Vector3& rhs);
	static Vector3 Normalize(const Vector3& v);
	static Vector3 Lerp(Vector3 from, Vector3 to, float delta);
	static bool Compare(const Vector3 &v1, const Vector3 &v2, const float tolerance = 0.0f);

	static Vector3 Clamp(const Vector3 &v, const Vector3 min, const Vector3 max);
	static Vector3 Max(const Vector3 &v, const Vector3 max);
	static Vector3 Min(const Vector3 &v, const Vector3 min);
	static Vector3 LimitLength(const Vector3 &v, const float &f);

	t_float Dot(const Vector3& rhs) const;
	void Normalize();
	t_float LengthSqr() const;
	t_float Length() const;

	void SetLength(t_float length);
	bool IsNormal();

	Vector4 ToVector4(float w = 1.0f) const;
	Vector2 ToVector2() const;
};

#pragma endregion

#pragma region Vector4

class Vector4
{
public:
	t_float x,y,z,w;

	void Set(t_float _x, t_float _y, t_float _z, t_float _w = 1)			{ x = _x; y = _y; z = _z; w = _w; }

	Vector4(void) : x(0), y(0), z(0), w(1) {}
	Vector4(t_float _x, t_float _y, t_float _z) : x(_x), y(_y), z(_z), w(1) {}
	Vector4(t_float _x, t_float _y, t_float _z, t_float _w) : x(_x), y(_y), z(_z), w(_w) {}
	Vector4(const Vector4 &other);
	~Vector4(void);

	t_float operator()(unsigned char index) const;
	t_float& operator()(unsigned char index);

	bool operator==(const Vector4 &value) const;
	bool operator!=(const Vector4 &value) const;

	static bool Compare(const Vector4 &v1, const Vector4 &v2, const float tolerance);

	static Vector4 CreateQuaternion(Matrix m);

	Vector3 ToVector3() const;
	Vector2 ToVector2() const;
};

#pragma endregion

#pragma region Matrix


// All rotations use degrees
// This Matrix is in column major
class Matrix
{
private:
	t_float m[MATRIX_SIZE][MATRIX_SIZE];

public:
	Matrix(void);
	Matrix(const Matrix& other);
	~Matrix(void);

	void Print() const;
	void BuildIdentity();
	void Set(t_float x1,t_float x2, t_float x3, t_float x4, 
		t_float y1, t_float y2, t_float y3, t_float y4,
		t_float z1, t_float z2, t_float z3, t_float z4,
		t_float w1, t_float w2, t_float w3, t_float w4);
	void Set(t_int row, t_int column, t_float value);
	t_float Get(t_int row, t_int column) const;
	void Transpose();
	void BuildScale(const t_float &x, const t_float &y, const t_float &z);
	void BuildScaleUniform(const t_float xyz);
	void BuildRotX(t_float degree);
	void BuildRotY(t_float degree);
	void BuildRotZ(t_float degree);
	void BuildRotAxis(const Vector3 &oAxis, const t_float &fAngle);
	void BuildTranslation(const t_float &x, const t_float &y, const t_float &z);
	void BuildTranslation(const Vector3 &trans);
	Vector3 GetRight() const;
	Vector3 GetUp() const;
	Vector3 GetForward() const;
	Vector3 GetTranslation() const;
	void SetRight(Vector3 right);
	void SetUp(Vector3 up);
	void SetForward(Vector3 forward);
	void SetTranslation(Vector3 trans);
	Vector4 Transform(const Vector4& v) const;
	Vector3 Transform(const Vector3& v) const;
	Vector2 Transform(const Vector2& v) const;
	t_float* GetBuffer() const;

	static Matrix CreateRotX(const t_float &degree, Vector3 translation = Vector3::zero);
	static Matrix CreateRotY(const t_float &degree, Vector3 translation = Vector3::zero);
	static Matrix CreateRotZ(const t_float &degree, Vector3 translation = Vector3::zero);
	static Matrix CreateRotXYZ(const t_float& x, const t_float& y, const t_float& z, Vector3 translation = Vector3::zero);
	static Matrix CreateRotAxis(const Vector3& axis, const t_float& degree, Vector3 translation = Vector3::zero);
	static Matrix CreateRotQuaternion(const Vector4& quaternion);
	static Matrix CreateIdentity();
	static Matrix CreateTranslation(const t_float &x, const t_float &y, const t_float &z);
	static Matrix CreateTranslation(const Vector3 &trans);
	static Matrix CreateScale(const t_float &x, const t_float &y, const t_float &z);
	static Matrix CreateScaleUniform(const t_float &xyz);
	static Matrix CreateProjection(t_float fFieldOfViewY, t_float fAspectRatio, t_float fNearZ, t_float fFarZ);
	static Matrix CreateViewMatrix(const Vector3 &oCameraPos, const Vector3 &oLookAt);

	Matrix operator*(const Matrix &mat) const;
	t_float &operator()(const unsigned char i, const unsigned char j);
	t_float operator()(const unsigned char i, const unsigned char j) const;
};

#pragma endregion

class Rect
{
public:
	t_float x,y,width,height;
	Rect() : x(0), y(0), width(0), height(0) {}
	Rect(float _x, float _y, float _width, float _height) : x(_x), y(_y), width(_width), height(_height) {}
	~Rect(){}

	inline void TopLeft		(Vector2 *container) const		{ container->x = x; container->y = y; }
	inline void BottomLeft	(Vector2 *container) const		{ container->x = x; container->y = y + height; }
	inline void TopRight	(Vector2 *container) const		{ container->x = x + width; container->y = y; }
	inline void BottomRight	(Vector2 *container) const		{ container->x = x + width; container->y = y + height; }
	inline void Center		(Vector2 *container) const		{ container->x = x + width / 2; container->y = x + height / 2; }
};

namespace GeneralMath
{
	t_int WrapInt(t_int kX, t_int const kLowerBound, t_int const kUpperBound);
	t_uint WrapUInt(t_uint kX, t_uint const kLowerBound, t_uint const kUpperBound);
	t_float WrapFloat(t_float kX, t_int const kLowerBound, t_int const kUpperBound);
}
#endif