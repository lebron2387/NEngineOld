#pragma once

#include <math.h>
#include <Float.h>
#include <stdlib.h>

typedef unsigned char uint8;
typedef signed char int8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef bool Bool;
typedef float Float32;
typedef double Double32;
typedef char Char;
typedef unsigned char uChar;


#define EPSILON 1e-6f
#define PI 3.14159265358979323846f
#define SQRT1_2 0.7071067811865475244008443621048490f
#define DEG2RAD (PI / 180.0f)
#define RAD2DEG (180.0f / PI)
__forceinline Float32 DEG2RADX(Float32 x) {return (Float32)(x * (PI / 180.0f));}
__forceinline Float32 RAD2DEGX(Float32 x) {return (Float32)(x * (180.0f / PI));}

struct Vec2i;
struct Vec2d;
struct Vec2f;
struct Vec3i;
struct Vec3d;
struct Vec3f;
struct Vec4i;
struct Vec4d;
struct Vec4f;
struct Mat3i;
struct Mat3d;
struct Mat3f;
struct Mat4i;
struct Mat4d;
struct Mat4f;
struct Quati;
struct Quatd;
struct Quatf;

__forceinline Float32 math_sin_deg(Float32 deg) {return (Float32) sin(DEG2RADX(deg));}
__forceinline Float32 math_cos_deg(Float32 deg) {return (Float32) cos(DEG2RADX(deg));}
__forceinline Float32 math_asin_deg(Float32 x) {return RAD2DEGX(asin((Float32) x));}
__forceinline Float32 math_acos_deg(Float32 x) {return RAD2DEGX(acos((Float32) x));}
__forceinline Float32 math_tan_deg(Float32 deg) {return (float)tan(DEG2RADX(deg));}
__forceinline Float32 math_atan2_deg(Float32 x, Float32 y) {Float32 RAD2DEGX((Float32) atan2(x, y));}
__forceinline Float32 math_atan_deg(Float32 x) {return RAD2DEGX((Float32) atan(x));}

Float32 math_max2(Float32 a,Float32 b);
Float32 math_max2(Float32 a,Float32 b);
Float32 math_max3(Float32 a,Float32 b,Float32 c);
Float32 math_max4(Float32 a,Float32 b,Float32 c,Float32 d);
Float32 math_min2(Float32 a,Float32 b);
Float32 math_min3(Float32 a,Float32 b,Float32 c);
Float32 math_min4(Float32 a,Float32 b,Float32 c,Float32 d);
Float32 math_clamp(Float32 a, Float32 low, Float32 high);
Float32 math_ranged_random(Float32 v1,Float32 v2);
//Trigonometric functions:
Float32 math_cos(Float32 x);	//Compute cosine (function)	
Float32 math_sin(Float32 x);	//Compute sine (function)
Float32 math_tan(Float32 x);	//Compute tangent (function)
Float32 math_acos(Float32 x);	//Compute arc cosine (function)
Float32 math_asin(Float32 x);	//Compute arc sine (function)
Float32 math_atan(Float32 x);	//Compute arc tangent (function)
Float32 math_atan2(Float32 y,Float32 x);	//Compute arc tangent with two parameters (function)
//Hyperbolic functions:
Float32 math_cosh(Float32 x);	//Compute hyperbolic cosine (function)
Float32 math_sinh(Float32 x);	//Compute hyperbolic sine (function)
Float32 math_tanh(Float32 x);	//Compute hyperbolic tangent (function)
//Exponential and logarithmic functions:
Float32 math_exp(Float32 x);	//Compute exponential function (function)
Float32 math_frexp(Float32 x,int32 *y);	//Get significand and exponent (function)
Float32 math_ldexp(Float32 x,int32 y);	//Generate number from significand and exponent (function)
Float32 math_log(Float32 x);	//Compute natural logarithm (function)
Float32 math_log10(Float32 x);	//Compute common logarithm (function)
Float32 math_modf(Float32 x,Float32 *y);	//Break into fractional and integral parts (function)
Float32 math_power(Float32 x,Float32 y); //functions pow	Raise to power (function)
Float32 math_sqrt(Float32 x);  //Compute square root (function)
Float32 math_inv_sqrt(Float32 x);  //Compute 1/square root (function)
//Rounding, absolute value and remainder functions:
Float32 math_ceil(Float32 x);	//Round up value (function)
Float32 math_fabs(Float32 x);	//Compute absolute value (function)
int32 math_abs(int32 x);	//Compute absolute value (function)
Float32 math_floor(Float32 x);	//Round down value (function)
Float32 math_fmod(Float32 x,Float32 y);	//Compute remainder of division (function)


/*
__forceinline Vec3f MatBuildNormalForPlane(Vec3f p1, Vec3f p2, Vec3f p3)
{
	Vec3f vnormal;

	Vec3f v1 = p3 - p1;
	Vec3f v2 = p2 - p1;

	vnormal = v1 ^ v2;
	vnormal.normalize();

	return vnormal;
}

//***************************************************************************

__forceinline Vec3f MatAbsoluteVector(Vec3f vec)
{
	Vec3f vnew;

	vnew.x=fabs(vec.x);
	vnew.y=fabs(vec.y);
	vnew.z=fabs(vec.z);

	return vnew;
}

//***************************************************************************

__forceinline Float MatAngleBetweenVectors(Vec3f v1, Vec3f v2)
{
	Float dotProduct = v1 * v2;
	Float vectorsMagnitude = v1.length() * v2.length();
	Float angle = acos(dotProduct / vectorsMagnitude);
	if(_isnan(angle)) {return 0;}
	return(angle);
}

//***************************************************************************

__forceinline Float MatTwoPointDistance(Vec3f point1, Vec3f point2)
{
	return (Float)sqrt((point2.x-point1.x)*(point2.x-point1.x)+
					   (point2.y-point1.y)*(point2.y-point1.y)+
					   (point2.z-point1.z)*(point2.z-point1.z));
}

//***************************************************************************

__forceinline Float MatPointToPlaneDistance(Vec3f p_point,Vec3f normal,Vec3f point)
{
	return (normal.x * point.x + normal.y * point.y + normal.z * point.z -
			normal.x * p_point.x - normal.y * p_point.y - normal.z * p_point.z);
}

//***************************************************************************

__forceinline Float MatDForPlane_Equation(Vec3f normal,Vec3f point)
{
	return -((normal.x * point.x) + (normal.y * point.y) + (normal.z * point.z));
}

//***************************************************************************

__forceinline Float MatEquationArrangementPointToLine(Float x,Float y,Float x0,Float y0,Float x1,Float y1)
{
	return (Float)(y - y0) * (x1 - x0) - (x - x0) * (y1 - y0);
}

//***************************************************************************
*/