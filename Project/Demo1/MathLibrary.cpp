#include "MathLibrary.h"
#include <iostream>

t_int GeneralMath::WrapInt(t_int kX, t_int const kLowerBound, t_int const kUpperBound)
{
    t_int range_size = kUpperBound - kLowerBound + 1;

    kX += range_size * ((kLowerBound - kX) / range_size + 1);

    return kLowerBound + (kX - kLowerBound) % range_size;
}

t_uint GeneralMath::WrapUInt(t_uint kX, t_uint const kLowerBound, t_uint const kUpperBound)
{
    t_uint range_size = kUpperBound - kLowerBound + 1;

    kX += range_size * ((kLowerBound - kX) / range_size + 1);

    return kLowerBound + (kX - kLowerBound) % range_size;
}

t_float GeneralMath::WrapFloat(t_float kX, t_int const kLowerBound, t_int const kUpperBound)
{
	t_int range_size = kUpperBound - kLowerBound + 1;

	t_float decimalPart, intPart;
	decimalPart = modf(kX, &intPart);

	t_int wrappedIntPart = WrapInt((t_int)intPart, kLowerBound, kUpperBound);

	return decimalPart + wrappedIntPart;
}

#pragma region Vector2

Vector2 Vector2::one(1.0f,1.0f);
Vector2 Vector2::zero(0.0f,0.0f);

Vector2 operator*(const float f, const Vector2 &v)
{
	return v * f;
}

Vector2::~Vector2(void)
{
}

Vector2 Vector2::operator+(const Vector2 &v) const
{
	Vector2 returnVect;
	returnVect.x = x + v.x;
	returnVect.y = y + v.y;
	return returnVect;
}

void Vector2::operator+=(const Vector2 &v)
{
	x += v.x;
	y += v.y;
}

Vector2 Vector2::operator-(const Vector2 &v) const
{
	Vector2 returnVect;
	returnVect.x = x - v.x;
	returnVect.y = y - v.y;
	return returnVect;
}

void Vector2::operator-=(const Vector2 &v)
{
	x -= v.x;
	y -= v.y;
}

Vector2 Vector2::operator*(const t_float &f) const
{
	Vector2 returnVect;
	returnVect.x = x * f;
	returnVect.y = y * f;
	return returnVect;
}

void Vector2::operator*=(const t_float &f)
{
	x *= f;
	y *= f;
}

Vector2 Vector2::operator/(const t_float &f) const
{
	Vector2 returnVect;
	returnVect.x = x / f;
	returnVect.y = y / f;
	return returnVect;
}

void Vector2::operator/=(const t_float &f)
{
	x /= f;
	y /= f;
}

void Vector2::SetLength(t_float length)
{
	Normalize();
	x *= length;
	y *= length;
}

t_float Vector2::Dot(const Vector2 &v) const
{
	return x * v.x + y * v.y;
}

void Vector2::Normalize()
{
	t_float len = Length();
	if(len != 0.0f)
	{
		x /= len;
		y /= len;
	}
}

t_float Vector2::Length() const
{
	t_float temp = LengthSqr();

	if(temp != 0.0f)
	{
		return sqrt(LengthSqr());
	}

	return 0.0f;
}

t_float Vector2::LengthSqr() const
{
	return x * x + y * y;
}

void Vector2::Reflect(Vector2 normal)
{
	Vector2 answer;
	answer = *this - (normal * (t_float)(this->Dot(normal) * 2));
	x = answer.x;
	y = answer.y;
}

Vector2 Vector2::Perpendicular()
{
	Vector2 perp;
	perp.x = -y;
	perp.y = x;
	return perp;
}

bool Vector2::Compare(const Vector2 &v1, const Vector2 &v2, const float tolerance)
{
	if(fabs(v1.x - v2.x) >= tolerance)
	{
		return false;
	}

	if(fabs(v1.y - v2.y) >= tolerance)
	{
		return false;
	}

	return true;
}

Vector2 Vector2::Clamp(const Vector2 &v, const Vector2 min, const Vector2 max)
{
	Vector2 retVal;
	retVal.x = CLAMP(v.x, min.x, max.x);
	retVal.y = CLAMP(v.y, min.y, max.y);
	return retVal;
}

Vector2 Vector2::Max(const Vector2 &v, const Vector2 max)
{
	Vector2 retVal;
	retVal.x = MAX(v.x, max.x);
	retVal.y = MAX(v.y, max.y);
	return retVal;
}

