#pragma once

class Player : public Transform
{
private:
    enum State
    {
        IDLE,
        RUN_F, RUN_B, RUN_R, RUN_L,
        ATTACK, GUNDRAW,GUNIDLE,GUNFIRE
    };
    enum WeaponMode
    {
        GUN, SWORD
    };

public:
    Player();
    ~Player();

    void Update();
    void Render();
    void PostRender();
    void GUIRender();

    void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
    CapsuleCollider* GetHitCollider() { return hitCollider; }
    BoxCollider* GetAttackCollider() { return  sword->GetCollider(); }

    bool isBuildMode() { return buildMode; }
    UINT GetCoin() { return coinNum; }
    void UseCoin() { coinNum--; }

    void BuildModeOn() { buildMode = true; }   
    void BuildModeOff() { buildMode = false; }
    void CoinEmpty() { textOn = true; }
private:
    void Control();
    void Move();

    void SetAnimation();
    void SetState(State state);

    void LoadClips();

    void EndAttack();
    void ActiveSword();
    void InactiveSword();
    void DrawGun();
    void GunIDLE();
    void GunFire();
    void Fire();
    void FireEnd();

    void WeaponChange();
    void WeaponUpdate();
    void WeaponRender();
    void NotEnoughCoin();
private:
    ModelAnimator* model;
    Sword* sword;
    Model* laserGun;
    Transform* rightHand;
    Transform* hip;
    Terrain* terrain;
    CapsuleCollider* hitCollider;
    SphereCollider* itemGetCollider;

    State curState = IDLE;
    WeaponMode weaponMode = SWORD;

    float moveSpeed = 10.0f;
    float rotSpeed = 1.0f;
    float accelation = 2.0f;
    float deceleration = 5.0f;

    Vector3 velocity;

    POINT clientCenterPos = { WIN_WIDTH >> 1, WIN_HEIGHT >> 1 };

    bool buildMode = false;
    UINT coinNum = 5;

    bool textOn = false;
    float textTime = 0.0f;
};