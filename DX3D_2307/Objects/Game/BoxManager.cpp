#include "Framework.h"

BoxManager::BoxManager()
{
    textures.push_back(L"Textures/Landscape/Box.png");
    textures.push_back(L"Textures/Landscape/Bricks.png");
    textures.push_back(L"Textures/Landscape/Dirt.png");
    textures.push_back(L"Textures/Landscape/Dirt2.png");
    textures.push_back(L"Textures/Landscape/Dirt3.png");
}

BoxManager::~BoxManager()
{
    for (Box* box : boxes)
        delete box;
}

void BoxManager::Update()
{
    for (Box* box : boxes)
        box->Update();
}

void BoxManager::Render()
{
    for (Box* box : boxes)
        box->Render();
}

void BoxManager::CreateFloor(UINT x, UINT y, UINT z)
{
    for (UINT i = 0; i < x; i++)
    {
        for (UINT j = 0; j < y; j++)
        {
            for (UINT k = 0; k < z; k++)
            {
                Box* box = new Box();
                box->SetLocalPosition(Vector3(i, j, k));
                wstring file = textures[Random(0, textures.size())];
                box->GetMaterial()->SetDiffuseMap(file);

                boxes.push_back(box);
            }
        }
    }
}

float BoxManager::GetHeight(Vector3 pos)
{
    Ray ray(pos, Vector3::Down());
    Contact contact;

    float maxHeight = -5.0f;

    for (Box* box : boxes)
    {
        if (box->GetCollider()->IsRayCollision(ray, &contact))
        {
            if (contact.hitPoint.y > maxHeight)
                maxHeight = contact.hitPoint.y;
        }
    }

    return maxHeight;
}

Box* BoxManager::GetCollision()
{
    Ray ray = CAM->ScreenPointToRay(Vector3(CENTER_X, CENTER_Y));

    float minDistance = FLT_MAX;
    Contact contact;    

    list<Box*>::iterator iter = boxes.begin();
    list<Box*>::iterator collisionBox = boxes.end();

    for (; iter != boxes.end(); iter++)
    {
        if ((*iter)->GetCollider()->IsRayCollision(ray, &contact))
        {
            if (contact.distance < minDistance)
            {
                minDistance = contact.distance;                
                collisionBox = iter;
            }
        }
    }

    if (collisionBox == boxes.end())
        return nullptr;

    Box* box = *collisionBox;
    boxes.erase(collisionBox);

    return box;
}

bool BoxManager::AddBox(Box* box)
{
    Ray ray = CAM->ScreenPointToRay(Vector3(CENTER_X, CENTER_Y));

    float minDistance = FLT_MAX;
    Contact contact;
    Vector3 hitPoint;

    list<Box*>::iterator iter = boxes.begin();
    list<Box*>::iterator collisionBox = boxes.end();

    for (; iter != boxes.end(); iter++)
    {
        if ((*iter)->GetCollider()->IsRayCollision(ray, &contact))
        {
            if (contact.distance < minDistance)
            {
                minDistance = contact.distance;
                collisionBox = iter;
                hitPoint = contact.hitPoint;
            }
        }
    }

    if (collisionBox == boxes.end())
        return false;

    box->SetLocalPosition(hitPoint);
    boxes.push_back(box);

    return true;
}
