#pragma once

class Coin : public Transform
{
public:
	Coin(Transform* transform, UINT indexNum);
	~Coin();

	void Update();
	void Render();
	void GUIRender();

	bool Collision(Collider* magnetic, Collider* getItem);

	UINT GetIndex() { return index; }
	SphereCollider* GetCollider() { return collider; }

private:
	SphereCollider* collider; //충돌처리 위한 콜리더

	UINT index; // modelinstancing index

	Vector3 velocity;
	float moveSpeed = 20.f;

};