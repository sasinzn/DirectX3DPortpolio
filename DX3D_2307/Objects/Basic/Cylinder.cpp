#include "Framework.h"

Cylinder::Cylinder(float radius, float height, UINT sliceCount)
    : GameObject(L"Light/NormalMapping.hlsl"),
    radius(radius), height(height), sliceCount(sliceCount)
{
    mesh = new Mesh<VertexType>();
    MakeMesh();
    MakeNormal();
    MakeTangent();
    mesh->CreateMesh();
}

Cylinder::~Cylinder()
{
    delete mesh;
}

void Cylinder::Render()
{
    if (!isActive) return;

    SetRender();
    mesh->Draw();
}

void Cylinder::MakeMesh()
{
    //Vertices
    vector<VertexType>& vertices = mesh->GetVertices();
    vertices.reserve((sliceCount + 1) * 4);

    float thetaStep = XM_2PI / sliceCount;

    //Top
    VertexType top;
    top.pos = { 0.0f, height * 0.5f, 0.0f };
    top.uv = { 0.5f, 0.5f };

    vertices.push_back(top);

    FOR(sliceCount + 1)
    {
        float theta = thetaStep * i;

        float x = cos(theta);
        float z = sin(theta);

        VertexType vertex;
        vertex.pos = { x * radius, height * 0.5f, z * radius };
        vertex.uv = { (x + 1) / 2, (z + 1) / 2 };

        vertices.push_back(vertex);
    }

    //Bottom
    VertexType bottom;
    bottom.pos = { 0, -height * 0.5f, 0 };
    bottom.uv = { 0.5f, 0.5f };

    vertices.push_back(bottom);
    FOR(sliceCount + 1)
    {
        float theta = thetaStep * i;

        float x = cos(theta);
        float z = sin(theta);

        VertexType vertex;
        vertex.pos = { x * radius, -height * 0.5f, z * radius };
        vertex.uv = { (x + 1) / 2, (z + 1) / 2 };

        vertices.push_back(vertex);
    }

    //Side
    FOR(sliceCount + 1)
    {
        float theta = thetaStep * i;

        float x = cos(theta);
        float z = sin(theta);

        VertexType vertex;
        vertex.pos = { x * radius, -height * 0.5f, z * radius };
        vertex.uv = { (float)i / (float)sliceCount, 1.0f};
        vertices.push_back(vertex);

        vertex.pos = { x * radius, +height * 0.5f, z * radius };
        vertex.uv = { (float)i / (float)sliceCount, 0.0f };
        vertices.push_back(vertex);
    }

    //Indices
    vector<UINT>& indices = mesh->GetIndices();

    indices.reserve(sliceCount * 6);

    //TopCircle
    FOR(sliceCount)
    {
        indices.push_back(0);
        indices.push_back(i + 2);
        indices.push_back(i + 1);
    }

    //BottomCircle
    UINT bottomIndex = sliceCount + 2;
    FOR(sliceCount)
    {
        indices.push_back(bottomIndex);
        indices.push_back(bottomIndex + i + 1);
        indices.push_back(bottomIndex + i + 2);
    }

    //Side
    UINT sideIndex = (sliceCount + 2) * 2;
    FOR(sliceCount)
    {
        indices.push_back(sideIndex + i * 2 + 0);//0
        indices.push_back(sideIndex + i * 2 + 1);//1
        indices.push_back(sideIndex + i * 2 + 2);//2

        indices.push_back(sideIndex + i * 2 + 2);//2
        indices.push_back(sideIndex + i * 2 + 1);//1
        indices.push_back(sideIndex + i * 2 + 3);//2
    }
}

void Cylinder::MakeNormal()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    vector<UINT>& indices = mesh->GetIndices();

    FOR(indices.size() / 3)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        Vector3 v0 = vertices[index0].pos;
        Vector3 v1 = vertices[index1].pos;
        Vector3 v2 = vertices[index2].pos;

        Vector3 e0 = v1 - v0;
        Vector3 e1 = v2 - v0;

        Vector3 normal = Cross(e0, e1).GetNormalized();

        vertices[index0].normal = normal + vertices[index0].normal;
        vertices[index1].normal = normal + vertices[index1].normal;
        vertices[index2].normal = normal + vertices[index2].normal;
    }
}

void Cylinder::MakeTangent()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    vector<UINT>& indices = mesh->GetIndices();

    FOR(indices.size() / 3)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        Vector3 p0 = vertices[index0].pos;
        Vector3 p1 = vertices[index1].pos;
        Vector3 p2 = vertices[index2].pos;

        Float2 uv0 = vertices[index0].uv;
        Float2 uv1 = vertices[index1].uv;
        Float2 uv2 = vertices[index2].uv;

        Vector3 e0 = p1 - p0;
        Vector3 e1 = p2 - p0;

        float u1 = uv1.x - uv0.x;
        float v1 = uv1.y - uv0.y;
        float u2 = uv2.x - uv0.x;
        float v2 = uv2.y - uv0.y;

        float d = 1.0f / (u1 * v2 - v1 * u2);
        Vector3 tangent = d * (e0 * v2 - e1 * v1);

        vertices[index0].tangent += tangent;
        vertices[index1].tangent += tangent;
        vertices[index2].tangent += tangent;
    }

    for (VertexType& vertex : vertices)
    {
        Vector3 t = vertex.tangent;
        Vector3 n = vertex.normal;

        vertex.tangent = (t - n * Dot(n, t)).GetNormalized();
    }
}

