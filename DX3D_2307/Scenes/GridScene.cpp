#include "Framework.h"
#include "GridScene.h"

GridScene::GridScene() : width(100), height(100)
{
    material = new Material(L"Basic/Grid.hlsl");
    mesh = new Mesh<VertexColor>();
    MakeMesh();
    mesh->CreateMesh();

    worldBuffer = new MatrixBuffer();
}

GridScene::~GridScene()
{
    delete material;
    delete mesh;
    delete worldBuffer;
}

void GridScene::Update()
{
}

void GridScene::PreRender()
{
}

void GridScene::Render()
{
    worldBuffer->SetVS(0);

    material->Set();
    mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void GridScene::PostRender()
{
}

void GridScene::GUIRender()
{
}

void GridScene::MakeMesh()
{
    vector<VertexColor>& vertices = mesh->GetVertices();
    //Axis X
    vertices.emplace_back(-width, 0, 0, 1, 0, 0);
    vertices.emplace_back(+width, 0, 0, 1, 0, 0);

    //Axis Y
    vertices.emplace_back(0, -width, 0, 0, 1, 0);
    vertices.emplace_back(0, +width, 0, 0, 1, 0);

    //Axis Z
    vertices.emplace_back(0, 0, -width, 0, 0, 1);
    vertices.emplace_back(0, 0, +width, 0, 0, 1);

    int halfW = width / 2;
    int halfH = height / 2;

    for (int x = -halfW; x <= halfW; x++)
    {
        if (x == 0) continue;

        vertices.emplace_back(x, 0, -halfH, 0.5f, 0.5f, 0.5f);
        vertices.emplace_back(x, 0, +halfH, 0.5f, 0.5f, 0.5f);
    }

    for (int z = -halfH; z <= halfH; z++)
    {
        if (z == 0) continue;

        vertices.emplace_back(-halfW, 0, z, 0.5f, 0.5f, 0.5f);
        vertices.emplace_back(+halfW, 0, z, 0.5f, 0.5f, 0.5f);
    }
}