Vector2 Vector2::Min(const Vector2 &v, const Vector2 min)
{
	Vector2 retVal;
	retVal.x = MIN(v.x, min.x);
	retVal.y = MIN(v.y, min.y);
	return retVal;
}

Vector2 Vector2::LimitLength(const Vector2 &v, const float &f)
{
	Vector2 retVal = v;
	float distSqr = v.LengthSqr();
	if(distSqr > f * f)
	{
		float dist = sqrt(distSqr);
		retVal /= dist;
		retVal *= f;
	}
	return retVal;
}

float Vector2::GetAngle(const Vector2 &v1, const Vector2 &v2)
{
	return RADTODEG(atan2(v1.x * v2.y - v1.y * v2.x, v1.x * v2.x + v1.y * v2.y));
}

Vector3 Vector2::ToVector3(const float z) const
{
	return Vector3(x,y,z);
}

Vector4 Vector2::ToVector4(const float z, const float w) const
{
	return Vector4(x,y,z,w);
}

#pragma endregion

#pragma region Vector3

Vector3 Vector3::one = Vector3(1.0f,1.0f,1.0f);
Vector3 Vector3::zero = Vector3(0.0f,0.0f,0.0f);

Vector3 operator*(const float f, const Vector3 &v)
{
	return v * f;
}

t_float Vector3::operator()(unsigned char index) const
{
	if(index == 0)
	{
		return x;
	}
	else if(index == 1)
	{
		return y;
	}

	return z;
}

t_float& Vector3::operator()(unsigned char index)
{
	if(index == 0)
	{
		return x;
	}
	else if(index == 1)
	{
		return y;
	}

	return z;
}

Vector3 Vector3::operator+(const Vector3& rhs) const
{
	Vector3 ans(x + rhs.x, y + rhs.y, z + rhs.z);
	return ans;
}

void Vector3::operator+=(const Vector3& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
}

Vector3 Vector3::operator-(const Vector3& rhs) const
{
	Vector3 ans(x - rhs.x, y - rhs.y, z - rhs.z);
	return ans;
}

void Vector3::operator-=(const Vector3& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
}

Vector3 Vector3::operator*(const t_float& value) const
{
	Vector3 ans(x * value, y * value, z * value);
	return ans;
}

void Vector3::operator*=(const t_float& value)
{
	x *= value;
	y *= value;
	z *= value;
}

Vector3 Vector3::operator/(const t_float& value) const
{
	Vector3 ans(x / value, y / value, z / value);
	return ans;
}

void Vector3::operator/=(const t_float& value)
{
	x /= value;
	y /= value;
	z /= value;
}

bool Vector3::operator==(const Vector3& rhs) const
{
	return x == rhs.x && y == rhs.y && z == rhs.z;
}

bool Vector3::operator!=(const Vector3& rhs) const
{
	return x != rhs.x || y != rhs.y || z != rhs.z;
}

void Vector3::Reflect(Vector3 normal)
{
	Vector3 answer;
	answer = *this - (normal * (t_float)(this->Dot(normal) * 2));
	x = answer.x;
	y = answer.y;
	z = answer.z;
}

Vector3 Vector3::Cross(const Vector3& lhs, const Vector3& rhs)
{
	//////////////////////////////////////
	//									//
	//  rhs is the bottom one			//
	//									//
	//	|	i		j		k	|		//
	//	|	x		y		z	|		//
	//	|	ro.x	ro.y	ro.z|		//
	//									//
	//////////////////////////////////////

	Vector3 ans;

	ans.x = (lhs.y * rhs.z) - (lhs.z * rhs.y);
	ans.y = -((lhs.x * rhs.z) - (lhs.z * rhs.x));
	ans.z = (lhs.x * rhs.y) - (lhs.y * rhs.x);

	return ans;
}

Vector3 Vector3::Lerp(Vector3 from, Vector3 to, float delta)
{
	//alpha * black + (1 - alpha) * red
	return to * delta + from * (1 - delta);
}

bool Vector3::Compare(const Vector3 &v1, const Vector3 &v2, const float tolerance)
{
	if(fabs(v1.x - v2.x) >= tolerance)
	{
		return false;
	}

	if(fabs(v1.y - v2.y) >= tolerance)
	{
		return false;
	}

	if(fabs(v1.z - v2.z) >= tolerance)
	{
		return false;
	}

	return true;
}

Vector3 Vector3::Clamp(const Vector3 &v, const Vector3 min, const Vector3 max)
{
	Vector3 retVal;
	retVal.x = CLAMP(v.x, min.x, max.x);
	retVal.y = CLAMP(v.y, min.y, max.y);
	retVal.z = CLAMP(v.z, min.z, max.z);
	return retVal;
}

