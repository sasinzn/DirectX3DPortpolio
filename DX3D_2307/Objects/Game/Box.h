#pragma once

class Box : public Cube
{
public:
    Box();
    ~Box();

    void Update();
    void Render();

    BoxCollider* GetCollider() { return collider; }

private:
    BoxCollider* collider;
};