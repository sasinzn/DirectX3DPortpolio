#include "Framework.h"
#include "ParticleScene.h"

ParticleScene::ParticleScene()
{
    collider = new SphereCollider(50);
    collider->UpdateWorld();

    //particle = new Sprite(L"Textures/FX/explosion.png", 30, 30, 5, 3);
    //particle = new Sprite(L"Textures/FX/fire_8x2.png", 10, 30, 8, 2, true);
    //particle = new Sprite(L"Textures/FX/greenCore_4x4.png", 30, 30, 4, 4, true);
    //particle = new Spark(L"Textures/FX/star.png", true);
    //particle = new Spark(L"Textures/FX/snow.png");
    //particle = new Rain();
    //particle = new Snow();
    //particle->Play({});

    particle = new ParticleSystem("TextData/FX/explosion.fx");
}

ParticleScene::~ParticleScene()
{
    delete collider;
    delete particle;    
}

void ParticleScene::Update()
{
    if (KEY->Down(VK_LBUTTON))
    {
        Ray ray = CAM->ScreenPointToRay(mousePos);
        Contact contact;

        if (collider->IsRayCollision(ray, &contact))
        {
            particle->Play(contact.hitPoint);
        }
    }

    particle->Update();
}

void ParticleScene::PreRender()
{
}

void ParticleScene::Render()
{
    collider->Render();
    particle->Render();
}

void ParticleScene::PostRender()
{
}

void ParticleScene::GUIRender()
{
    //particle->GUIRender();
}