Vector3 Vector3::Max(const Vector3 &v, const Vector3 max)
{
	Vector3 retVal;
	retVal.x = MAX(v.x, max.x);
	retVal.y = MAX(v.y, max.y);
	retVal.z = MAX(v.z, max.z);
	return retVal;
}

Vector3 Vector3::Min(const Vector3 &v, const Vector3 min)
{
	Vector3 retVal;
	retVal.x = MIN(v.x, min.x);
	retVal.y = MIN(v.y, min.y);
	retVal.z = MIN(v.z, min.z);
	return retVal;
}

Vector3 Vector3::LimitLength(const Vector3 &v, const float &f)
{
	Vector3 retVal = v;
	float distSqr = v.LengthSqr();
	if(distSqr > f * f)
	{
		float dist = sqrt(distSqr);
		retVal /= dist;
		retVal *= f;
	}
	return retVal;
}

Vector3 Vector3::Normalize(const Vector3& v)
{
	Vector3 copy(v);
	copy.Normalize();
	return copy;
}

void Vector3::Normalize()
{
	t_float length(Length());
	if(length != 0.0f)
	{
		x /= length;
		y /= length;
		z /= length;
	}
}

t_float Vector3::LengthSqr() const
{
	return x * x + y * y + z * z;
}

t_float Vector3::Length() const
{
	return sqrt(x * x + y * y + z * z);
}

t_float Vector3::Dot(const Vector3& rhs) const
{
	return x * rhs.x + y * rhs.y + z * rhs.z;
}

void Vector3::SetLength(t_float length)
{
	Normalize();
	x *= length;
	y *= length;
	z *= length;
}

bool Vector3::IsNormal()
{
	return (x * x + y * y + z * z) == 1.0f;
}

Vector4 Vector3::ToVector4(float w) const
{
	return Vector4(x,y,z,w);
}

Vector2 Vector3::ToVector2() const
{
	return Vector2(x,y);
}

#pragma endregion

#pragma region Vector4

Vector4::Vector4(const Vector4 &other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
}

Vector4::~Vector4(void)
{
}

t_float Vector4::operator()(unsigned char index) const
{
	if(index == 0)
	{
		return x;
	}
	else if(index == 1)
	{
		return y;
	}
	else if(index == 2)
	{
		return z;
	}

	return w;
}

t_float& Vector4::operator()(unsigned char index)
{
	if(index == 0)
	{
		return x;
	}
	else if(index == 1)
	{
		return y;
	}
	else if(index == 2)
	{
		return z;
	}

	return w;
}

bool Vector4::Compare(const Vector4 &v1, const Vector4 &v2, const float tolerance)
{
	if(fabs(v1.x - v2.x) >= tolerance)
	{
		return false;
	}

	if(fabs(v1.y - v2.y) >= tolerance)
	{
		return false;
	}

	if(fabs(v1.z - v2.z) >= tolerance)
	{
		return false;
	}

	if(fabs(v1.w - v2.w) >= tolerance)
	{
		return false;
	}

	return true;
}

bool Vector4::operator==(const Vector4 &value) const
{
	return x == value.x && y == value.y && z == value.z && w == value.x;
}

bool Vector4::operator!=(const Vector4 &value) const
{
	return x != value.x || y != value.y || z != value.z || w != value.x;
}
//////////////////////////////////////////////////////////////////////////////////////////

Vector4 Vector4::CreateQuaternion(Matrix m)
{
	Vector4 q(0,0,0,0);
	return q;
}

Vector3 Vector4::ToVector3() const
{
	return Vector3(x,y,z);
}

Vector2 Vector4::ToVector2() const
{
	return Vector2(x,y);
}

#pragma endregion

#pragma region Matrix

/*
All rotations use degrees
*/

Matrix::Matrix(void)
{
	memset(m, 0, MATRIX_SIZE * MATRIX_SIZE * sizeof(t_float));

	m[0][0] = 1;
	m[1][1] = 1;
	m[2][2] = 1;
	m[3][3] = 1;
}


////////////////////////////////////////////////////////////////////////////////////
// Function: Copy Constructor
//
// Purpose: Sets the values of the Matrix
//
// Arguments: const Matrix& other - the other matrix to copy from
////////////////////////////////////////////////////////////////////////////////////
Matrix::Matrix(const Matrix& other)
{
	memcpy(m, other.GetBuffer(), sizeof(t_float) * 16); 
}

Matrix::~Matrix(void)
{
}


