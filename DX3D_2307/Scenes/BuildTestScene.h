#pragma once

class BuildTestScene : public Scene
{
public:
	BuildTestScene();
	~BuildTestScene();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	AStar* astar;
	Terrain* terrain;

};