#pragma once

class Car : public Transform
{
private:
    const float GRAVITY = 98.0f;
    const float JUMP_POWER = 50.0f;
    const float CAR_HEIGHT = 1.0f;

public:
    Car();
    ~Car();

    void Update();
    void Render();
    void GUIRender();

    void SetTerrain(Terrain* terrain) { this->terrain = terrain; }

private:
    void CreateMesh();
    void UpdateMesh();

    void RotateWheel();
    void Jump();
    void Move();

private:
    Cylinder* body;
    Sphere* head;
    vector<Cylinder*> wheels;

    Terrain* terrain;    

    Vector3 velocity;
    float moveSpeed = 10.0f;    
    float rotSpeed = 10.0f;
    float accelation = 100.0f;
    float deceleration = 20.0f;
};