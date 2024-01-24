#include "Framework.h"

Bullet::Bullet(Transform* transform)
    : modelTransform(transform)
{
    SetLocalScale(0.2,0.2,0.2);
    modelTransform->SetParent(this);
    SetActive(false);
    SetLocalPosition(0, 15, 0);
}

void Bullet::Update()
{
    if (!Active()) return;

    Translate(velocity * speed * DELTA);
    time += DELTA;

    if (time > LIFE_TIME)
        isActive = false;

    UpdateWorld();
}

void Bullet::Fire(Vector3 pos, Vector3 rot, Vector3 dir)
{
    SetActive(true);

    SetLocalPosition(pos.x,pos.y+3,pos.z);
    SetLocalRotation(rot.x,rot.y,rot.z);
    velocity = dir;
    
    time = 0.0f;
}
