#pragma once

namespace GameMath
{
    int Random(const int& min, const int& max);
    float Random(const float& min, const float& max);
    Vector3 Random(const Vector3& min, const Vector3& max);

    bool NearlyEqual(const float& s1, const float& s2);

    Vector3 Cross(Vector3& vec1, Vector3& vec2);
    float Dot(Vector3& vec1, Vector3& vec2);

    float Distance(const Vector3& vec1, const Vector3& vec2);

    float Clamp(const float& min, const float& max, const float& value);

    float Lerp(const float& start, const float& end, float t);
    Vector3 Lerp(const Vector3& start, const Vector3& end, float t);
    Matrix Lerp(const Matrix& start, const Matrix& end, float t);

    Vector3 GetRotFromNormal(Vector3& normal);
    Vector3 GetNormalFromPolygon(const Vector3& v0, const Vector3& v1, const Vector3& v2);

    Vector3 ClosestPointOnLine(const Vector3& start, const Vector3& end, const Vector3& point);
}