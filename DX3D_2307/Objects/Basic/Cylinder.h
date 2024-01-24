#pragma once

class Cylinder : public GameObject
{
private:
    typedef VertexUVNormalTangent VertexType;

public:
    Cylinder(float radius = 1.0f, float height = 2.0f, UINT sliceCount = 32);
    ~Cylinder();

    void Render();

private:
    void MakeMesh();
    void MakeNormal();
    void MakeTangent();

private:
    Mesh<VertexType>* mesh;

    float radius, height;
    UINT sliceCount;
};