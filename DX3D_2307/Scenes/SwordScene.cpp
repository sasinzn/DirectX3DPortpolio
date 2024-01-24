#include "Framework.h"
#include "SwordScene.h"

SwordScene::SwordScene()
{
    bot = new Bot();
    terrain = new Terrain();
    terrain->SetHeightMap(L"Textures/HeightMaps/CarMap.png");    
    terrain->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Dirt2.png");
    terrain->SetAlphaMap(L"Textures/HeightMaps/TestAlpha.png");
    terrain->SetSecondMap(L"Textures/Landscape/Stones.png");

    bot->SetTerrain(terrain);

    //CAM->SetTarget(bot);
    //CAM->TargetOptionLoad("Bot");

    DataManager::Get();

    store = new Quad(L"Textures/Test.jpg");
    store->SetTag("Store");
    store->Load();

    inventory = new Quad(L"Textures/UI/items.png");
    inventory->SetTag("Inventory");
    inventory->Load();

    storeBtn = new Button(L"Textures/UI/StoreIcon.png");
    storeBtn->SetTag("StoreBtn");
    storeBtn->Load();
    storeBtn->SetParamEvent(bind(&SwordScene::ClickUIIcon, this, placeholders::_1));
    storeBtn->SetObejct(store);

    invenBtn = new Button(L"Textures/UI/InventoryIcon.png");    
    invenBtn->SetTag("InvenBtn");
    invenBtn->Load();
    invenBtn->SetParamEvent(bind(&SwordScene::ClickUIIcon, this, placeholders::_1));
    invenBtn->SetObejct(inventory);    
}

SwordScene::~SwordScene()
{
    delete bot;
    delete terrain;

    delete storeBtn;
    delete invenBtn;
}

void SwordScene::Update()
{
    bot->Update();

    storeBtn->Update();
    invenBtn->Update();

    store->UpdateWorld();
    inventory->UpdateWorld();
}

void SwordScene::PreRender()
{
}

void SwordScene::Render()
{
    bot->Render();
    terrain->Render();
}

void SwordScene::PostRender()
{
    bot->PostRender();

    storeBtn->Render();
    invenBtn->Render();

    store->Render();
    inventory->Render();
}

void SwordScene::GUIRender()
{
    bot->GUIRender();

    store->GUIRender();
    inventory->GUIRender();
}

void SwordScene::ClickUIIcon(void* panel)
{
    Quad* quad = (Quad*)panel;

    if (quad->Active())
        quad->SetActive(false);
    else
        quad->SetActive(true);
}