////////////////////////////////////////////////////////////////////////////////////
// Function: Get
//
// Purpose: used to get a value at the row and column
//
// Arguments: t_int row - the row the value it at
//	t_int column - the column the value is at
//
// Return: t_float - the value at the specified row and column
////////////////////////////////////////////////////////////////////////////////////
t_float Matrix::Get(t_int row, t_int column) const
{
	return m[row][column];
}


////////////////////////////////////////////////////////////////////////////////////
// Function: BuildIdentity
//
// Purpose: used to set the current Matrix to an Identity Matrix
//
// Arguments: none
//
// Return: none
////////////////////////////////////////////////////////////////////////////////////
void Matrix::BuildIdentity()
{
	memset(m, 0, MATRIX_SIZE * MATRIX_SIZE * sizeof(t_float));

	m[0][0] = 1;
	m[1][1] = 1;
	m[2][2] = 1;
	m[3][3] = 1;
}


////////////////////////////////////////////////////////////////////////////////////
// Function: Set
//
// Purpose: used to set all the values individually
//
// Arguments: 16 floats
//	the letters represent the row
//	the numbers represent the column
//
// Return: none
////////////////////////////////////////////////////////////////////////////////////
void Matrix::Set(
	t_float x1, t_float x2, t_float x3, t_float x4, 
	t_float y1, t_float y2, t_float y3, t_float y4,
	t_float z1, t_float z2, t_float z3, t_float z4,
	t_float w1, t_float w2, t_float w3, t_float w4)
{
	m[0][0] = x1;	m[0][1] = y1;	m[0][2] = z1;	m[0][3] = w1;
	m[1][0] = x2;	m[1][1] = y2;	m[1][2] = z2;	m[1][3] = w2;
	m[2][0] = x3;	m[2][1] = y3;	m[2][2] = z3;	m[2][3] = w3;
	m[3][0] = x4;	m[3][1] = y4;	m[3][2] = z4;	m[3][3] = w4;
}


////////////////////////////////////////////////////////////////////////////////////
// Function: Set
//
// Purpose: used to set the value at the row and column provided
//
// Arguments: t_int row - the row of the value to set
//	t_int column - the column of the value to set
//
// Return: 
////////////////////////////////////////////////////////////////////////////////////
void Matrix::Set(t_int row, t_int column, t_float value)
{
	m[row][column] = value;
}


