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
	SphereCollider* collider; //�浹ó�� ���� �ݸ���

	UINT index; // modelinstancing index

	Vector3 velocity;
	float moveSpeed = 20.f;

};