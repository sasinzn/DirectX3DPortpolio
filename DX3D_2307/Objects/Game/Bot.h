#pragma once

class Bot : public Transform
{
private:
    enum State
    {
        IDLE,
        RUN_F, RUN_B, RUN_R, RUN_L,
        ATTACK
    };

public:
    Bot();
    ~Bot();

    void Update();
    void Render();
    void PostRender();
    void GUIRender();

    void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
    CapsuleCollider* GetHitCollider() { return hitCollider; }
    BoxCollider* GetAttackCollider() { return sword->GetCollider(); }

private:
    void Control();
    void Move();

    void SetAnimation();
    void SetState(State state);

    void LoadClips();

    void EndAttack();
    void ActiveSword();
    void InactiveSword();

private:
    ModelAnimator* model;
    Sword* sword;
    Transform* rightHand;
    Terrain* terrain;
    CapsuleCollider* hitCollider;

    State curState = IDLE;

    float moveSpeed = 10.0f;
    float rotSpeed = 1.0f;
    float accelation = 2.0f;
    float deceleration = 5.0f;

    Vector3 velocity;

    POINT clientCenterPos = { WIN_WIDTH >> 1, WIN_HEIGHT >> 1 };
};