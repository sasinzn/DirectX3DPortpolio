#include "Framework.h"
#include "TerrainScene.h"

TerrainScene::TerrainScene()
{
    terrain = new Terrain();
    terrain->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Dirt2.png");
    //terrain->GetMaterial()->SetDiffuseMap(L"Textures/Solid/White.png");
    //terrain->SetHeightMap(L"Textures/HeightMaps/ColorMap256.png");
    terrain->SetHeightMap(L"Textures/HeightMaps/MyMap.png");
    terrain->SetAlphaMap(L"Textures/HeightMaps/TestAlpha.png");
    terrain->SetSecondMap(L"Textures/Landscape/Stones.png");

    //terrain = new Terrain("Stage1.map");

    terrain->SetPivot({ 128, 0, 128 });

    block = new Block({10, 10, 5});
    block->Load("TextData/Spider.tex");
    block->SetLocalPosition(30, 0, 30);

    sphere = new Sphere(10);
    sphere->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Fieldstone_DM.tga");
    sphere->GetMaterial()->SetSpecularMap(L"Textures/Landscape/Fieldstone_SM.tga");
    sphere->GetMaterial()->SetNormalMap(L"Textures/Landscape/Fieldstone_NM.tga");
    sphere->GetMaterial()->SetShader(L"Light/NormalMapping.hlsl");
    sphere->Translate(Vector3::Up() * 10);
}

TerrainScene::~TerrainScene()
{
    delete terrain;
    delete block;
    delete sphere;
}

void TerrainScene::Update()
{
    if (KEY->Press('W'))
        block->Translate(block->GetBack() * 10 * DELTA);

    if (KEY->Press('A'))
        block->Rotate(Vector3::Up() * DELTA);
    if (KEY->Press('D'))
        block->Rotate(Vector3::Down() * DELTA);

    Vector3 blockPos = block->GetLocalPosition();
    blockPos.y = terrain->GetHeight(blockPos) + 5.0f;
    block->SetLocalPosition(blockPos);

    block->Update();

    terrain->UpdateWorld();

    sphere->Rotate(Vector3::Up() * DELTA);
    sphere->UpdateWorld();
}

void TerrainScene::PreRender()
{
}

void TerrainScene::Render()
{
    terrain->Render();
    block->Render();
    sphere->Render();
}

void TerrainScene::PostRender()
{
}

void TerrainScene::GUIRender()
{
    terrain->GUIRender();
    sphere->GUIRender();
    sphere->GetMaterial()->GUIRender();
}
