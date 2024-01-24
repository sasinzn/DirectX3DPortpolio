#pragma once

class EnemyManager : public Singleton<EnemyManager>
{
private:
    const float SPAWN_TIME = 1.0f;

public:
    EnemyManager();
    ~EnemyManager();

    void Update();
    void Render();
    void PostRender();
    void GUIRender();

    void CreateEnemies(UINT poolSize);
    bool Collision();
    Enemy* TargetCollision(Collider* collider);

    void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
    void SetAStar(AStar* aStar) { this->aStar = aStar; }
    void SetPlayer(class Player* player) { this->player = player; }
    ModelAnimatorInstancing* GetInstancingModel() { return modelInstancing; }
    vector<Enemy*> GetEnemies() { return enemies; }
    float GetTime() { return curTime; }

private:
    void Spawn();
    void CreateModel();

private:
    //Enemy model
    ModelAnimatorInstancing* modelInstancing;
    vector< ModelAnimatorInstancing*> models;
    int instanceIndex = 0;
    int clip = 0;
    vector<Enemy*> enemies;

    Terrain* terrain;
    AStar* aStar;
    class Player* player;
    bool setNode = false;

    //effect
    vector<ParticleSystem*> effects;

    //roundDatas
    map<int, RoundData> roundData;
    UINT enemyCount;
    UINT maxEnemy;
    float curTime;
    float playTime = 0.0f;
    int curRound;
};