////////////////////////////////////////////////////////////////////////////////////
// Function: Transpose
//
// Purpose: used to transpose the current Matrix
//
// Arguments: none
//
// Return: none
////////////////////////////////////////////////////////////////////////////////////
void Matrix::Transpose()
{
	Matrix temp;
	for(t_int i = 0; i < MATRIX_SIZE; ++i)
	{
		for(t_int j = 0; j < MATRIX_SIZE; ++j)
		{
			temp.Set(j,i,m[i][j]);
		}
	}

	for(t_int i = 0; i < MATRIX_SIZE; ++i)
	{
		for(t_int j = 0; j < MATRIX_SIZE; ++j)
		{
			m[i][j] = temp.m[i][j];
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////
// Function: BuildScale
//
// Purpose: used to turn the current Matrix into a Scale Matrix
//
// Arguments: t_float x,y,z - the scale to set along the x,y,z axes
//
// Return: none
////////////////////////////////////////////////////////////////////////////////////
void Matrix::BuildScale(const t_float &x, const t_float &y, const t_float &z)
{
	BuildIdentity();
	Set(0,0,x);
	Set(1,1,y);
	Set(2,2,z);
}

void Matrix::BuildScaleUniform(const t_float xyz)
{
	BuildScale(xyz,xyz,xyz);
}

Matrix Matrix::operator*(const Matrix &oRHS) const
{
	Matrix returnValue;
	t_float value = 0.0f;
	
	Matrix oRes;

	// Matrix multiplication
	oRes.m[0][0] = m[0][0]*oRHS.m[0][0] + m[1][0]*oRHS.m[0][1] + m[2][0]*oRHS.m[0][2] + m[3][0]*oRHS.m[0][3];
	oRes.m[1][0] = m[0][0]*oRHS.m[1][0] + m[1][0]*oRHS.m[1][1] + m[2][0]*oRHS.m[1][2] + m[3][0]*oRHS.m[1][3];
	oRes.m[2][0] = m[0][0]*oRHS.m[2][0] + m[1][0]*oRHS.m[2][1] + m[2][0]*oRHS.m[2][2] + m[3][0]*oRHS.m[2][3];
	oRes.m[3][0] = m[0][0]*oRHS.m[3][0] + m[1][0]*oRHS.m[3][1] + m[2][0]*oRHS.m[3][2] + m[3][0]*oRHS.m[3][3];

	oRes.m[0][1] = m[0][1]*oRHS.m[0][0] + m[1][1]*oRHS.m[0][1] + m[2][1]*oRHS.m[0][2] + m[3][1]*oRHS.m[0][3];
	oRes.m[1][1] = m[0][1]*oRHS.m[1][0] + m[1][1]*oRHS.m[1][1] + m[2][1]*oRHS.m[1][2] + m[3][1]*oRHS.m[1][3];
	oRes.m[2][1] = m[0][1]*oRHS.m[2][0] + m[1][1]*oRHS.m[2][1] + m[2][1]*oRHS.m[2][2] + m[3][1]*oRHS.m[2][3];
	oRes.m[3][1] = m[0][1]*oRHS.m[3][0] + m[1][1]*oRHS.m[3][1] + m[2][1]*oRHS.m[3][2] + m[3][1]*oRHS.m[3][3];

	oRes.m[0][2] = m[0][2]*oRHS.m[0][0] + m[1][2]*oRHS.m[0][1] + m[2][2]*oRHS.m[0][2] + m[3][2]*oRHS.m[0][3];
	oRes.m[1][2] = m[0][2]*oRHS.m[1][0] + m[1][2]*oRHS.m[1][1] + m[2][2]*oRHS.m[1][2] + m[3][2]*oRHS.m[1][3];
	oRes.m[2][2] = m[0][2]*oRHS.m[2][0] + m[1][2]*oRHS.m[2][1] + m[2][2]*oRHS.m[2][2] + m[3][2]*oRHS.m[2][3];
	oRes.m[3][2] = m[0][2]*oRHS.m[3][0] + m[1][2]*oRHS.m[3][1] + m[2][2]*oRHS.m[3][2] + m[3][2]*oRHS.m[3][3];

	oRes.m[0][3] = m[0][3]*oRHS.m[0][0] + m[1][3]*oRHS.m[0][1] + m[2][3]*oRHS.m[0][2] + m[3][3]*oRHS.m[0][3];
	oRes.m[1][3] = m[0][3]*oRHS.m[1][0] + m[1][3]*oRHS.m[1][1] + m[2][3]*oRHS.m[1][2] + m[3][3]*oRHS.m[1][3];
	oRes.m[2][3] = m[0][3]*oRHS.m[2][0] + m[1][3]*oRHS.m[2][1] + m[2][3]*oRHS.m[2][2] + m[3][3]*oRHS.m[2][3];
	oRes.m[3][3] = m[0][3]*oRHS.m[3][0] + m[1][3]*oRHS.m[3][1] + m[2][3]*oRHS.m[3][2] + m[3][3]*oRHS.m[3][3];

	return oRes;
	/*
	for(t_int i = 0; i < MATRIX_SIZE; ++i)
	{
		for(t_int j = 0; j < MATRIX_SIZE; ++j)
		{
			value = 0.0f;
			for(t_int k = 0; k < MATRIX_SIZE; ++k)
			{
				value += mat.m[i][k] * m[k][j];
			}
			returnValue.Set(i,j,value);
		}
	}

	return returnValue;*/
}

void Matrix::BuildRotX(t_float degree)
{
	BuildIdentity();
	t_float ca = cosf(DEGTORAD(degree));
	t_float sa = sinf(DEGTORAD(degree));
	Set(1,1, ca);
	Set(1,2, -sa);
	Set(2,1, sa);
	Set(2,2, ca);
}

void Matrix::BuildRotY(t_float degree)
{
	BuildIdentity();
	t_float ca = cosf(DEGTORAD(degree));
	t_float sa = sinf(DEGTORAD(degree));
	Set(0,0, ca);
	Set(0,2, sa);
	Set(2,0, -sa);
	Set(2,2, ca);
}

void Matrix::BuildRotZ(t_float degree)
{
	BuildIdentity();
	t_float ca = cosf(DEGTORAD(degree));
	t_float sa = sinf(DEGTORAD(degree));
	Set(0,0, ca);
	Set(0,1, -sa);
	Set(1,0, sa);
	Set(1,1, ca);
}


void Matrix::BuildRotAxis(const Vector3 &oAxis, const t_float &fAngle)
{
	// 1. Perform transformations which align rotation axis with one of coordinate axis (x, y, z).
	//	  This is essentially a transformation matrix with the given vector as one of the rows, and
	//    orthogonal vectors as the other rows.
	// 2. Perform rotation about the axis 
	// 3. Do inverse of (1)
	t_float fCosTheta = cosf( fAngle );
	t_float fSinTheta = sinf( fAngle );

	BuildIdentity();
	Set(0, 0, fCosTheta + (1 - fCosTheta) * (oAxis.x * oAxis.x));
	Set(1, 0, (1 - fCosTheta)*(oAxis.x * oAxis.y) + (fSinTheta * oAxis.z));
	Set(2, 0, (1 - fCosTheta)*(oAxis.x * oAxis.z) - (fSinTheta * oAxis.y));	
	Set(0, 1, (1 - fCosTheta)*(oAxis.x * oAxis.y) - (fSinTheta * oAxis.z));
	Set(1, 1, fCosTheta + (1 - fCosTheta) * (oAxis.y * oAxis.y));
	Set(2, 1, (1 - fCosTheta)*(oAxis.y * oAxis.z) + (fSinTheta * oAxis.x));
	Set(0, 2, (1 - fCosTheta)*(oAxis.x * oAxis.z) + (fSinTheta * oAxis.y));
	Set(1, 2, (1 - fCosTheta)*(oAxis.y * oAxis.z) - (fSinTheta * oAxis.x));
	Set(2, 2, fCosTheta + (1 - fCosTheta) * (oAxis.z * oAxis.z));
}

void Matrix::BuildTranslation(const t_float &x, const t_float &y, const t_float &z)
{
	BuildIdentity();
	Set(3,0,x);
	Set(3,1,y);
	Set(3,2,z);
}

void Matrix::BuildTranslation(const Vector3 &trans)
{
	BuildIdentity();
	Set(3,0,trans.x);
	Set(3,1,trans.y);
	Set(3,2,trans.z);
}

Vector4 Matrix::Transform(const Vector4& v) const
{
	Vector4 rVect;
	rVect.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0];
	rVect.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1];
	rVect.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2];
	rVect.w = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3];
	return rVect;
}

