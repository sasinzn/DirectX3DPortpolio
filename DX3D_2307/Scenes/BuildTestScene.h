#pragma once

class BuildTestScene : public Scene
{
public:
	BuildTestScene();
	~BuildTestScene();

	// Scene��(��) ���� ��ӵ�
	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	AStar* astar;
	Terrain* terrain;

};