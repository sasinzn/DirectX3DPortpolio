#include "Framework.h"
#include "Mission1Scene.h"

Mission1Scene::Mission1Scene()
{
    skyBox = new SkyBox(L"Textures/Landscape/Snow_ENV.dds");
    player = new Player();
    terrain = new Terrain();
    terrain->SetHeightMap(L"Textures/Solid/Black.png");    
    terrain->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Dirt2.png");
    terrain->SetAlphaMap(L"Textures/HeightMaps/TestAlpha.png");
    terrain->SetSecondMap(L"Textures/Landscape/Stones.png");

    aStar = new AStar(100,100);
    aStar->SetNode(terrain);
   
    player->SetTerrain(terrain);

    EnemyManager::Get()->SetTerrain(terrain);
    EnemyManager::Get()->SetAStar(aStar);
    EnemyManager::Get()->CreateEnemies(50);

    WallManager::Get()->SetAStar(aStar);
    WallManager::Get()->SetTerrain(terrain);
    WallManager::Get()->Create(100);

    CoinManager::Get()->Create(50);
    TowerManager::Get()->Create(50);

    CreateDest();
}

Mission1Scene::~Mission1Scene()
{
    if (!Active()) return;

    delete skyBox;
    delete player;
    delete terrain;
    EnemyManager::Delete();
    CoinManager::Delete();
    TowerManager::Delete();
    delete aStar;

}

void Mission1Scene::Update()
{
    if (!Active()) return;

    player->Update();
    EnemyManager::Get()->Update();
    EnemyManager::Get()->Collision();
    CoinManager::Get()->Update();
    TowerManager::Get()->Update();
    //건물 건설 모드 해제
    if (KEY->Down(VK_ESCAPE))
    {
        SetCamera();
        player->BuildModeOff();
        WallManager::Get()->EndBuildMode();
    }
    //건물 건설 모드
    if (KEY->Down('B'))
    {
        player->BuildModeOn();
        CAM->SetTopView();
        CAM->SetTarget(nullptr);

    }

    aStar->Update();


    WallManager::Get()->Update();

    if (KEY->Down(VK_LBUTTON) && player->isBuildMode())
    {
        WallManager::Get()->BuildMode();
    }

    if (TowerManager::Get()->UseCoinCheck())
    {
        player->UseCoin();
        TowerManager::Get()->UseCoinFinish();
    }

    //타워 건설 모드
    if (KEY->Down('T'))
    {
        if (player->GetCoin() > 0)
        {
            TowerManager::Get()->BuildMode();
        }
        else
            player->CoinEmpty();
    }

    //skyBox->SetLocalPosition(CAM->GetLocalPosition());
    //skyBox->UpdateWorld();
}

void Mission1Scene::PreRender()
{
}

void Mission1Scene::Render()
{
    if (!Active()) return;
    skyBox->Render();
    player->Render();
    terrain->Render();
    EnemyManager::Get()->Render();
    WallManager::Get()->Render();
    CoinManager::Get()->Render();
    TowerManager::Get()->Render();
    //aStar->Render();

    FOR(4)
        dest[i]->Render();
}

void Mission1Scene::PostRender()
{
    if (!Active()) return;

    player->PostRender();
    EnemyManager::Get()->PostRender();

    FOR(4)
    {
        Vector3 worldPos = dest[i]->GetLocalPosition() + Vector3::Up() * 3.0f;
        Vector3 screenPos = CAM->WorldToScreen(worldPos);
        if (CAM->ContainPoint(worldPos) == false) continue;
        Font::Get()->RenderText(to_string(i+1), {screenPos.x, screenPos.y});
    }

}

void Mission1Scene::GUIRender()
{
    player->GUIRender();
    EnemyManager::Get()->GUIRender();

    FOR(4)
        dest[i]->GUIRender();
}

void Mission1Scene::SetCamera()
{
    CAM->SetTarget(player);
    CAM->TargetOptionLoad("camera3rd");
    CAM->Load("camera3rd");

}

void Mission1Scene::CreateDest()
{
    FOR(4)
    {
        dest[i] = new Quad(Float2(5, 5));
        dest[i]->GetMaterial()->SetDiffuseMap(L"Textures/LandScape/Floor.png");
        dest[i]->GetMaterial()->SetNormalMap(L"Textures/LandScape/Floor_normal.png");
        dest[i]->SetTag("dest" + to_string(i));
        dest[i]->Load();
    }

    dest[0]->SetLocalPosition(10, 0.1f, 10);
    dest[1]->SetLocalPosition(100, 0.1f, 10);
    dest[2]->SetLocalPosition(100, 0.1f, 100);
    dest[3]->SetLocalPosition(10, 0.1f, 100);
   
    FOR(4)
    {
        dest[i]->UpdateWorld();
    }
    
}

void Mission1Scene::ClickUIIcon(void* panel)
{
    Quad* quad = (Quad*)panel;

    if (quad->Active())
        quad->SetActive(false);
    else
        quad->SetActive(true);
}
