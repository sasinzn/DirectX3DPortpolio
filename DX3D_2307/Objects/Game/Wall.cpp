#include "Framework.h"

Wall::Wall(Transform* transform, UINT indexNum)
	:index(indexNum)
{
	transform->SetParent(this);

	collider = new BoxCollider();
	collider->SetTag("WallCollider");
	collider->Load();
	collider->SetParent(this);
}

Wall::~Wall()
{
	delete collider;
}

void Wall::Update()
{
	collider->UpdateWorld();
	UpdateWorld();
}

void Wall::Render()
{
	if (CAM->ContainPoint(GetGlobalPosition()) == false) return;

	//collider->Render();
}

void Wall::GUIRender()
{
	collider->GUIRender();
}
