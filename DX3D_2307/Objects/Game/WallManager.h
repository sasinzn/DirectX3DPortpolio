#pragma once

class WallManager : public Singleton<WallManager>
{
public:
	WallManager();
	~WallManager();

	void Update();
	void Render();
	void GUIRender();

	void Create(UINT poolSize);
	bool Collision(IN Ray ray, OUT Vector3& pos);

	void SetAStar(AStar* aStar) { this->astar = aStar; }
	void SetTerrain(Terrain* terrain) { this->terrain = terrain; tile->SetNode(terrain);}

	void BuildMode() { buildMode = true; } // 건물 짓기 모드 활성화
	void EndBuildMode() { buildMode = false; }
	void SetBuildPos(); //건물 지을 위치

	bool isBuildMode() { return buildMode; }
private:
	

private:
	Terrain* terrain;
	AStar* astar;
	AStar* tile;
	ModelInstancing* model;
	UINT instanceIndex;

	vector<Wall*> walls;
	bool buildMode = false;
	bool wallPicking = false;

	Vector3 buildPos;
};
