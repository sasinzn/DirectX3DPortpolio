#pragma once

class Cube : public GameObject
{
private:
    typedef VertexUVNormalTangent VertexType;

public:
    Cube(Vector3 size = Vector3(1, 1, 1));
    ~Cube();

    void Render();

private:
    void MakeMesh();
    void MakeNormal();
    void MakeTangent();

private:   
    Vector3 size;

    Mesh<VertexType>* mesh;
};