Vector3 Matrix::Transform(const Vector3& v) const
{
	Vector3 rVect;
	rVect.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0];
	rVect.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1];
	rVect.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2];
	return rVect;
}

// only applies z rotation
Vector2 Matrix::Transform(const Vector2& v) const
{
	Vector2 rVect;
	rVect.x = v.x * m[0][0] + v.y * m[1][0] + m[3][0];
	rVect.y = v.x * m[0][1] + v.y * m[1][1] + m[3][1];
	return rVect;
}

Vector3 Matrix::GetTranslation() const
{
	Vector3 r(m[3][0],m[3][1],m[3][2]);
	return r;
}

Vector3 Matrix::GetRight() const
{
	return Vector3(m[0][0],m[1][0],m[2][0]);
}

Vector3 Matrix::GetUp() const
{
	return Vector3(m[0][1],m[1][1],m[2][1]);
}

Vector3 Matrix::GetForward() const
{
	return Vector3(-m[0][2],-m[1][2],-m[2][2]);
}

void Matrix::SetRight(Vector3 right)
{
	m[0][0] = right.x;
	m[1][0] = right.y;
	m[2][0] = right.z;
}

void Matrix::SetUp(Vector3 up)
{
	m[0][1] = up.x;
	m[1][1] = up.y;
	m[2][1] = up.z;
}

void Matrix::SetForward(Vector3 forward)
{
	m[0][2] = -forward.x;
	m[1][2] = -forward.y;
	m[2][2] = -forward.z;
}

void Matrix::SetTranslation(Vector3 trans)
{
	m[3][0] = trans.x;
	m[3][1] = trans.y;
	m[3][2] = trans.z;
}

t_float* Matrix::GetBuffer() const
{
	//Transpose();
	//transpose is not needed since mine is already column major
	return (t_float*)m;
}

t_float &Matrix::operator()(const unsigned char i, const unsigned char j)
{
	return m[i >= 4 ? 3 : i][j >= 4 ? 3 : j];
}

t_float Matrix::operator()(const unsigned char i, const unsigned char j) const
{
	return m[i >= 4 ? 3 : i][j >= 4 ? 3 : j];
}

////////////////////////////////////////////////////////////////////////////////////////////
// static functions
//
Matrix Matrix::CreateRotX(const t_float &degree, Vector3 translation)
{
	Matrix rM;
	rM.BuildRotX(degree);

	if(translation != Vector3(0,0,0))
	{
		Matrix trans,unTrans;
		trans.BuildTranslation(translation);
		unTrans.BuildTranslation(-translation);
		rM = trans * rM * unTrans;
	}

	return rM;
}

