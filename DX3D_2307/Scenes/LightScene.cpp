#include "Framework.h"
#include "LightScene.h"

LightScene::LightScene()
{
    skyBox = new SkyBox(L"Textures/Landscape/Snow_ENV.dds");

    CreateObjects();    
}

LightScene::~LightScene()
{
    delete skyBox;

    delete quad;
    delete sphere;
    delete model;
}

void LightScene::Update()
{
    quad->UpdateWorld();
    sphere->UpdateWorld();
    model->UpdateWorld();
    bot->Update();

    //skyBox->SetLocalPosition(CAM->GetLocalPosition());
    //skyBox->UpdateWorld();
}

void LightScene::PreRender()
{
}

void LightScene::Render()
{
    skyBox->Render();

    quad->Render();

    sphere->Render();
    model->Render();
    bot->Render();    
}

void LightScene::PostRender()
{
}

void LightScene::GUIRender()
{
    quad->GUIRender();

    sphere->GUIRender();
    sphere->GetMaterial()->GUIRender();
    model->GUIRender();
    bot->GUIRender();
}

void LightScene::CreateObjects()
{
    quad = new Quad();
    quad->SetTag("Quad");
    quad->Load();    

    sphere = new Sphere();    
    sphere->SetTag("Sphere");
    sphere->Load();    
    sphere->GetMaterial()->Load("TextData/Materials/FieldStone.mat");

    model = new Model("Bunny");
    model->Load();    

    bot = new Bot();
    bot->Load();
}
