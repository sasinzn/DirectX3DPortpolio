#include "Framework.h"
#include "RenderTargetScene.h"

RenderTargetScene::RenderTargetScene()
{
    CreateObjects();

    renderTarget = new RenderTarget();
    depthStencil = new DepthStencil();

    //quad->GetMaterial()->SetShader(L"Basic/Texture.hlsl");
    Texture* target = Texture::Add(L"target", renderTarget->GetSRV());
    quad->GetMaterial()->SetShader(L"PostEffect/RadialBlur.hlsl");    
    quad->GetMaterial()->SetDiffuseMap(target);       

    quad2->GetMaterial()->SetShader(L"PostEffect/Blur.hlsl");
    quad2->GetMaterial()->SetDiffuseMap(target);

    valueBuffer = new FloatValueBuffer();
    //valueBuffer->GetData()[1] = CENTER_X;
    //valueBuffer->GetData()[2] = CENTER_Y;
}

RenderTargetScene::~RenderTargetScene()
{
    delete quad;
    delete quad2;
    delete sphere;
    delete model;

    delete valueBuffer;
}

void RenderTargetScene::Update()
{
    quad->UpdateWorld();
    quad2->UpdateWorld();

    sphere->UpdateWorld();
    model->UpdateWorld();
    bot->Update();
}

void RenderTargetScene::PreRender()
{
    renderTarget->Set(depthStencil, {1, 1, 0, 1});

    sphere->Render();
    model->Render();
    bot->Render();
}

void RenderTargetScene::Render()
{
    
}

void RenderTargetScene::PostRender()
{
    valueBuffer->SetPS(10);

    quad->Render();
    quad2->Render();
}

void RenderTargetScene::GUIRender()
{
    ImGui::SliderFloat("Scale1", &valueBuffer->GetData()[0], 1, 200);
    ImGui::SliderFloat("Scale2", &valueBuffer->GetData()[1], 0, 10);
    ImGui::SliderFloat("Scale3", &valueBuffer->GetData()[2], 0, 10);

    quad->GetMaterial()->GUIRender();
    //quad2->GetMaterial()->GUIRender();
}

void RenderTargetScene::CreateObjects()
{
    quad = new Quad(Float2(CENTER_X, CENTER_Y));
    quad->SetTag("Quad");
    quad->SetLocalPosition(CENTER_X / 2, CENTER_Y, 0);

    quad2 = new Quad(Float2(CENTER_X, CENTER_Y));
    quad2->SetTag("Quad2");
    quad2->SetLocalPosition(CENTER_X / 2 * 3, CENTER_Y, 0);

    sphere = new Sphere();
    sphere->SetTag("Sphere");
    sphere->Load();
    sphere->GetMaterial()->Load("TextData/Materials/FieldStone.mat");

    model = new Model("Bunny");
    model->Load();

    bot = new Bot();
    bot->Load();
}
