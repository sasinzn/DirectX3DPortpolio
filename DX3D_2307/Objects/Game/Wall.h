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
	BoxCollider* collider; //파괴 가능 오브젝트 구현을 위한 콜리더


	AStar* astar; // 설치할 위치 정보 파악을 위한 astar 노드 정보

	UINT index; // modelinstancing index


};