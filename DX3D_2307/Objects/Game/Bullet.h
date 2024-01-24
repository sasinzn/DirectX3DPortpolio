#pragma once

class Bullet : public SphereCollider
{
private:
    const float LIFE_TIME = 10.0f;

public:
    Bullet(Transform* transform);
    ~Bullet() = default;

    void Update();

    void Fire(Vector3 pos, Vector3 rot, Vector3 dir);

private:
    Transform* modelTransform;

    Vector3 velocity = {};
    float speed = 10.0f;
    float time = 0;
};