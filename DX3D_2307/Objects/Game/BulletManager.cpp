#include "Framework.h"

BulletManager::BulletManager()
{
    modelLaser = new ModelInstancing("LazerBullet");
    modelBomb = new ModelInstancing("RocketBullet");
}

BulletManager::~BulletManager()
{
    delete modelLaser;
    delete modelBomb;
  
}

void BulletManager::Update()
{
    BulletUpdate();

    modelLaser->Update();
    modelBomb->Update();

    time += DELTA;
    if (time > 1.0f)
    {
        triger = true;
    }
}

void BulletManager::Render()
{
    BulletRender();

   

    modelLaser->Render();
    modelBomb->Render();
 
}

void BulletManager::GUIRender()
{
    modelLaser->GUIRender();
    modelBomb->GUIRender();
}

void BulletManager::Fire(Vector3 pos, Vector3 rot, Vector3 dir)
{
    switch (mode)
    {
    case BulletManager::LASER:
        for (Bullet* bullet : laserBullets)
        {
            if (!bullet->Active() && triger)
            {
                bullet->Fire(pos, rot, dir);
                triger = false;
                time = 0.0f;
                return;
            }
        }
        break;
    case BulletManager::BOMB:
        for (Bullet* bullet : bombBullets)
        {
            if (!bullet->Active() && triger)
            {
                bullet->Fire(pos, rot, dir);
                triger = false;
                time = 0.0f;
                return;
            }
        }
        break;
    default:
        break;
    }

   


}

void BulletManager::CreateBullets(UINT poolSize)
{
    laserBullets.resize(poolSize);
    bombBullets.resize(poolSize);

    FOR(poolSize)
    {
        Transform* laser = modelLaser->Add();
        laser->Load("LazerBullet");
        laserBullets[i] = new Bullet(laser);
        Transform* bomb = modelBomb->Add();
        bomb->Load("RocketBullet");
        bombBullets[i] = new Bullet(bomb);
    }
}

bool BulletManager::Collision(Collider* collider)
{
    for (Bullet* bullet : laserBullets)
    {
        if (bullet->Active())
        {
            if (bullet->IsCollision(collider))
            {
                return true;
            }
        }
    }

    for (Bullet* bullet : bombBullets)
    {
        if (bullet->Active())
        {
            if (bullet->IsCollision(collider))
            {
                bullet->SetActive(false);
                return true;
            }
        }
    }
        
    return false;
}

void BulletManager::BulletUpdate()
{
    
    for (Bullet* bullet : laserBullets)
        bullet->Update();

    for (Bullet* bullet : bombBullets)
        bullet->Update();
     
}

void BulletManager::BulletRender()
{
    for (Bullet* bullet : laserBullets)
        bullet->Render();

    for (Bullet* bullet : bombBullets)
        bullet->Render();
}
