#pragma once

class Wall : public Transform
{
public:
	Wall(Transform* transform, UINT indexNum);
	~Wall();

	void Update();
	void Render();
	void GUIRender();

	UINT GetIndex() { return index; }
	BoxCollider* GetCollider() { return collider; }

private:
	BoxCollider* collider; //�ı� ���� ������Ʈ ������ ���� �ݸ���


	AStar* astar; // ��ġ�� ��ġ ���� �ľ��� ���� astar ��� ����

	UINT index; // modelinstancing index


};