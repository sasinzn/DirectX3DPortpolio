#include "Framework.h"

ModelMesh::ModelMesh(string name) : name(name)
{
    boneBuffer = new MatrixBuffer();
}

ModelMesh::~ModelMesh()
{
    delete mesh;
    delete boneBuffer;
}

void ModelMesh::Render()
{
    boneBuffer->SetVS(10);

    material->Set();
    mesh->Draw();
}

void ModelMesh::RenderInstanced(UINT instanceCount)
{
    material->Set();
    mesh->DrawInstanced(instanceCount);
}

void ModelMesh::CreateMesh(void* vertices, UINT vertexCount, void* indices, UINT indexCount)
{
    mesh = new Mesh<ModelVertex>();

    mesh->GetVertices().resize(vertexCount);
    memcpy(mesh->GetVertices().data(), vertices, sizeof(ModelVertex) * vertexCount);

    mesh->GetIndices().resize(indexCount);
    memcpy(mesh->GetIndices().data(), indices, sizeof(UINT) * indexCount);

    mesh->CreateMesh();
}

void ModelMesh::SetBox(Vector3& minBox, Vector3& maxBox)
{
    vector<ModelVertex>& vertices = mesh->GetVertices();

    if (vertices.empty()) return;

    Vector3 minPos = vertices[0].pos;
    Vector3 maxPos = vertices[0].pos;

    for (UINT i = 1; i < vertices.size(); i++)
    {
        minPos.x = min(minPos.x, vertices[i].pos.x);
        minPos.y = min(minPos.y, vertices[i].pos.y);
        minPos.z = min(minPos.z, vertices[i].pos.z);

        maxPos.x = max(maxPos.x, vertices[i].pos.x);
        maxPos.y = max(maxPos.y, vertices[i].pos.y);
        maxPos.z = max(maxPos.z, vertices[i].pos.z);
    }

    minBox = minPos;
    maxBox = maxPos;
}
