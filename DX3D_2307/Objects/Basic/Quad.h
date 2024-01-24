#pragma once

class Quad : public GameObject
{
private:
    typedef VertexUVNormalTangent VertexType;

public:
    Quad(Float2 size = { 1, 1 });
    Quad(wstring textureFile);
    ~Quad();

    void Render();
    void SetRender();
    void RenderInstanced(UINT instanceCount);

    Float2 GetSize() 
    { 
        return  { GetGlobalScale().x * size.x, GetGlobalScale().y * size.y };
    }

private:
    void MakeMesh();   
    void MakeNormal();
    void MakeTangent();

private:
    Float2 size;
    Mesh<VertexType>* mesh;
};