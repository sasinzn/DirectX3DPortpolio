#pragma once

class ModelMesh
{
public:
    ModelMesh(string name);
    ~ModelMesh();

    void Render();
    void RenderInstanced(UINT instanceCount);

    void CreateMesh(void* vertices, UINT vertexCount,
        void* indices, UINT indexCount);

    void SetBox(Vector3& minBox, Vector3& maxBox);

    void SetMaterial(Material* material) { this->material = material; }

    MatrixBuffer* GetBone() { return boneBuffer; }
    string GetName() { return name; }
private:
    string name;

    Material* material;
    Mesh<ModelVertex>* mesh;

    MatrixBuffer* boneBuffer;
};