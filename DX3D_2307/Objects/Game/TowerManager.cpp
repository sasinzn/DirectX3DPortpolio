#include "Framework.h"

TowerManager::TowerManager()
{
}

TowerManager::~TowerManager()
{
    for (Tower* tower : towers)
        delete tower;

    delete model;
}

void TowerManager::Update()
{
    for (Tower* tower : towers)
        tower->Update();

    model->Update();
    SetBuildPos();
    Collision();
}

void TowerManager::Render()
{
    for (Tower* tower : towers)
        tower->Render();
    model->Render();
}

void TowerManager::GUIRender()
{
    for (Tower* tower : towers)
        tower->GUIRender();
}

void TowerManager::Create(UINT poolSize)
{
    model = new ModelInstancing("missileTower");
    FOR(poolSize)
    {
        Transform* transform = model->Add();
        transform->Load("missileTower");
        Tower* tower = new Tower(transform, i);
        tower->SetActive(false);
        towers.push_back(tower);
    }
}

bool TowerManager::Collision()
{
    for (Tower* tower : towers)
    {
        if (tower->isTarget() == false && tower->Active() == true)
        {
            tower->SetTarget(EnemyManager::Get()->TargetCollision(tower->GetCollider()));
        }
    }
    return false;
}

void TowerManager::SetBuildPos()
{
    if (buildMode == true && towerPicking == false)
    {
        for (Tower* tower : towers)
        {
            if (tower->Active() == false)
            {
                instanceIndex = tower->GetIndex();
                towerPicking = true;
                return;
            }
        }
    }

    if (towerPicking)
    {
        Vector3 pos;
        Ray ray = CAM->ScreenPointToRay(mousePos);
        if (WallManager::Get()->Collision(ray, pos))
        {
            pos.y = pos.y + 5.0f;
            towers[instanceIndex]->SetActive(true);
            towers[instanceIndex]->SetLocalPosition(pos);
        }
        if (KEY->Down(VK_LBUTTON))
        {
            towerPicking = false;
            buildMode = false;
            coinCheck = true;
            return;
        }
    }

}
