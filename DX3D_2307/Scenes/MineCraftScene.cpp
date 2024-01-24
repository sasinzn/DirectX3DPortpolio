#include "Framework.h"
#include "MineCraftScene.h"

MineCraftScene::MineCraftScene()
{
    BoxManager::Get()->CreateFloor(10, 2, 10);

    player = new Player();
    player->SetLocalPosition(5, 5, 5);

    CAM->SetTarget(player);
    CAM->TargetOptionLoad("MineCraft");
}

MineCraftScene::~MineCraftScene()
{
    BoxManager::Delete();

    delete player;
}

void MineCraftScene::Update()
{
    BoxManager::Get()->Update();

    player->Update();
}

void MineCraftScene::PreRender()
{
}

void MineCraftScene::Render()
{
    BoxManager::Get()->Render();

    player->Render();
}

void MineCraftScene::PostRender()
{
    player->PostRender();
}

void MineCraftScene::GUIRender()
{
    player->GUIRender();
}
