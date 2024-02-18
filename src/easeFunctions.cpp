#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

// Sine
static double inSine(double x)
{
	return 1.0 - cos((x * M_PI) / 2.0);
}

static double outSine(double x)
{
	return sin((x * M_PI) / 2.0);
}

static double inOutSine(double x)
{
	return -(cos(M_PI * x) - 1.0) / 2.0;
}

// Quad
static double inQuad(double x)
{
	return x * x;
}

static double outQuad(double x)
{
	return 1.0 - (1.0 - x) * (1.0 - x);
}

static double inOutQuad(double x)
{
	return x < 0.5 ? 2.0 * x * x : 1.0 - pow(-2.0 * x + 2.0, 2.0) / 2.0;
}

// Cubic
static double inCubic(double x)
{
	return x * x * x;
}

static double outCubic(double x)
{
	return 1.0 - pow(1.0 - x, 3.0);
}

static double inOutCubic(double x)
{
	return x < 0.5 ? 4.0 * x * x * x : 1.0 - pow(-2.0 * x + 2.0, 3.0) / 2.0;
}

// Quart
static double inQuart(double x)
{
	return x * x * x * x;
}

static double outQuart(double x)
{
	return 1.0 - pow(1.0 - x, 4.0);
}

static double inOutQuart(double x)
{
	return x < 0.5 ? 8.0 * x * x * x * x : 1.0 - pow(-2.0 * x + 2.0, 4.0) / 2.0;
}

// Quint
static double inQuint(double x)
{
	return x * x * x * x * x;
}
   
static double outQuint(double x)
{
	return 1.0 - pow(1.0 - x, 5.0);
}

static double inOutQuint(double x)
{
	return x < 0.5 ? 16.0 * x * x * x * x * x : 1.0 - pow(-2.0 * x + 2.0, 5.0) / 2.0;
}

// Expo
static double inExpo(double x)
{
	return x == 0.0 ? 0.0 : pow(2.0, 10.0 * x - 10.0);
}

static double outExpo(double x)
{
	return x == 1.0 ? 1.0 : 1.0 - pow(2.0, -10.0 * x);
}

static double inOutExpo(double x)
{
	return x == 0.0
		? 0.0
		: x == 1.0
		? 1.0
		: x < 0.5
		? pow(2.0, 20.0 * x - 10.0) / 2.0
		: (2.0 - pow(2.0, -20.0 * x + 10.0)) / 2.0;
}

// Circ
static double inCirc(double x)
{
	if(x <= 0.0) return 0.0;
	if (x >= 1.0) return 1.0;
	return 1.0 - sqrt(1.0 - pow(x, 2.0));
}

static double outCirc(double x)
{
	return sqrt(1.0 - pow(x - 1.0, 2.0));
}

static double inOutCirc(double x)
{
	return x < 0.5
		? (1.0 - sqrt(1.0 - pow(2.0 * x, 2.0))) / 2.0
		: (sqrt(1.0 - pow(-2.0 * x + 2.0, 2.0)) + 1.0) / 2.0;
}

// Back
static double inBack(double x)
{
	constexpr double c1 = 1.70158;
	constexpr double c3 = c1 + 1.0;

	return c3 * x * x * x - c1 * x * x;
}

static double outBack(double x)
{
	constexpr double c1 = 1.70158;
	constexpr double c3 = c1 + 1.0;

	return 1 + c3 * pow(x - 1.0, 3.0) + c1 * pow(x - 1.0, 2.0);
}

static double inOutBack(double x)
{
	constexpr double c1 = 1.70158;
	constexpr double c2 = c1 * 1.525;

	return x < 0.5
		? (pow(2.0 * x, 2.0) * ((c2 + 1.0) * 2.0 * x - c2)) / 2.0
		: (pow(2.0 * x - 2.0, 2.0) * ((c2 + 1.0) * (x * 2.0 - 2.0) + c2) + 2.0) / 2.0;
}

// Elastic
static double inElastic(double x)
{
	constexpr double c4 = (2 * M_PI) / 3;

	return x == 0.0 ? 0.0 : (x == 1.0 ? 1.0 : -pow(2.0, 10.0 * x - 10.0) * sin((x * 10.0 - 10.75) * c4));
}

static double outElastic(double x)
{
	constexpr double c4 = (2 * M_PI) / 3;

	return x == 0.0 ? 0.0 : (x == 1.0 ? 1.0 : pow(2.0, -10.0 * x) * sin((x * 10.0 - 0.75) * c4) + 1.0);
}

static double inOutElastic(double x)
{
	constexpr double c5 = (2 * M_PI) / 4.5;

	return x == 0.0 ? 0.0 : (x == 1.0 ? 1.0 : x < 0.5
				? -(pow(2.0, 20.0 * x - 10.0) * sin((20.0 * x - 11.125) * c5)) / 2.0
				: (pow(2.0, -20.0 * x + 10.0) * sin((20.0 * x - 11.125) * c5)) / 2.0 + 1.0);
}

// Bounce
static double outBounce(double x)
{
	constexpr double n1 = 7.5625;
	constexpr double d1 = 2.75;

	if (x < 1.0 / d1) 
		return n1 * x * x;
	else if (x < 2.0 / d1) 
		return n1 * (x -= 1.5 / d1) * x + 0.75;
	else if (x < 2.5 / d1) 
		return n1 * (x -= 2.25 / d1) * x + 0.9375;
	else 
		return n1 * (x -= 2.625 / d1) * x + 0.984375;
}

static double inBounce(double x)
{
	return 1.0 - outBounce(1.0 - x);
}

static double inOutBounce(double x)
{
	return x < 0.5 ? (1.0 - outBounce(1.0 - 2.0 * x)) / 2.0 : (1.0 + outBounce(2.0 * x - 1.0)) / 2.0;
}