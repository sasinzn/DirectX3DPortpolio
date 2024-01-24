#include "Framework.h"

Coin::Coin(Transform* transform, UINT indexNum):index(indexNum)
{
    transform->SetParent(this);
    collider = new SphereCollider();
    collider->SetLocalPosition(0, 1, 0);
    collider->SetParent(this);

}

Coin::~Coin()
{
    delete collider;
}

void Coin::Update()
{
    if (!Active()) return;
    UpdateWorld();
    collider->UpdateWorld();
}

void Coin::Render()
{
    if (!Active()) return;

}

void Coin::GUIRender()
{
}

bool Coin::Collision(Collider* magnetic, Collider* getItem)
{
    if (collider->IsCollision(magnetic))
    {
        Vector3 direction = magnetic->GetGlobalPosition() - GetGlobalPosition();
        direction.y = 0.0f;

        velocity = direction.GetNormalized();

        Translate(velocity * moveSpeed * DELTA);
    }
    
    if (collider->IsCollision(getItem))
    {
        SetActive(false);
        return true;
    }

    return false;
}
