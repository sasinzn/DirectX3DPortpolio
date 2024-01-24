#include "Framework.h"
#include "BuildTestScene.h"

BuildTestScene::BuildTestScene()
{
    terrain = new Terrain();
    terrain->SetHeightMap(L"Textures/Solid/Black.png");
    terrain->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Dirt2.png");
    terrain->SetAlphaMap(L"Textures/HeightMaps/TestAlpha.png");
    terrain->SetSecondMap(L"Textures/Landscape/Stones.png");

    astar = new AStar(40,40);
    astar->SetNode(terrain);

    WallManager::Get()->SetAStar(astar);
    WallManager::Get()->SetTerrain(terrain);
    WallManager::Get()->Create(10);
    CAM->SetTopView();
}

BuildTestScene::~BuildTestScene()
{
    delete terrain;
    delete astar;
}

void BuildTestScene::Update()
{
    astar->Update();
    WallManager::Get()->Update();

    if (KEY->Down(VK_LBUTTON))
    {
        WallManager::Get()->BuildMode();
    }

    
}

void BuildTestScene::PreRender()
{
}

void BuildTestScene::Render()
{
    terrain->Render();
    WallManager::Get()->Render();
}

void BuildTestScene::PostRender()
{
}

void BuildTestScene::GUIRender()
{
    WallManager::Get()->GUIRender();
}
