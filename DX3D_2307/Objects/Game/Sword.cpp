#include "Framework.h"

Sword::Sword() : Model("Sword")
{
    collider = new BoxCollider();
    collider->SetTag("SwordCollider");
    collider->Load();
    collider->SetParent(this);
    collider->SetColor(1, 0, 0);
    collider->SetActive(false);

    Load();
}

Sword::~Sword()
{
    delete collider;
}

void Sword::Update()
{
    UpdateWorld();
    collider->UpdateWorld();
}

void Sword::Render()
{
    Model::Render();
    collider->Render();
}

void Sword::GUIRender()
{
    Model::GUIRender();
    collider->GUIRender();
}
