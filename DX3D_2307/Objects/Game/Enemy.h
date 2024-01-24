#pragma once

class Enemy : public Transform
{
private:
    enum State
    {
        IDLE,
        RUN_F, WALK_F,
        ATTACK, SWIPING, REACTION, DYING
    };
public:
    Enemy(Transform* transform, ModelAnimatorInstancing* instancingModel, UINT index);
    ~Enemy();

    void Update();
    void Render();
    void GUIRender();
    void PostRender();

    void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
    void SetAStar(AStar* astar) { this->aStar = astar; }
    void SetDestination(Vector3 destPos) { this->destPos.push_back(destPos); destNum = this->destPos.size(); }

    BoxCollider* GetHitCollider() { return hitCollider; }
    Transform* GetTransform() { return this; }

    void Hit(Collider* collider);
    void Hit();

    State GetState() { return curState; }
    UINT GetIndexNum() { return enemyIndexNum; }
    bool isDying() { return dying; }

    void Reset();
    void SetRoute();

private:
    void SetState(State state);
    void Move();
    void Rotate();

    
    void SetEvent(int clip, Event event, float timeRatio);
    void ExcuteEvent();
    
    void EndHit();
    void EndDying();
    void EndAttack() { SetState(IDLE); }
    //void ActiveAttack() { rightFist->SetActive(true); }
    //void InactiveAttack() { rightFist->SetActive(false); }
private:
    //colliders
    SphereCollider* attackCollider;
    BoxCollider* hitCollider;
    Transform* rightHand;
    SphereCollider* rightFist;
    
    Terrain* terrain;

    //UI
    ProgressBar* hpBar;
    int maxHp = 10;
    int currentHp = 10;

    //move
    State curState = IDLE;

    float moveSpeed = 8.0f;
    float rotSpeed = 1.0f;
    float accelation = 2.0f;
    float deceleration = 5.0f;
    float time = 0.0f;

    Vector3 velocity;

    Vector3 direction;
    Vector3 targetPos;
    AStar* aStar;

    //Astar
    UINT destNum = 0;
    UINT curDestNum = 0;
    vector<Vector3> destPos;
    vector<Vector3> path;
    bool dying = false;

    //instancing
    UINT enemyIndexNum;
    ModelAnimatorInstancing* instancing;
    ModelAnimatorInstancing::Motion* motion;

    //events
    vector<map<float, Event>> totalEvent;
    vector<map<float, Event>::iterator> eventIters;

    //effect
    ParticleSystem* effect;
};