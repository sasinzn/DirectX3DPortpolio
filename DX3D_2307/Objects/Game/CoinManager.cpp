#include "Framework.h"

CoinManager::CoinManager()
{
}

CoinManager::~CoinManager()
{
    for (Coin* coin : coins)
        delete coin;

    delete model;
}

void CoinManager::Update()
{
    model->Update();
    for (Coin* coin : coins)
        coin->Update();
    
}

void CoinManager::Render()
{
    model->Render();
    for (Coin* coin : coins)
        coin->Render();

}

void CoinManager::GUIRender()
{
}

void CoinManager::Create(UINT poolSize)
{
    model = new ModelInstancing("Coin");
    FOR(poolSize)
    {
        Transform* transform = model->Add();
        Coin* coin = new Coin(transform, i);
        coin->SetActive(false);
        coins.push_back(coin);
    }
}

void CoinManager::Spawn(Vector3 pos)
{
    for (Coin* coin : coins)
    {
        if (coin->Active() == false)
        {
            coin->SetActive(true);
            coin->SetLocalPosition(pos);
            coin->Update();
            return;
        }
    }
}

bool CoinManager::Collision(Collider* playerItemGetCol, Collider* player)
{
    for (Coin* coin : coins)
    {
        if (coin->Active() == true)
        {
            if(coin->Collision(playerItemGetCol, player))
                return true;
        }
    }
    return false;
}
