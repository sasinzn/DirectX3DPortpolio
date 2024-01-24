#pragma once

class Camera : public Transform
{
public:
    Camera();
    ~Camera();

    void Update();
    void GUIRender();

    void SetView();

    Ray ScreenPointToRay(Vector3 screenPoint);

    Vector3 WorldToScreen(Vector3 worldPos);

    void SetTarget(Transform* target) { this->target = target; topView = false; }

    void TargetOptionSave(string file);
    void TargetOptionLoad(string file);

    bool ContainPoint(Vector3 point);
    bool ContainSphere(Vector3 center, float radius);

    ViewBuffer* GetViewBuffer() { return viewBuffer; }

    void SetTopView();

private:
    void FreeMode();    
    void FollowMode();

    void Frustum();
private:
    ViewBuffer* viewBuffer;

    Matrix view;
    Matrix invView;
    Matrix projection;

    float moveSpeed = 50.0f;
    float rotSpeed = 30.0f;

    Vector3 prevMousePos;

    //TargetOption
    Transform* target = nullptr;

    float distance = 20.0f;
    float height = 20.0f;
    float moveDamping = 5.0f;
    float rotDamping = 1.0f;
    float rotY = 0;
    float destRot = 0.0f;

    Vector3 destPos;
    Vector3 focusOffset;

    bool isLookAtTargetX = true;
    bool isLookAtTargetY = true;
    bool topView = false;

    Matrix rotMatrix;

    char file[128] = {};

    //Frustum
    Vector3 planes[6];
    float a, b, c, d;
};