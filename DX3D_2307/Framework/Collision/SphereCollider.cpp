#include "Framework.h"

SphereCollider::SphereCollider(float radius, UINT stackCount, UINT sliceCount)
    : radius(radius), stackCount(stackCount), sliceCount(sliceCount)
{
    type = SPHERE;

    MakeMesh();
    mesh->CreateMesh();
}

bool SphereCollider::IsRayCollision(const Ray& ray, Contact* contact)
{
    Vector3 P = ray.origin;
    Vector3 D = ray.direction;
    Vector3 C = GetGlobalPosition();
    Vector3 A = P - C;

    float r = pow(Radius(), 2);
    
    float b = Dot(D, A);
    float c = Dot(A, A) - r;

    float k = pow(b, 2);
    

    if (pow(b, 2) >= c)
    {
        float t = -b - sqrt(pow(b, 2) - c);

        if (contact)
        {
            contact->tag = tag;
            contact->distance = t;
            contact->hitPoint = P + D * t;
        }        

        return true;
    }

    return false;
}

bool SphereCollider::IsBoxCollision(BoxCollider* collider)
{
    return collider->IsSphereCollision(this);
}

bool SphereCollider::IsSphereCollision(SphereCollider* collider)
{
    float distance = Distance(GetGlobalPosition(), collider->GetGlobalPosition());

    return distance <= (Radius() + collider->Radius());
}

bool SphereCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
    return collider->IsSphereCollision(this);
}

void SphereCollider::MakeMesh()
{
    float thetaStep = XM_2PI / sliceCount;
    float phiStep = XM_PI / stackCount;

    vector<Vertex>& vertices = mesh->GetVertices();
    vertices.reserve((sliceCount + 1) * (stackCount + 1));

    for (UINT i = 0; i <= stackCount; i++)
    {
        float phi = i * phiStep;

        for (UINT j = 0; j <= sliceCount; j++)
        {
            float theta = j * thetaStep;

            Vertex vertex;
            vertex.pos.x = cos(theta) * sin(phi) * radius;
            vertex.pos.y = cos(phi) * radius;
            vertex.pos.z = sin(theta) * sin(phi) * radius;

            vertices.push_back(vertex);
        }
    }

    vector<UINT>& indices = mesh->GetIndices();

    indices.reserve(sliceCount * stackCount * 6);
    for (UINT i = 0; i < stackCount; i++)
    {
        for (UINT j = 0; j < sliceCount; j++)
        {
            indices.push_back((sliceCount + 1) * i + j);//0                                               
            indices.push_back((sliceCount + 1) * (i + 1) + j);//1

            indices.push_back((sliceCount + 1) * i + j);//0          
            indices.push_back((sliceCount + 1) * i + j + 1);//2                                    
        }
    }
}
