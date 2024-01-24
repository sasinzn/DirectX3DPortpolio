#pragma once

class BulletManager : public Singleton<BulletManager>
{
private:
    enum BulletMode
    {
        LASER, BOMB
    };

public:
    BulletManager();
    ~BulletManager();

    void Update();
    void Render();
    void GUIRender();

    void Fire(Vector3 pos, Vector3 rot, Vector3 dir);
    void SetMode(int mode) { this->mode = (BulletMode)mode; }

    void CreateBullets(UINT poolSize);
    bool Collision(Collider* collider);
private:
    void BulletUpdate();
    void BulletRender();

private:
    ModelInstancing* modelLaser;
    ModelInstancing* modelBomb;
    vector<Bullet*> laserBullets;
    vector<Bullet*> bombBullets;
    bool triger = true;
    float time = 0.0f;

    BulletMode mode = LASER;
};