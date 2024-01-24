#include "Framework.h"

Terrain::Terrain()
    : GameObject(L"Landscape/Terrain.hlsl"), width(10), height(10)
{
    tag = "Terrain";

    mesh = new Mesh<VertexType>();    
}

Terrain::~Terrain()
{
    delete mesh;
}

void Terrain::Render()
{
    alphaMap->SetPS(10);
    secondMap->SetPS(11);

    SetRender();
    mesh->Draw();
}

void Terrain::SetHeightMap(wstring file)
{
    heightMap = Texture::Add(file);

    MakeMesh();
    MakeNormal();
    mesh->CreateMesh();
}

void Terrain::SetAlphaMap(wstring file)
{
    alphaMap = Texture::Add(file);
}

void Terrain::SetSecondMap(wstring file)
{
    secondMap = Texture::Add(file);
}

float Terrain::GetHeight(const Vector3& pos, Vector3* normal)
{
    int x = (int)pos.x;    
    int z = (int)(height - pos.z - 1);

    if (x < 0 || x >= width - 1) return 0.0f;
    if (z < 0 || z >= height - 1) return 0.0f;

    UINT index[4];
    index[0] = width * (z + 1) + x;
    index[1] = width * z + x;
    index[2] = width * (z + 1) + x + 1;
    index[3] = width * z + x + 1;

    vector<VertexType>& vertices = mesh->GetVertices();

    Vector3 p[4];
    FOR(4)
        p[i] = vertices[index[i]].pos;

    float u = pos.x - p[0].x;
    float v = pos.z - p[0].z;

    Vector3 result;

    if (u + v <= 1.0f)
    {
        result = ((p[2] - p[0]) * u + (p[1] - p[0]) * v) + p[0];

        if (normal)
            (*normal) = GetNormalFromPolygon(p[0], p[1], p[2]);
    }
    else
    {
        u = 1.0f - u;
        v = 1.0f - v;
        
        result = ((p[1] - p[3]) * u + (p[2] - p[3]) * v) + p[3];

        if (normal)
            (*normal) = GetNormalFromPolygon(p[2], p[1], p[3]);
    }

    return result.y;
}

bool Terrain::Picking(IN Ray ray, OUT Vector3& pos)
{
    for (UINT z = 0; z < height - 1; z++)
    {
        for (UINT x = 0; x < width - 1; x++)
        {
            UINT index[4];
            index[0] = width * z + x;
            index[1] = width * z + x + 1;
            index[2] = width * (z + 1) + x;
            index[3] = width * (z + 1) + x + 1;

            vector<VertexType>& vertices = mesh->GetVertices();

            Vector3 p[4];
            FOR(4)
                p[i] = vertices[index[i]].pos;

            float distance = 0.0f;
            if (Intersects(ray.origin, ray.direction, p[0], p[1], p[2], distance))
            {
                pos = ray.origin + ray.direction * distance;
                return true;
            }
            if (Intersects(ray.origin, ray.direction, p[3], p[1], p[2], distance))
            {
                pos = ray.origin + ray.direction * distance;
                return true;
            }
        }
    }

    return false;
}

void Terrain::MakeMesh()
{
    width = (UINT)heightMap->GetSize().x;
    height = (UINT)heightMap->GetSize().y;

    vector<Float4> pixels;
    heightMap->ReadPixels(pixels);

    //Vertices
    vector<VertexType>& vertices = mesh->GetVertices();

    vertices.reserve(width * height);
    for (UINT z = 0; z < height; z++)
    {
        for (UINT x = 0; x < width; x++)
        {
            VertexType vertex;
            vertex.pos = Vector3(x, 0.0f, height - z - 1);
            //vertex.pos = Vector3(x, 0.0f, z);
            vertex.uv.x = x / (float)(width - 1);
            vertex.uv.y = z / (float)(height - 1);

            UINT index = width * z + x;
            vertex.pos.y = pixels[index].x * MAX_HEIGHT;

            vertices.push_back(vertex);
        }
    }

    //Indices
    vector<UINT>& indices = mesh->GetIndices();

    indices.reserve((width - 1) * (height - 1) * 6);
    for (UINT z = 0; z < height - 1; z++)
    {
        for (UINT x = 0; x < width - 1; x++)
        {
            indices.push_back(width * z + x);//0                
            indices.push_back(width * z + x + 1);//2                        
            indices.push_back(width * (z + 1) + x);//1

            indices.push_back(width * (z + 1) + x);//1            
            indices.push_back(width * z + x + 1);//2                        
            indices.push_back(width * (z + 1) + x + 1);//3
        }
    }
}

void Terrain::MakeNormal()
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
