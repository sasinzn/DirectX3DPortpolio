#pragma once

class CapsuleCollider : public Collider
{
public:
    CapsuleCollider(float radius = 1.0f, float height = 2.0f,
        UINT stackCount = 8, UINT sliceCount = 16);
    ~CapsuleCollider() = default;

    virtual bool IsRayCollision(const Ray& ray, Contact* contact = nullptr) override;
    virtual bool IsBoxCollision(BoxCollider* collider) override;
    virtual bool IsSphereCollision(SphereCollider* collider) override;
    virtual bool IsCapsuleCollision(CapsuleCollider* collider) override;

    float Radius()
    {
        return radius * max(GetGlobalScale().x,
            max(GetGlobalScale().y, GetGlobalScale().z));
    }

    float Height() { return height * GetGlobalScale().y; }

private:
    virtual void MakeMesh() override;

private:
    float radius, height;
    UINT stackCount, sliceCount;
};