Matrix Matrix::CreateRotY(const t_float &degree, Vector3 translation)
{
	Matrix rM;
	rM.BuildRotY(degree);

	if(translation != Vector3(0,0,0))
	{
		Matrix trans,unTrans;
		trans.BuildTranslation(translation);
		unTrans.BuildTranslation(-translation);
		rM = trans * rM * unTrans;
	}

	return rM;
}

Matrix Matrix::CreateRotZ(const t_float &degree, Vector3 translation)
{
	Matrix rM;
	rM.BuildRotZ(degree);

	if(translation != Vector3(0,0,0))
	{
		Matrix trans,unTrans;
		trans.BuildTranslation(translation);
		unTrans.BuildTranslation(-translation);
		rM = trans * rM * unTrans;
	}

	return rM;
}

Matrix Matrix::CreateRotXYZ(const t_float& x, const t_float& y, const t_float& z, Vector3 translation)
{
	Matrix rX,rY,rZ,rM;
	rX.BuildRotX(x);
	rY.BuildRotY(y);
	rZ.BuildRotZ(z);
	rM = rX * rY * rZ;

	if(translation != Vector3(0,0,0))
	{
		Matrix trans,unTrans;
		trans.BuildTranslation(translation);
		unTrans.BuildTranslation(-translation);
		rM = trans * rM * unTrans;
	}

	return rM;
}

