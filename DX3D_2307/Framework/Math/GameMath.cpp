#include "Framework.h"

int GameMath::Random(const int& min, const int& max)
{
    return rand() % (max - min) + min;
}

float GameMath::Random(const float& min, const float& max)
{
    float normal = rand() / (float)RAND_MAX;

    return (max - min) * normal + min;
}

Vector3 GameMath::Random(const Vector3& min, const Vector3& max)
{
    return Vector3(Random(min.x, max.x), Random(min.y, max.y), Random(min.z, max.z));
}

bool GameMath::NearlyEqual(const float& s1, const float& s2)
{
    return abs(s1 - s2) < FLT_EPSILON;
}

Vector3 GameMath::Cross(Vector3& vec1, Vector3& vec2)
{
    return XMVector3Cross(vec1, vec2);
}

float GameMath::Dot(Vector3& vec1, Vector3& vec2)
{
    return XMVectorGetX(XMVector3Dot(vec1, vec2));
}

float GameMath::Distance(const Vector3& vec1, const Vector3& vec2)
{
    return (vec1 - vec2).Length();
}

float GameMath::Clamp(const float& min, const float& max, const float& value)
{
    if (value < min)
        return min;

    if (value > max)
        return max;

    return value;
}

float GameMath::Lerp(const float& start, const float& end, float t)
{
    t = Clamp(0.0f, 1.0f, t);

    return start + (end - start) * t;
}

Vector3 GameMath::Lerp(const Vector3& start, const Vector3& end, float t)
{
    t = Clamp(0.0f, 1.0f, t);

    return start + (end - start) * t;
}

Matrix GameMath::Lerp(const Matrix& start, const Matrix& end, float t)
{
    t = Clamp(0.0f, 1.0f, t);

    return start + (end - start) * t;
}

Vector3 GameMath::GetRotFromNormal(Vector3& normal)
{
    Vector3 yAxis(0, 1, 0);

    float angle = Dot(normal, yAxis);
    Vector3 axis = Cross(normal, yAxis);
    if (axis == Vector3::Zero())
        return axis;

    return XMQuaternionRotationAxis(axis.GetNormalized(), angle) * -1.0f;
}

Vector3 GameMath::GetNormalFromPolygon(const Vector3& v0, const Vector3& v1, const Vector3& v2)
{
    Vector3 e0 = v1 - v0;
    Vector3 e1 = v2 - v0;

    return Cross(e0, e1).GetNormalized();
}

Vector3 GameMath::ClosestPointOnLine(const Vector3& start, const Vector3& end, const Vector3& point)
{
    Vector3 line = end - start;
    Vector3 A = point - start;

    float x = Dot(line, A);
    float y = Dot(line, line);

    float t = Clamp(0, 1, x / y);

    return start + line * t;
}
