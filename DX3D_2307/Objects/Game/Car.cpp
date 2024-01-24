#include "Framework.h"

Car::Car()
{
    CreateMesh();
}

Car::~Car()
{
    delete body;
    delete head;

    for (Cylinder* wheel : wheels)
        delete wheel;
}

void Car::Update()
{
    Ray ray = CAM->ScreenPointToRay(Vector3(CENTER_X, CENTER_Y, 0));

    Move();
    Jump();

    RotateWheel();
    Translate(velocity * DELTA);

    UpdateMesh();
}

void Car::Render()
{
    body->Render();
    head->Render();

    for (Cylinder* wheel : wheels)
        wheel->Render();
}

void Car::GUIRender()
{
    //head->GUIRender();
    //head->GetMaterial()->GUIRender();
    //body->GetMaterial()->GUIRender();
    //
    for (Cylinder* wheel : wheels)
        wheel->GUIRender();

    Transform::GUIRender();
}

void Car::CreateMesh()
{
    body = new Cylinder();
    body->SetTag("body");
    body->SetParent(this);
    body->Load();
    body->GetMaterial()->Load("TextData/Materials/CarBody.mat");

    head = new Sphere();
    head->SetTag("Head");
    head->SetParent(this);
    head->Load();
    head->GetMaterial()->Load("TextData/Materials/CarHead.mat");

    FOR(4)
    {
        string tag = "Wheel_" + to_string(i);

        Cylinder* wheel = new Cylinder();
        wheel->SetTag(tag);
        wheel->SetParent(this);
        wheel->Load();
        wheel->GetMaterial()->Load("TextData/Materials/CarWheel.mat");

        wheels.push_back(wheel);
    }
}

void Car::UpdateMesh()
{
    body->UpdateWorld();
    head->UpdateWorld();

    for (Cylinder* wheel : wheels)
        wheel->UpdateWorld();

    UpdateWorld();
}

void Car::RotateWheel()
{
    for (Cylinder* wheel : wheels)
    {
        wheel->Rotate(Vector3::Right() * moveSpeed * DELTA);
    }
}

void Car::Jump()
{
    if (KEY->Down(VK_SPACE))
        velocity.y = JUMP_POWER;

    velocity.y -= GRAVITY * DELTA;

    Vector3 normal;

    float bottom = localPosition.y - CAR_HEIGHT;
    float terrainHeight = terrain->GetHeight(localPosition, &normal);    

    if (velocity.y < 0 && bottom < terrainHeight)
    {
        velocity.y = 0.0f;
        localPosition.y = terrainHeight + CAR_HEIGHT;

        float rotY = localRotation.y;
        localRotation = GetRotFromNormal(normal);
        localRotation.y = rotY;
    }
}

void Car::Move()
{
    if (KEY->Press(VK_UP))
    {
        velocity += GetForward() * accelation * DELTA;
    }
    if (KEY->Press(VK_DOWN))
    {
        velocity += GetBack() * accelation * DELTA;
    }
    if (KEY->Press(VK_RIGHT))
    {
        Rotate(Vector3::Up() * rotSpeed * DELTA);
    }
    if (KEY->Press(VK_LEFT))
    {
        Rotate(Vector3::Down() * rotSpeed * DELTA);
    }

    velocity.x = Lerp(velocity.x, 0.0f, deceleration * DELTA);
    velocity.z = Lerp(velocity.z, 0.0f, deceleration * DELTA);        
}
