#include "Framework.h"

Fox::Fox()
{
    model = new ModelAnimator("Fox");
    model->ReadClip("Idle");
    model->ReadClip("Run");
    model->SetParent(this);
    model->SetLocalRotation(XM_PIDIV2, 0, 0);
    model->SetLocalScale(0.1f, 0.1f, 0.1f);

    model->PlayClip(curState);
}

Fox::~Fox()
{
    delete model;
}

void Fox::Update()
{
    Control();
    Move();
    Rotate();

    UpdateWorld();
    model->Update();    
}

void Fox::Render()
{
    model->Render();
}

void Fox::Control()
{
    if (KEY->Down(VK_LBUTTON))
    {
        Ray ray = CAM->ScreenPointToRay(mousePos);

        if (terrain->Picking(ray, destPos))
        {
            //path.insert(path.begin(), destPos);
            int start = aStar->FindCloseNode(GetGlobalPosition());
            int end = aStar->FindCloseNode(destPos);

            aStar->GetPath(start, end, path);
        }
    }
}

void Fox::Move()
{
    if (path.empty())
    {
        SetState(IDLE);
        return;
    }

    SetState(RUN);

    Vector3 dest = path.back();

    Vector3 direction = dest - GetGlobalPosition();
    direction.y = 0.0f;

    if (direction.Length() < 1.0f)
        path.pop_back();

    velocity = direction.GetNormalized();

    Translate(velocity * moveSpeed * DELTA);
    localPosition.y = terrain->GetHeight(localPosition);
}

void Fox::Rotate()
{
    if (curState == IDLE) return;

    Vector3 forward = GetForward();
    Vector3 cross = Cross(forward, velocity);

    if (cross.y < 0)
        Transform::Rotate(Vector3::Up() * rotSpeed * DELTA);
    else if(cross.y > 0)
        Transform::Rotate(Vector3::Down() * rotSpeed * DELTA);
}

void Fox::SetState(State state)
{
    if (curState == state) return;

    curState = state;
    model->PlayClip(state);
}
