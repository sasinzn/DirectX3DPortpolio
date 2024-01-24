#include "Framework.h"
#include "AStarScene.h"

AStarScene::AStarScene()
{
    terrain = new Terrain();
    terrain->SetHeightMap(L"Textures/HeightMaps/CarMap.png");
    terrain->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Dirt2.png");
    terrain->SetAlphaMap(L"Textures/HeightMaps/TestAlpha.png");
    terrain->SetSecondMap(L"Textures/Landscape/Stones.png");

    aStar = new AStar();
    aStar->SetNode(terrain);

    fox = new Fox();
    fox->SetTerrain(terrain);
    fox->SetAStar(aStar);
}

AStarScene::~AStarScene()
{
    delete terrain;
    delete aStar;
    delete fox;
}

void AStarScene::Update()
{
    aStar->Update();
    fox->Update();
}

void AStarScene::PreRender()
{
}

void AStarScene::Render()
{
    terrain->Render();
    aStar->Render();
    fox->Render();
}

void AStarScene::PostRender()
{
}

void AStarScene::GUIRender()
{
}
