#include "Framework.h"

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
    for (Enemy* enemy : enemies)
        delete enemy;

    delete modelInstancing;
}

void EnemyManager::Update()
{
    if (enemyCount <= maxEnemy && curTime > 30)
    {
        Spawn();
    }
       

    for (Enemy* enemy : enemies)
        enemy->Update();

    modelInstancing->Update();
    curTime += DELTA;

}

void EnemyManager::Render()
{
    modelInstancing->Render();

    for (Enemy* enemy : enemies)
        enemy->Render();
}

void EnemyManager::PostRender()
{
    for (Enemy* enemy : enemies)
        enemy->PostRender();
    if (30 - curTime >= 0)
    {
        Font::Get()->RenderText("라운드 시작까지", { 200, 600 });
        Font::Get()->RenderText(" : ", { 250, 600 });
        Font::Get()->RenderText(to_string(30 - (UINT)curTime) + " 초", {300, 600});
    }
}

void EnemyManager::GUIRender()
{
    modelInstancing->GUIRender();
}

void EnemyManager::CreateEnemies(UINT poolSize)
{
    roundData = DataManager::Get()->GetRoundDatas();

    CreateModel();

    maxEnemy = poolSize;
    enemyCount = 0;

    FOR(poolSize)
    {
        Transform* transform = modelInstancing->Add();
        transform->Load("Mutant");
        Enemy* enemy = new Enemy(transform, modelInstancing, i);
        enemy->SetActive(false);
        enemy->SetTerrain(terrain);
        enemy->SetAStar(aStar);
        enemy->SetDestination({ 10,0,10 });
        enemy->SetDestination({ 100,0,10 });
        enemy->SetDestination({ 100,0,100 });
        enemy->SetDestination({ 10,0,100 });
        enemies.push_back(enemy);
    }
}

bool EnemyManager::Collision()
{
    for (Enemy* enemy : enemies)
    {
        if (enemy->Active())
        {
           //enemy->Hit(player->GetAttackCollider());
           if (BulletManager::Get()->Collision(enemy->GetHitCollider()))
           {
              enemy->Hit();
           }
        }
    }

    return false;
}

Enemy* EnemyManager::TargetCollision(Collider* collider)
{
    for (Enemy* enemy : enemies)
    {
        if (enemy->Active() && enemy->isDying() == false)
        {
            if (enemy->GetHitCollider()->IsCollision(collider))
            {
                return enemy;
            }
        }
    }
    return nullptr;
}

void EnemyManager::Spawn()
{
    playTime += DELTA;

    if (playTime > SPAWN_TIME)
    {
        playTime = 0.0f;

        for (Enemy* enemy : enemies)
        {
            if (enemy->Active() == false && enemy->isDying() == false)
            {
                enemy->SetActive(true);
                enemyCount++;
                Vector3 pos;
                pos.x = 100 + Random(0, 10);
                pos.y = Random(1, 2);
                pos.z = 100 + Random(0, 10);

                enemy->SetLocalPosition(pos);
                enemy->SetRoute();
                
                return;
            }
        }
    }
}

void EnemyManager::CreateModel()
{
    modelInstancing = new ModelAnimatorInstancing("Mutant");
    modelInstancing->ReadClip("Idle");
    modelInstancing->ReadClip("RunF");
    modelInstancing->ReadClip("WalkF");
    modelInstancing->ReadClip("Attack");
    modelInstancing->ReadClip("Swiping");
    modelInstancing->ReadClip("Reaction");
    modelInstancing->ReadClip("Dying");
    modelInstancing->Load("Mutant");
}
