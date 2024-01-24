#include "Framework.h"

Collider::Collider() : GameObject(L"Basic/Collider.hlsl")
{
    mesh = new Mesh<Vertex>();    

    SetColor(0, 1, 0);
}

Collider::~Collider()
{
    delete mesh;
}

void Collider::Render()
{
    if (!Active()) return;

    SetRender();

    mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

bool Collider::IsCollision(Collider* collider)
{
    if (!Active()) return false;
    if (!collider->Active()) return false;

    switch (collider->type)
    {
    case Collider::BOX:
        return IsBoxCollision((BoxCollider*)collider);        
    case Collider::SPHERE:
        return IsSphereCollision((SphereCollider*)collider);
    case Collider::CAPSULE:
        return IsCapsuleCollision((CapsuleCollider*)collider);    
    }

    return false;
}
