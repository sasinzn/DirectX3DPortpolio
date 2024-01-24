#include "Framework.h"

Lay::Lay()
{
    material = new Material(L"Basic/Grid.hlsl");
    mesh = new Mesh<VertexColor>();
    

    worldBuffer = new MatrixBuffer();
}

Lay::~Lay()
{
    delete material;
    delete mesh;
    delete worldBuffer;
}

void Lay::Update()
{
    UpdateWorld();
    
}

void Lay::Render()
{
    worldBuffer->SetVS(0);

    material->Set();
    mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void Lay::MakeMesh()
{
    vector<VertexColor>& vertices = mesh->GetVertices();
    //target Lay
    vertices.emplace_back(GetLocalPosition().x, GetLocalPosition().y, GetLocalPosition().z, 1, 0, 0);
    vertices.emplace_back(target->GetLocalPosition().x, target->GetLocalPosition().y, target->GetLocalPosition().z, 1, 0, 0);
}
