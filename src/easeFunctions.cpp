#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

enum class movement_type : int {
    LINEAR,
    IN_POW,
    OUT_POW,
    IN_OUT_POW,
    IN_SIN,
    OUT_SIN,
    IN_OUT_SIN,
    IN_EXP,
    OUT_EXP,
    IN_OUT_EXP,
    IN_CIRC,
    OUT_CIRC,
    IN_OUT_CIRC,
    REBOUND,
    IN_BACK,
    OUT_BACK,
    IN_OUT_BACK
};

static float linear(float x, float u)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return x;
}

static float inPow(float x, float p)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return pow(x, p);
}

static float outPow(float x, float p)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;

    int sign = static_cast<int>(p) % 2 == 0 ? -1 : 1;
    return (sign * (pow(x - 1.f, p) + sign));
}

static float inOutPow(float x, float p)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;

    x *= 2;
    if (x < 1) return inPow(x, p) / 2;

    int sign = static_cast<int>(p) % 2 == 0 ? -1 : 1;
    return (sign / 2.0f * (pow(x - 2.f, p) + sign * 2.f));
}

static float inSin(float x, float u)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return -cos(x * (M_PI / 2.0f)) + 1;
}

static float outSin(float x, float u)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return sin(x * (M_PI / 2.0f));
}

static float inOutSin(float x, float u)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return -0.5f * (cos(M_PI * x) - 1.0f);
}

static float inExp(float x, float u)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return pow(2.0f, 10.0f * (x - 1.0f));
}

static float outExp(float x, float u)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return -pow(2, -10 * x) + 1;
}

static float inOutExp(float x, float u)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return x < 0.5f ? 0.5f * pow(2.f, 10.f * (2.f * x - 1.f)) :
        0.5f * (-pow(2.f, 10.f * (-2.f * x + 1.f)) + 2.f);
}

static float inCirc(float x, float u)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return -(sqrt(1.f - x * x) - 1.f);
}

static float outCirc(float x, float u)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return sqrt(1.f - (x - 1.f) * (x - 1.f));
}

static float inOutCirc(float x, float u)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return x < 1.f ? -0.5f * (sqrt(1.f - x * x) - 1.f) :
        0.5f * (sqrt(1.f - ((2.f * x) - 2.f) * ((2.f * x) - 2.f)) + 1.f);
}

static float rebound(float x, float u)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;

    if (x < (1.f / 2.75f)) return 1.f - 7.5625f * x * x;
    else if (x < (2.f / 2.75f)) return 1.f - (7.5625f * (x - 1.5f / 2.75f) *
        (x - 1.5f / 2.75f) + 0.75f);
    else if (x < (2.5f / 2.75f)) return 1.f - (7.5625f * (x - 2.25f / 2.75f) *
        (x - 2.25f / 2.75f) + 0.9375f);
    else return 1.f - (7.5625f * (x - 2.625f / 2.75f) * (x - 2.625f / 2.75f) +
        0.984375f);
}

static float inBack(float x, float u)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return x * x * ((1.70158f + 1.f) * x - 1.70158f);
}

static float outBack(float x, float u)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return (x - 1.f) * (x - 1.f) * ((1.70158f + 1.f) * (x - 1.f) + 1.70158f) + 1.f;
}

static float inOutBack(float x, float u)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return x < 0.5f ? 0.5f * (4.f * x * x * ((2.5949f + 1.f) * 2.f * x - 2.5949f)) :
        0.5f * ((2.f * x - 2.f) * (2.f * x - 2.f) * ((2.5949f + 1.f) * (2.f * x - 2.f) +
            2.5949f) + 2.f);
}