#pragma once

class Terrain : public GameObject
{
private:
    typedef VertexUVNormal VertexType;
    const float MAX_HEIGHT = 20.0f;

public:
    Terrain();
    ~Terrain();

    void Render();

    void SetHeightMap(wstring file);
    void SetAlphaMap(wstring file);
    void SetSecondMap(wstring file);

    float GetHeight(const Vector3& pos, Vector3* normal = nullptr);
    bool Picking(IN Ray ray, OUT Vector3& pos);

    Float2 GetSize() { return Float2(width, height); }

private:
    void MakeMesh();
    void MakeNormal();

private:
    UINT width, height;

    Mesh<VertexType>* mesh;

    Texture* heightMap;
    Texture* alphaMap;

    Texture* secondMap;
};