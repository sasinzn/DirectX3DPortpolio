#include "Framework.h"

Bot::Bot()
{
    model = new ModelAnimator("Axia");
    model->SetParent(this);
    model->SetLocalScale(0.05f, 0.05f, 0.05f);
    model->SetLocalRotation(0, XM_PI, 0);

    LoadClips();
    model->PlayClip(0);

    hitCollider = new CapsuleCollider();
    hitCollider->SetParent(this);
    hitCollider->SetTag("AxiaHitCollider");
    hitCollider->Load();
    sword = new Sword();
    rightHand = new Transform();

    sword->SetParent(rightHand);

    ClientToScreen(hWnd, &clientCenterPos);
    SetCursorPos(clientCenterPos.x, clientCenterPos.y);


}

Bot::~Bot()
{
    delete model;
    delete sword;
    delete rightHand;
}

void Bot::Update()
{
    rightHand->SetWorld(model->GetTransformByNode(35));

    Control();
    SetAnimation();

    UpdateWorld();
    sword->Update();
    model->Update();
    hitCollider->UpdateWorld();

    if (KEY->Down(VK_ESCAPE))
        model->Stop();
}

void Bot::Render()
{
    sword->Render();
    model->Render();
    hitCollider->Render();
}

void Bot::PostRender()
{
    Vector3 worldPos = localPosition + Vector3::Up() * 10.0f;
    Vector3 screenPos = CAM->WorldToScreen(worldPos);
    Font::Get()->RenderText(model->GetTag(), { screenPos.x, screenPos.y });
}

void Bot::GUIRender()
{
    sword->GUIRender();
    hitCollider->GUIRender();
}

void Bot::Control()
{
    Move();

    Vector3 delta = mousePos - Vector3(CENTER_X, CENTER_Y);
    SetCursorPos(clientCenterPos.x, clientCenterPos.y);

    Rotate(Vector3::Up() * delta.x * DELTA);
    CAM->Rotate(Vector3::Left() * delta.y * rotSpeed * DELTA);

    if (KEY->Down(VK_LBUTTON))
        SetState(ATTACK);
}

void Bot::Move()
{
    if (curState == ATTACK) return;

    bool isMoveZ = false;
    bool isMoveX = false;

    if (KEY->Press('W'))
    {
        velocity.z += accelation * DELTA;
        isMoveZ = true;
    }

    if (KEY->Press('S'))
    {
        velocity.z -= accelation * DELTA;
        isMoveZ = true;
    }

    if (KEY->Press('A'))
    {
        velocity.x -= accelation * DELTA;
        isMoveX = true;
    }

    if (KEY->Press('D'))
    {
        velocity.x += accelation * DELTA;
        isMoveX = true;
    }

    if (velocity.Length() > 1.0f)
        velocity.Normalize();

    if (!isMoveX)
        velocity.x = Lerp(velocity.x, 0, deceleration * DELTA);

    if (!isMoveZ)
        velocity.z = Lerp(velocity.z, 0, deceleration * DELTA);

    Matrix rotY = XMMatrixRotationY(GetLocalRotation().y);
    Vector3 direction = XMVector3TransformNormal(velocity, rotY);

    Translate(direction * moveSpeed * DELTA);

    if (terrain)
        localPosition.y = terrain->GetHeight(localPosition);
}

void Bot::SetAnimation()
{
    if (curState == ATTACK) return;

    if (velocity.z > 0.1f)
        SetState(RUN_F);
    else if (velocity.z < -0.1f)
        SetState(RUN_B);
    else if (velocity.x < -0.1f)
        SetState(RUN_L);
    else if (velocity.x > 0.1f)
        SetState(RUN_R);
    else
        SetState(IDLE);
}

void Bot::SetState(State state)
{
    if (curState == state) return;

    curState = state;
    model->PlayClip(state);
}

void Bot::LoadClips()
{
    model->ReadClip("Idle");
    model->ReadClip("RunF");
    model->ReadClip("RunB");
    model->ReadClip("RunR");
    model->ReadClip("RunL");
    model->ReadClip("Attack");

    model->GetClip(ATTACK)->SetEvent(bind(&Bot::EndAttack, this), 0.7f);
    model->GetClip(ATTACK)->SetEvent(bind(&Bot::ActiveSword, this), 0.3f);
    model->GetClip(ATTACK)->SetEvent(bind(&Bot::InactiveSword, this), 0.5f);
}

void Bot::EndAttack()
{
    SetState(IDLE);
}

void Bot::ActiveSword()
{
    sword->GetCollider()->SetActive(true);
}

void Bot::InactiveSword()
{
    sword->GetCollider()->SetActive(false);
}
