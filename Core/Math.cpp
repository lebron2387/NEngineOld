#include "Math.h"
#include "Types.h"

//Min,Max functions:
Float32 math_max2(Float32 a,Float32 b)
{
	return (a > b ? a : b);
}

Float32 math_max3(Float32 a,Float32 b,Float32 c)
{
	return (math_max2(a, math_max2(b, c)));
}

Float32 math_max4(Float32 a,Float32 b,Float32 c,Float32 d)
{
	return (math_max2(a, math_max3(b, c, d)));
}

Float32 math_min2(Float32 a,Float32 b) 
{
	return (a < b ? a : b);
}

Float32 math_min3(Float32 a,Float32 b,Float32 c) 
{
	return (math_min2(a, math_min2(b, c)));
}

Float32 math_min4(Float32 a,Float32 b,Float32 c,Float32 d) 
{
	return (math_min2(a, math_min3(b, c, d)));
}

Float32 math_clamp(Float32 a, Float32 low, Float32 high)
{
	return math_max2(low, math_min2(a, high));
}

//Random functions:
Float32 math_ranged_random(Float32 v1,Float32 v2) 
{
	return v1 + (v2-v1)*((Float32)rand())/((Float32)RAND_MAX);
};

//Trigonometric functions:
Float32 math_cos(Float32 x)
{
	return cos(x);
}

Float32 math_sin(Float32 x)
{
	return sin(x);
}

Float32 math_tan(Float32 x)
{
	return tan(x);
}

Float32 math_acos(Float32 x)
{
	return acos(x);
}

Float32 math_asin(Float32 x)
{
	return asin(x);
}

Float32 math_atan(Float32 x)
{
	return atan(x);
}

Float32 math_atan2(Float32 y,Float32 x)
{
	return atan2(y,x);
}

//Hyperbolic functions:
Float32 math_cosh(Float32 x)
{
	return cosh(x);
}

Float32 math_sinh(Float32 x)
{
	return sinh(x);
}

Float32 math_tanh(Float32 x)
{
	return tanh(x);
}

//Exponential and logarithmic functions:
Float32 math_exp(Float32 x)
{
	return exp(x);
}

Float32 math_frexp(Float32 x,int32 *y)
{
	return frexp(x,y);
}

Float32 math_ldexp(Float32 x,int32 y)
{
	return ldexp(x,(int32)y);
}

Float32 math_log(Float32 x)
{
	return log(x);
}

Float32 math_log10(Float32 x)
{
	return log10(x);
}

Float32 math_modf(Float32 x,Float32 *y)
{
	return modf(x,(double*)y);
}

Float32 math_power(Float32 x,Float32 y)
{
	return pow(x,y);
}

Float32 math_sqrt(Float32 x)
{
	return sqrt(x);
}

Float32 math_inv_sqrt(Float32 x)
{
	return (1/sqrt(x));
}

//Rounding, absolute value and remainder functions:
Float32 math_ceil(Float32 x)
{
	return ceil(x);
}

Float32 math_fabs(Float32 x)
{
	return fabs(x);
}

int32 math_abs(int32 x)
{
	return (x < 0)? x*-1:x;
}

Float32 math_floor(Float32 x)
{
	return floor(x);
}

Float32 math_fmod(Float32 x,Float32 y)
{
	return fmod(x,y);
}