Matrix Matrix::CreateRotAxis(const Vector3& axis, const t_float& degree, Vector3 translation)
{
	Matrix rM;
	rM.BuildRotAxis(axis, degree);

	if(translation != Vector3(0,0,0))
	{
		Matrix trans,unTrans;
		trans.BuildTranslation(translation);
		unTrans.BuildTranslation(-translation);
		rM = trans * rM * unTrans;
	}

	return rM;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Creates a rotational matrix based on a Vector4 this function assumes is a quaternion
//
//	x = 1 y = 2 z = 3 w = 0
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
Matrix Matrix::CreateRotQuaternion(const Vector4& q)
{
	Matrix m = Matrix::CreateIdentity();

	//version 1
	/*
	m.Set(0,0, 1.0f - 2.0f * (q2 * q2 + q3 * q3));	m.Set(1,0, 2.0f * (q1 * q2 - q0 * q3));			m.Set(2,0, 2.0f * (q0 * q2 + q1 * q3));
	m.Set(0,1, 2.0f * (q1 * q2 + q0 * q3));			m.Set(1,1, 1.0f - 2.0f * (q1 * q1 + q3 * q3));	m.Set(2,1, 2.0f * (q2 * q3 - q0 * q1));
	m.Set(0,2, 2.0f * (q1 * q3 - q0 * q2));			m.Set(1,2, 2.0f * (q0 * q1 + q2 * q3));			m.Set(2,2, 1.0f - 2.0f * (q1 * q1 + q2 * q2));
	*/

	//version 2
	/*
	m.Set(0,0, a * a + b * b - c * c - d * d);		m.Set(1,0, 2.0f * (b * c - a * d));					m.Set(2,0, 2.0f * (a * c + b * d));
	m.Set(0,1, 2.0f * (b * c + a * d));				m.Set(1,1, a * a - b * b + c * c - d * d);			m.Set(2,1, 2.0f * (c * d - a * b));
	m.Set(0,2, 2.0f * (b * d - a * c));				m.Set(1,2, 2.0f * (a * b + c * d));					m.Set(2,2, a * a - b * b - c * c + d * d);
	*/

	//currently using version 1
	m.Set(0,0, 1.0f - 2.0f * (q.y * q.y + q.z * q.z));	m.Set(1,0, 2.0f * (q.x * q.y - q.w * q.z));			m.Set(2,0, 2.0f * (q.w * q.y + q.x * q.z));
	m.Set(0,1, 2.0f * (q.x * q.y + q.w * q.z));			m.Set(1,1, 1.0f - 2.0f * (q.x * q.x + q.z * q.z));	m.Set(2,1, 2.0f * (q.y * q.z - q.w * q.x));
	m.Set(0,2, 2.0f * (q.x * q.z - q.w * q.y));			m.Set(1,2, 2.0f * (q.w * q.x + q.y * q.z));			m.Set(2,2, 1.0f - 2.0f * (q.x * q.x + q.y * q.y));

	return m;
}

Matrix Matrix::CreateIdentity()
{
	Matrix rM;
	rM.BuildIdentity();
	return rM;
}

Matrix Matrix::CreateTranslation(const t_float &x, const t_float &y, const t_float &z)
{
	Matrix rM;
	rM.BuildTranslation(x,y,z);
	return rM;
}

Matrix Matrix::CreateTranslation(const Vector3 &trans)
{
	Matrix rM;
	rM.BuildTranslation(trans);
	return rM;
}

Matrix Matrix::CreateScale(const t_float &x, const t_float &y, const t_float &z)
{
	Matrix rM;
	rM.BuildScale(x,y,z);
	return rM;
}

Matrix Matrix::CreateScaleUniform(const t_float &xyz)
{
	Matrix rM;
	rM.BuildScaleUniform(xyz);
	return rM;
}

Matrix Matrix::CreateProjection(t_float fFieldOfViewY, t_float fAspectRatio, t_float fNearZ, t_float fFarZ)
{
	Matrix oProjMat;

	// Transposed version of D3DXMatrixPerspectiveFovRH
	t_float fYScale = 1 / tanf( fFieldOfViewY / 2.0f );
	t_float fXScale = fYScale / fAspectRatio;
	memset( oProjMat.GetBuffer(), 0, sizeof( t_float ) * 16 );
	oProjMat.Set(0, 0, fXScale);
	oProjMat.Set(1, 1, fYScale);
	oProjMat.Set(2, 2, fFarZ / ( fNearZ - fFarZ ));
	oProjMat.Set(3, 2, ( fNearZ * fFarZ ) / ( fNearZ - fFarZ ));
	oProjMat.Set(2, 3, -1.0f);
	return oProjMat;
}


Matrix Matrix::CreateViewMatrix(const Vector3 &oCameraPos, const Vector3 &oLookAt)
{
	Matrix oTranslate, oRotate, oView;

	Vector3 oViewZ = oLookAt - oCameraPos; 
	oViewZ.Normalize();
	Vector3 oViewX = Vector3::Cross(oViewZ, Vector3(0,1,0)); 
	oViewX.Normalize();
	Vector3 oViewY = Vector3::Cross(oViewX, oViewZ);

	oRotate.BuildIdentity();
	//oRotate.Set(0, 0, oViewX.GetX()); oRotate.Set(0, 1, oViewX.GetY()); oRotate.Set(0, 2, oViewX.GetZ());
	//oRotate.Set(1, 0, oViewY.GetX()); oRotate.Set(1, 1, oViewY.GetY()); oRotate.Set(1, 2, oViewY.GetZ());
	//oRotate.Set(2, 0, -oViewZ.GetX()); oRotate.Set(2, 1, -oViewZ.GetY()); oRotate.Set(2, 2, -oViewZ.GetZ());
	oRotate.SetRight(oViewX);
	oRotate.SetUp(oViewY);
	oRotate.SetForward(oViewZ);
	oTranslate.BuildTranslation(-oCameraPos.x, -oCameraPos.y, -oCameraPos.z);

	//Matrix::Multiply(oView, oRotate, oTranslate);
	oView = oRotate * oTranslate;

	return oView;
}

Matrix Matrix::CreateViewMatrix(const Vector3 &oCameraPos, const Vector3 &oLookAt, const Vector3 &upVector)
{
	Matrix oTranslate, oRotate, oView;

	Vector3 oViewZ = oLookAt - oCameraPos; 
	oViewZ.Normalize();
	Vector3 oViewX = Vector3::Cross(oViewZ, upVector); 
	oViewX.Normalize();
	Vector3 oViewY = Vector3::Cross(oViewX, oViewZ);

	oRotate.BuildIdentity();
	//oRotate.Set(0, 0, oViewX.GetX()); oRotate.Set(0, 1, oViewX.GetY()); oRotate.Set(0, 2, oViewX.GetZ());
	//oRotate.Set(1, 0, oViewY.GetX()); oRotate.Set(1, 1, oViewY.GetY()); oRotate.Set(1, 2, oViewY.GetZ());
	//oRotate.Set(2, 0, -oViewZ.GetX()); oRotate.Set(2, 1, -oViewZ.GetY()); oRotate.Set(2, 2, -oViewZ.GetZ());
	oRotate.SetRight(oViewX);
	oRotate.SetUp(oViewY);
	oRotate.SetForward(oViewZ);
	oTranslate.BuildTranslation(-oCameraPos.x, -oCameraPos.y, -oCameraPos.z);

	//Matrix::Multiply(oView, oRotate, oTranslate);
	oView = oRotate * oTranslate;

	return oView;
}
////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion