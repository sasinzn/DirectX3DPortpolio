#include "Framework.h"

WallManager::WallManager()
{
    tile = new AStar(40, 40);
}

WallManager::~WallManager()
{
    for (Wall* wall : walls)
        delete wall;

    delete model;
    delete tile;
}

void WallManager::Update()
{
    model->Update();
    for (Wall* wall : walls)
        wall->Update();
    
    SetBuildPos();

    tile->Update();
}

void WallManager::Render()
{
    model->Render();
    for (Wall* wall : walls)
        wall->Render();
}

void WallManager::GUIRender()
{
    model->GUIRender();
    for (Wall* wall : walls)
        wall->GUIRender();
}

void WallManager::Create(UINT poolSize)
{
    model = new ModelInstancing("Wall");
    FOR(poolSize)
    {
        Transform* transform = model->Add();
        transform->Load("Wall");
        Wall* wall = new Wall(transform, i);
        wall->SetActive(false);
        walls.push_back(wall);
    }

}

bool WallManager::Collision(IN Ray ray, OUT Vector3& pos)
{
    for (Wall* wall : walls)
    {
        if (wall->GetCollider()->IsRayCollision(ray))
        {
            pos = wall->GetLocalPosition();
            return true;
        }
    }
    return false;
}

void WallManager::SetBuildPos()
{
    if (buildMode == true && wallPicking == false)
    {
        for (Wall* wall : walls)
        {
            if (wall->Active() == false)
            {
                instanceIndex = wall->GetIndex();
                wallPicking = true;
                return; 
            }
        }
    }

    if (wallPicking)
    {
        Vector3 pos;
        Ray ray = CAM->ScreenPointToRay(mousePos);
        terrain->Picking(ray, pos);
        pos = tile->FindCloseNodePos(pos);
        pos.y = pos.y + 0.1f;
        walls[instanceIndex]->SetActive(true);
        walls[instanceIndex]->SetLocalPosition(pos);
        if (KEY->Down('G'))
        {
            wallPicking = false;
            buildMode = false;
            astar->AddObstacle(walls[instanceIndex]->GetCollider());
            astar->SetObstacle();
            astar->Update();
            return;
        }  
    }
}
