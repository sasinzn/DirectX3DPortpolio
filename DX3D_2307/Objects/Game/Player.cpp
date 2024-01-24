#include "Framework.h"

Player::Player()
{
    model = new ModelAnimator("Axia");
    model->SetParent(this);
    model->SetLocalScale(0.05f, 0.05f, 0.05f);
    model->SetLocalRotation(0, XM_PI, 0);

    laserGun = new Model("MachineGun");
    laserGun->Load("MachineGunIDLE");
    

    LoadClips();
    model->PlayClip(0);

    hitCollider = new CapsuleCollider();
    hitCollider->SetParent(this);
    hitCollider->SetTag("AxiaHitCollider");
    hitCollider->Load();

    itemGetCollider = new SphereCollider(5);
    itemGetCollider->SetParent(this);
    itemGetCollider->SetTag("PlayerItemGetCollider");

    sword = new Sword();
    rightHand = new Transform();
    hip = new Transform();

    sword->SetParent(rightHand);
    laserGun->SetParent(hip);

    ClientToScreen(hWnd, &clientCenterPos);
    SetCursorPos(clientCenterPos.x, clientCenterPos.y);

    BulletManager::Get()->CreateBullets(30);
    
}

Player::~Player()
{
    delete model;
    delete sword;
    delete rightHand;
    delete laserGun;
    delete hitCollider;
    delete itemGetCollider;
    BulletManager::Delete();
}

void Player::Update()
{
    rightHand->SetWorld(model->GetTransformByNode(35));
    hip->SetWorld(model->GetTransformByNode(2));

    if(!buildMode)
        Control();
    
    
    SetAnimation();

    UpdateWorld();
    WeaponChange();
    WeaponUpdate();
    
    model->Update();
    hitCollider->UpdateWorld();
    itemGetCollider->UpdateWorld();

    if (KEY->Down(VK_ESCAPE))
        model->Stop();

    BulletManager::Get()->Update();
    
    coinNum += CoinManager::Get()->Collision(itemGetCollider, hitCollider);
    textTime += DELTA;

    if (textTime > 2.0f)
    {
        textOn = false;
        textTime = 0.f;
    }
}

void Player::Render()
{
    WeaponRender();
   
    model->Render();
    hitCollider->Render();
    itemGetCollider->Render();
    BulletManager::Get()->Render();
}

void Player::PostRender()
{
    Vector3 worldPos = localPosition + Vector3::Up() * 10.0f;
    Vector3 screenPos = CAM->WorldToScreen(worldPos);
    Font::Get()->RenderText(model->GetTag(), { screenPos.x, screenPos.y });

    Font::Get()->RenderText("현재 코인", { 200, 300 });
    Font::Get()->RenderText(" : ", { 250, 300 });
    Font::Get()->RenderText(to_string(coinNum) + " 개", { 300, 300 });

    if (textOn == true && textTime < 2.0f)
        NotEnoughCoin();
}

void Player::GUIRender()
{
    //sword->GUIRender();
    //hitCollider->GUIRender();
    //laserGun->GUIRender();
    BulletManager::Get()->GUIRender();
}

void Player::Control()
{
    Move();

    Vector3 delta = mousePos - Vector3(CENTER_X, CENTER_Y);
    SetCursorPos(clientCenterPos.x, clientCenterPos.y);

    Rotate(Vector3::Up() * delta.x * DELTA);
    CAM->Rotate(Vector3::Left() * delta.y * rotSpeed * DELTA);

    if (KEY->Down(VK_LBUTTON))
    {
        switch (weaponMode)
        {
        case Player::GUN:
            SetState(GUNFIRE);
            break;
        case Player::SWORD:
            SetState(ATTACK);
            break;
        default:
            break;
        }
    }
       
}

void Player::Move()
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

void Player::SetAnimation()
{
    if (curState == ATTACK || curState == GUNFIRE) return;

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

void Player::SetState(State state)
{
    if (curState == state) return;

    curState = state;
    model->PlayClip(state);
}

void Player::LoadClips()
{
    model->ReadClip("Idle");
    model->ReadClip("RunF");
    model->ReadClip("RunB");
    model->ReadClip("RunR");
    model->ReadClip("RunL");
    model->ReadClip("Attack");
    model->ReadClip("DrawingGun");
    model->ReadClip("GunIdle");
    model->ReadClip("GunFire");

    model->GetClip(ATTACK)->SetEvent(bind(&Player::EndAttack, this), 0.7f);
    model->GetClip(ATTACK)->SetEvent(bind(&Player::ActiveSword, this), 0.3f);
    model->GetClip(ATTACK)->SetEvent(bind(&Player::InactiveSword, this), 0.5f);
    model->GetClip(GUNDRAW)->SetEvent(bind(&Player::DrawGun, this), 0.3f);
    model->GetClip(GUNDRAW)->SetEvent(bind(&Player::GunIDLE, this), 0.9f);
    model->GetClip(GUNFIRE)->SetEvent(bind(&Player::GunFire, this), 0.01f);
    model->GetClip(GUNFIRE)->SetEvent(bind(&Player::Fire, this), 0.5f);
    model->GetClip(GUNFIRE)->SetEvent(bind(&Player::FireEnd, this), 0.9f);
}

void Player::EndAttack()
{
    SetState(IDLE);
}

void Player::ActiveSword()
{
    sword->GetCollider()->SetActive(true);
}

void Player::InactiveSword()
{
    sword->GetCollider()->SetActive(false);
}

void Player::DrawGun()
{
    laserGun->Load("MachineGunDraw");
    laserGun->SetParent(rightHand);
}

void Player::GunIDLE()
{
    SetState(GUNIDLE);
    laserGun->Load("MachineGunReady");
}

void Player::GunFire()
{
    laserGun->Load("MachineGunFire");
}

void Player::Fire()
{
    Ray ray = CAM->ScreenPointToRay(mousePos);
    BulletManager::Get()->Fire(GetLocalPosition(), CAM->GetLocalRotation(),ray.direction);
}

void Player::FireEnd()
{
    SetState(IDLE);
}

void Player::WeaponChange()
{
    if (KEY->Down('1'))
    {
        if (weaponMode == SWORD)
        {
            model->PlayClip(GUNDRAW);
        }

        weaponMode = GUN;
        BulletManager::Get()->SetMode(0);
    }
    if (KEY->Down('2'))
    {
        if (weaponMode == SWORD)
        {
            model->PlayClip(GUNDRAW);
        }
        weaponMode = GUN;
        BulletManager::Get()->SetMode(1);
    }
    if (KEY->Down('3'))
    {
        if (weaponMode == GUN)
        {
            model->PlayClip(0);
            laserGun->Load("MachineGunIDLE");
            laserGun->SetParent(hip);
        }

        weaponMode = SWORD;

    }

       

}

void Player::WeaponUpdate()
{
    switch (weaponMode)
    {
    case Player::GUN:
        laserGun->UpdateWorld();
        break;
    case Player::SWORD:
        sword->Update();
        break;
    default:
        break;
    }
}

void Player::WeaponRender()
{
    switch (weaponMode)
    {
    case Player::GUN:
        laserGun->Render();
        break;
    case Player::SWORD:
        sword->Render();
        break;
    default:
        break;
    }
   
}

void Player::NotEnoughCoin()
{
    Font::Get()->RenderText("코인이 부족합니다.", { 350, 300 });
}
