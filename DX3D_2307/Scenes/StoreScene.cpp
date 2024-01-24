#include "Framework.h"
#include "StoreScene.h"

StoreScene::StoreScene()
{
	storeBG = new Quad(L"Textures/UI/UIBackgroundResize.png");
	storeBG->SetTag("storeBG");
	storeBG->Load();

	storeSelectBG = new Quad(L"Textures/UI/ShopSelectBack.png");
	storeSelectBG->SetTag("storeSelectBG");
	storeSelectBG->Load();

	nameBG = new Quad(L"Textures/UI/ShopSelectBack.png");
	nameBG->SetTag("nameBG");
	nameBG->Load();
	explainBG = new Quad(L"Textures/UI/ShopSelectBack.png");
	explainBG->SetTag("explainBG");
	explainBG->Load();
	statusBG = new Quad(L"Textures/UI/ShopSelectBack.png");
	statusBG->SetTag("statusBG");
	statusBG->Load();

	itemBG = new Quad(L"Textures/UI/ShopSelectItem.png");
	itemBG->SetTag("itemBG");
	itemBG->Load();

	CAM->Load("StoreCamera");

	player = new ModelAnimator("Axia");
	player->SetLocalScale(0.05f, 0.05f, 0.05f);
	player->SetLocalRotation(0, XM_PI, 0);
	player->SetTag("StoreModel");
	player->Load("StoreModel");
	player->ReadClip("StandingIdle");
	player->PlayClip(0);

	storeModelRightHand = new Transform();

	SetWeapon();

	mainBtn = new Button(L"Textures/UI/MainBtn.png");
	mainBtn->SetTag("mainBtn");
	mainBtn->Load();
	mainBtn->SetEvent(bind(&StoreScene::ClickMainIcon, this));


}

StoreScene::~StoreScene()
{
	delete storeBG;
	delete storeSelectBG;
	delete itemBG;
	delete mainBtn;
	delete player;
	delete storeModelRightHand;
	delete nameBG;
	delete explainBG;
	delete statusBG;

	for (Item* item : items)
		delete item;
	for (Button* button : itemButtons)
		delete button;
}

void StoreScene::Update()
{
	storeBG->UpdateWorld();
	storeSelectBG->UpdateWorld();
	itemBG->UpdateWorld();
	nameBG->UpdateWorld();
	explainBG->UpdateWorld();
	statusBG->UpdateWorld();

	storeModelRightHand->SetWorld(player->GetTransformByNode(35));

	mainBtn->Update();
	player->Update();

	for (Button* button : itemButtons)
		button->Update();

	for (Item* item : items)
		item->Update();
}

void StoreScene::PreRender()
{
}

void StoreScene::Render()
{
	storeBG->Render();
	storeSelectBG->Render();

	player->Render();

	for (Item* item : items)
		item->Render();
}

void StoreScene::PostRender()
{
	nameBG->Render();
	explainBG->Render();
	statusBG->Render();
	mainBtn->Render();
	//itemBG->Render();
	for (Button* button : itemButtons)
		button->Render();

	ItemExplain();


}

void StoreScene::GUIRender()
{

	//nameBG->GUIRender();
	//explainBG->GUIRender();
	//statusBG->GUIRender();
	//
	//player->GUIRender();
	//itemBG->GUIRender();

	//for (Button* button : itemButtons)
	//	button->GUIRender();
	//
	//Font::Get()->RenderText(" 상점 (SHOP) ", { 170, 680 });
	//
	
		items[1]->GUIRender();
}

void StoreScene::ClickMainIcon()
{
	SCENE->Add("Start");
	SCENE->Remove("Store");
}

void StoreScene::ClickItem(void* panel)
{
	Item* item = (Item*)panel;

	if (item->Active())
	{
		renderedItem = nullptr;
		item->SetActive(false);
		player->Load("StoreModel");
	}
	else
	{
		renderedItem = item;
		item->SetActive(true);
		player->Load("StoreModelEquipRightHand");
	}

}

void StoreScene::SetWeapon()
{
	weaponName.push_back("BeamSaber");
	weaponName.push_back("Sword");

	itemButtons.resize(weaponName.size());
	items.resize(weaponName.size());

	DataManager::Get();
	for (int i = 0; i < weaponName.size(); i++)
	{
		items[i] = new Item(weaponName[i]);
		items[i]->SetTag("Store" + weaponName[i]);
		items[i]->Load("Store" + weaponName[i]);
		items[i]->SetActive(false);
		items[i]->SetParent(storeModelRightHand);
		items[i]->SetData(DataManager::Get()->GetItemDatas()[401 + i]);

		itemButtons[i] = new Button(L"Textures/UI/" + ToWString(weaponName[i]) + L"Item.png");
		itemButtons[i]->SetTag(weaponName[i] + "ItemBtn");
		itemButtons[i]->Load();
		itemButtons[i]->SetParamEvent(bind(&StoreScene::ClickItem, this, placeholders::_1));
		itemButtons[i]->SetObejct(items[i]);
	}



}

void StoreScene::ItemExplain()
{
	if (!renderedItem) return;

	Font::Get()->RenderText(renderedItem->GetData()->name, { nameBG->GetGlobalPosition().x, nameBG->GetGlobalPosition().y });
	Font::Get()->RenderText(renderedItem->GetData()->explain, { explainBG->GetGlobalPosition().x + 180, explainBG->GetGlobalPosition().y });
	Font::Get()->RenderText("가격 : " + to_string(renderedItem->GetData()->price), { statusBG->GetGlobalPosition().x - 90, statusBG->GetGlobalPosition().y + 50 });
	Font::Get()->RenderText("위력 : " + to_string(renderedItem->GetData()->power), { statusBG->GetGlobalPosition().x - 100, statusBG->GetGlobalPosition().y + 10 });



}
