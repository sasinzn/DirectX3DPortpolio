#pragma once

class MainScene : public Scene
{
public:
	MainScene();
	~MainScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

	void ClickStoreIcon();
	void ClickEquipIcon();
	void ClickMissionIcon();
private:
	Quad* mainBG;
	Quad* menuBG;
	Button* storeBtn;
	Button* equipBtn;
	Button* missionBtn;

};