#pragma once

class TowerManager : public Singleton<TowerManager>
{
public:
	TowerManager();
	~TowerManager();

	void Update();
	void Render();
	void GUIRender();

	void Create(UINT poolSize);
	bool Collision();

	void BuildMode() { buildMode = true; } // 건물 짓기 모드 활성화
	void EndBuildMode() { buildMode = false; }
	void SetBuildPos(); //건물 지을 위치

	bool isBuildMode() { return buildMode; }
	bool UseCoinCheck() { return coinCheck; }
	void UseCoinFinish() { coinCheck = false; }
private:


private:
	ModelInstancing* model;
	UINT instanceIndex;

	vector<Tower*> towers;
	bool buildMode = false;
	bool towerPicking = false;
	bool coinCheck = false;

	Vector3 buildPos;
};
