#pragma once

#include "Objects/Basic/GameObject.h"

struct Ray
{
    Vector3 origin;
    Vector3 direction;

    Ray()
    {
    }

    Ray(Vector3 position, Vector3 direction)
        : origin(position), direction(direction)
    {
    }
};

struct Contact
{
    string tag;

    Vector3 hitPoint;
    float distance;
};

class Collider : public GameObject
{
public:
    enum Type
    {
        BOX, SPHERE, CAPSULE
    };

public:
    Collider();
    ~Collider();

    void Render();

    bool IsCollision(Collider* collider);

    virtual bool IsRayCollision(const Ray& ray, Contact* contact = nullptr) = 0;
    virtual bool IsBoxCollision(class BoxCollider* collider) = 0;
    virtual bool IsSphereCollision(class SphereCollider* collider) = 0;
    virtual bool IsCapsuleCollision(class CapsuleCollider* collider) = 0;

    void SetColor(Float4 color) { material->GetData()->diffuse = color; }
    void SetColor(float r, float g, float b)
    {
        material->GetData()->diffuse = { r, g, b, 1 };
    }

private:
    virtual void MakeMesh() = 0;

protected:
    Type type;

    Mesh<Vertex>* mesh;
};