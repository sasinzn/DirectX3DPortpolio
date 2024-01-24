#include "Framework.h"

Tower::Tower(Transform* transform, UINT indexNum):index(indexNum)
{
	transform->SetParent(this);

	collider = new SphereCollider(10);
	collider->SetParent(this);
	targetLay = new Lay();
}

Tower::~Tower()
{
	delete collider;
}

void Tower::Update()
{
	if (!Active()) return;

	UpdateWorld();
	collider->UpdateWorld();
	Attack();
	Rotate();
}

void Tower::Render()
{
	if (!Active()) return;
	if (CAM->ContainPoint(GetGlobalPosition()) == false) return;

	collider->Render();
}

void Tower::GUIRender()
{
}

void Tower::Rotate()
{
	if (target)
	{
		targetDistance = Distance(target->GetLocalPosition(), GetLocalPosition());
		Vector3 forward = GetForward();
		Vector3 dir = target->GetLocalPosition() - GetLocalPosition();
		Vector3 cross = Cross(forward, dir);

		if (cross.y < 0)
			Transform::Rotate(Vector3::Up() * rotSpeed * DELTA);
		else if (cross.y > 0)
			Transform::Rotate(Vector3::Down() * rotSpeed * DELTA);

		if (targetDistance > 10.0)
			target = nullptr;
	}

}

void Tower::Attack()
{
	if (target)
	{
		if (target->isDying())
		{
			target = nullptr;
			return;
		}
		
		if (delay > attackDelay)
		{
			target->Hit();
			delay = 0;
		}
		delay += DELTA;
	}
}
