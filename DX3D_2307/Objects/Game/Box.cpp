#include "Framework.h"

Box::Box()
{
    collider = new BoxCollider();
    collider->SetParent(this);
}

Box::~Box()
{
    delete collider;
}

void Box::Update()
{
    if (Active() == false) return;

    UpdateWorld();
    collider->UpdateWorld();
}

void Box::Render()
{
    if (Active() == false) return;

    Cube::Render();
    //collider->Render();
}
