#pragma once

class SphereCollider : public Collider
{
public:
    SphereCollider(float radius = 1.0f, UINT stackCount = 8, UINT sliceCount = 16);
    ~SphereCollider() = default;

    virtual bool IsRayCollision(const Ray& ray, Contact* contact = nullptr) override;
    virtual bool IsBoxCollision(BoxCollider* collider) override;
    virtual bool IsSphereCollision(SphereCollider* collider) override;
    virtual bool IsCapsuleCollision(CapsuleCollider* collider) override;

    float Radius()
    {
        return radius * max(GetGlobalScale().x,
            max(GetGlobalScale().y, GetGlobalScale().z));
    }

private:
    virtual void MakeMesh() override;

private:
    float radius;
    UINT stackCount, sliceCount;
};