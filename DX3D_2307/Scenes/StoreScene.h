#pragma once

class StoreScene : public Scene
{
public:
	StoreScene();
	~StoreScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void ClickMainIcon();
	void ClickItem(void* panel);
	void SetWeapon();
	void ItemExplain();

private:
	Quad* storeBG;
	Quad* storeSelectBG;
	Quad* itemBG;
	Quad* nameBG;
	Quad* explainBG;
	Quad* statusBG;

	ModelAnimator* player;

	Button* mainBtn;

	Transform* storeModelRightHand;

	vector<string> weaponName;
	vector<Item*> items;
	vector<Button*> itemButtons;

	Item* renderedItem = nullptr;
	bool isItemRender = false;
};