#include "Framework.h"

Quad::Quad(Float2 size) : size(size)
{
    mesh = new Mesh<VertexType>();
    MakeMesh();
    MakeNormal();
    MakeTangent();
    mesh->CreateMesh();
}

Quad::Quad(wstring textureFile)
{
    material->SetDiffuseMap(textureFile);
    material->SetShader(L"Basic/Texture.hlsl");
    size = material->GetDiffuseMap()->GetSize();

    mesh = new Mesh<VertexType>();
    MakeMesh();
    mesh->CreateMesh();
}

Quad::~Quad()
{
    delete mesh;
}

void Quad::Render()
{
    if (!isActive) return;

    SetRender();
    mesh->Draw();
}

void Quad::SetRender()
{
    mesh->GetVertexBuffer()->Set();
    mesh->GetIndexBuffer()->Set();

    GameObject::SetRender();
}

void Quad::RenderInstanced(UINT instanceCount)
{
    SetRender();
    mesh->DrawInstanced(instanceCount);
}

void Quad::MakeMesh()
{
    vector<VertexType>& vertices = mesh->GetVertices();

    float left = -size.x * 0.5f;
    float right = +size.x * 0.5f;
    float top = +size.y * 0.5f;
    float bottom = -size.y * 0.5f;

    vertices.emplace_back(left, top, 0, 0, 0);
    vertices.emplace_back(right, top, 0, 1, 0);
    vertices.emplace_back(left, bottom, 0, 0, 1);
    vertices.emplace_back(right, bottom, 0, 1, 1);

    vector<UINT>& indices = mesh->GetIndices();
    indices = { 0, 1, 2, 2, 1, 3 };
}

void Quad::MakeNormal()
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

void Quad::MakeTangent()
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