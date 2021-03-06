#include "Vector.h"
#include <math.h>
//#include <iostream>//atodekesu

Vector::Vector(void)
{
	X = 0;
	Y = 0;
	Z = 0;
}

Vector::Vector(float* factor)
{
	X = factor[0];
	Y = factor[1];
	Z = factor[2];
}

Vector::Vector(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

Vector::Vector(Vector* vector)
{
	X = vector->X;
	Y = vector->Y;
	Z = vector->Z;
}

Vector::Vector(const Vector& vector)
{
	X = vector.X;
	Y = vector.Y;
	Z = vector.Z;
}

Vector::~Vector(void)
{
}

float Vector::getX(void) const
{
	return X;
}

float Vector::getY(void) const
{
	return Y;
}

float Vector::getZ(void) const
{
	return Z;
}

float Vector::getMagnitude(void) const
{
	return sqrt(X*X + Y*Y + Z*Z);
}

void Vector::getVector(float* factor) const
{
	factor[0] = X;
	factor[1] = Y;
	factor[2] = Z;
}

void Vector::setX(float x)
{
	X = x;
}

void Vector::setY(float y)
{
	Y = y;
}

void Vector::setZ(float z)
{
	Z = z;
}

void Vector::setVector(float* factor)
{
	X = factor[0];
	Y = factor[1];
	Z = factor[2];
}

void Vector::setVector(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

void Vector::setVector(const Vector& vector)
{
	X = vector.X;
	Y = vector.Y;
	Z = vector.Z;
}

Vector Vector::operator+(const Vector& vector) const
{
	return Vector(X + vector.X, Y + vector.Y, Z + vector.Z);
}

Vector Vector::operator-(const Vector& vector) const
{
	return Vector(X - vector.X, Y - vector.Y, Z - vector.Z);
}

Vector Vector::operator*(float k) const
{
	return Vector(X * k, Y * k, Z * k);
}

Vector Vector::operator/(float k) const
{
	return Vector(X / k, Y / k, Z / k);
}

float Vector::operator*(const Vector& vector) const
{
	return (X * vector.X) + (Y * vector.Y) + (Z * vector.Z);
}

Vector Vector::operator%(const Vector& vector) const
{
	return Vector(
		Y*vector.Z - vector.Y*Z,
		Z*vector.X - vector.Z*X,
		X*vector.Y - vector.X*Y
	);
}

const Vector& Vector::operator=(const Vector& vector)
{
	X = vector.X;
	Y = vector.Y;
	Z = vector.Z;
	return *this;
}

const Vector& Vector::operator+=(const Vector& vector)
{
	X += vector.X;
	Y += vector.Y;
	Z += vector.Z;
	return *this;
}

const Vector& Vector::operator-=(const Vector& vector)
{
	X -= vector.X;
	Y -= vector.Y;
	Z -= vector.Z;
	return *this;
}

const Vector& Vector::operator*=(float k)
{
	X *= k;
	Y *= k;
	Z *= k;
	return *this;
}

const Vector& Vector::operator/=(float k)
{
	X /= k;
	Y /= k;
	Z /= k;
	return *this;
}
