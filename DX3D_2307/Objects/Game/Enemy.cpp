#include "Framework.h"

Enemy::Enemy(Transform* transform, ModelAnimatorInstancing* instancingModel, UINT index)
    :instancing(instancingModel), enemyIndexNum(index)
{
    transform->SetParent(this);

    hitCollider = new BoxCollider({ 1,2,1 });
    hitCollider->SetParent(this);
    hitCollider->SetTag("EnemyHitCollider");
    hitCollider->Load();

    attackCollider = new SphereCollider(3);
    attackCollider->SetParent(this);
    attackCollider->SetTag("EnemyAttackCollider");
    attackCollider->Load();
    
    rightHand = new Transform();
    rightFist = new SphereCollider(3);
    rightFist->SetParent(rightHand);
    rightFist->SetTag("EnemyRightFist");
    rightFist->Load();
    rightFist->SetColor(1, 0, 0);
    rightFist->SetActive(false);

   
    maxHp = 5;
    currentHp = 5;
    hpBar = new ProgressBar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_bar_BG.png");
    //instancing set
    motion = instancingModel->GetMotion(index);
    totalEvent.resize(instancingModel->GetClipSize());
    eventIters.resize(instancingModel->GetClipSize());

    SetEvent(REACTION, bind(&Enemy::EndHit, this), 0.9f);
    SetEvent(DYING, bind(&Enemy::EndDying, this), 0.9f);
    SetEvent(ATTACK, bind(&Enemy::EndAttack, this), 0.9f);

    FOR(totalEvent.size())
    {
        eventIters[i] = totalEvent[i].begin();
    }

    effect = new ParticleSystem("TextData/FX/explosion.fx");
    effect->SetActive(false);
}

Enemy::~Enemy()
{
    delete hitCollider;
    delete attackCollider;
    delete hpBar;
    delete effect;
}

void Enemy::Update()
{
    if (!Active()) return;

    Vector3 screenPos = CAM->WorldToScreen(localPosition + Vector3::Up() * 5.0f);
    hpBar->SetLocalPosition(screenPos);

    UpdateWorld();
    hitCollider->UpdateWorld();
    attackCollider->UpdateWorld();

    rightFist->UpdateWorld();
    hpBar->UpdateWorld();
    Move();
    ExcuteEvent();
    Rotate();

    effect->Update();

}

void Enemy::Render()
{
    if (!Active()) return;
    if (CAM->ContainPoint(GetGlobalPosition()) == false) return;

    hitCollider->Render();
    attackCollider->Render();
    rightFist->Render();
    effect->Render();
}

void Enemy::GUIRender()
{
    attackCollider->GUIRender();
    hitCollider->GUIRender();
    rightFist->GUIRender();
    ImGui::SliderInt("Hp", &currentHp, 0, 10);
    ImGui::Text(to_string((int)time).c_str());
}

void Enemy::PostRender()
{
    if (!isActive) return;
    if (CAM->ContainPoint(GetGlobalPosition()) == false) return;

    Vector3 worldPos = localPosition + Vector3::Up() * 5.0f;
    Vector3 screenPos = CAM->WorldToScreen(worldPos);
    hpBar->Render();
    Font::Get()->RenderText(to_string(currentHp), { screenPos.x - 25, screenPos.y });
    Font::Get()->RenderText("/", { screenPos.x, screenPos.y });
    Font::Get()->RenderText(to_string(maxHp), { screenPos.x + 25, screenPos.y });
}


void Enemy::SetEvent(int clip, Event event, float timeRatio)
{
    if (totalEvent[clip].count(timeRatio) > 0)
        return;

    totalEvent[clip][timeRatio] = event;
}

void Enemy::ExcuteEvent()
{
    int index = curState;

    if (totalEvent[index].empty()) return;
    if (eventIters[index] == totalEvent[index].end()) return;

    float ratio = motion->runningTime / motion->duration;

    if (eventIters[index]->first > ratio) return;

    eventIters[index]->second();
    eventIters[index]++;
}


void Enemy::Hit(Collider* collider)
{
    if (hitCollider->IsCollision(collider))
    {
        SetState(REACTION);
        hitCollider->SetColor(1, 0, 0);
    }
    
}

void Enemy::Hit()
{
    if (curState == REACTION) return;
    if (curState == DYING) return;

    Vector3 hitPos = GetGlobalPosition();
    hitPos.y = hitPos.y + 2.f;
    effect->Play(hitPos);

    SetState(REACTION);
    hitCollider->SetColor(1, 0, 0);

    currentHp -= 3;
    
    hpBar->SetPercent(currentHp / (float)maxHp);

    if (currentHp <= 0)
    {
        currentHp = 0;
        dying = true;
        SetState(DYING);
    }
}

void Enemy::EndHit()
{
    SetState(IDLE);
    hitCollider->SetColor(0, 1, 0);

   
}

void Enemy::EndDying()
{
    CoinManager::Get()->Spawn(GetLocalPosition());
    SetActive(false);
    
}

void Enemy::Reset()
{
    maxHp = 5;
    currentHp = 5;

    dying = false;
}

void Enemy::SetRoute()
{
    int start = aStar->FindCloseNode(GetLocalPosition());
    int end = aStar->FindCloseNode(destPos[curDestNum]);

    aStar->GetPath(start, end, path);
}

void Enemy::SetState(State state)
{
    if (curState == state) return;

    curState = state;

    EnemyManager::Get()->GetInstancingModel()->PlayClip(enemyIndexNum, state);
    eventIters[state] = totalEvent[state].begin();
}

void Enemy::Move()
{
    if (curState == REACTION || curState == DYING) return;

    if (path.empty())
    {
        SetState(IDLE);
        if (destNum-1 > curDestNum)
        {
            curDestNum++;
            SetRoute();
        }
        return;
    }

   

    SetState(WALK_F);

    Vector3 dest = path.back();

    Vector3 direction = dest - GetGlobalPosition();
    direction.y = 0.0f;

    if (direction.Length() < 1.0f)
    {
        path.pop_back();
    }
       

    velocity = direction.GetNormalized();

    Translate(velocity * moveSpeed * DELTA);
    localPosition.y = terrain->GetHeight(localPosition);

}

void Enemy::Rotate()
{
    if (curState == IDLE) return;

    Vector3 forward = GetForward();
    Vector3 cross = Cross(forward, velocity);

    if (cross.y < 0)
        Transform::Rotate(Vector3::Up() * rotSpeed * DELTA);
    else if (cross.y > 0)
        Transform::Rotate(Vector3::Down() * rotSpeed * DELTA);
}

