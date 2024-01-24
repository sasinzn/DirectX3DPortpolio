#include "Framework.h"
#include "MainScene.h"

MainScene::MainScene()
{
	mainBG = new Quad(L"Textures/UI/mainBackResize.png");
	mainBG->SetTag("mainBG");
	mainBG->Load();

	menuBG = new Quad(L"Textures/UI/menuBack.png");
	menuBG->SetTag("menuBG");
	menuBG->Load();
	
	CAM->Load("StoreCamera");



	storeBtn = new Button(L"Textures/UI/ShopBtn.png");
	storeBtn->SetTag("StoreBtn");
	storeBtn->Load();
	storeBtn->SetEvent(bind(&MainScene::ClickStoreIcon, this));
	//storeBtn->SetParamEvent(bind(&SwordScene::ClickUIIcon, this, placeholders::_1));
	//storeBtn->SetObejct(store);

	equipBtn = new Button(L"Textures/UI/EquipBtn.png");
	equipBtn->SetTag("EquipBtn");
	equipBtn->Load();
	//equipBtn->SetParamEvent(bind(&SwordScene::ClickUIIcon, this, placeholders::_1));
	//equipBtn->SetObejct(inventory);

	missionBtn = new Button(L"Textures/UI/MissionBtn.png");
	missionBtn->SetTag("MissionBtn");
	missionBtn->Load();
	missionBtn->SetEvent(bind(&MainScene::ClickMissionIcon, this));
}

MainScene::~MainScene()
{
	delete mainBG;
	delete menuBG;
	delete equipBtn;
	delete storeBtn;
	delete missionBtn;
}

void MainScene::Update()
{
	mainBG->UpdateWorld();
	menuBG->UpdateWorld();

	equipBtn->Update();
	storeBtn->Update();
	missionBtn->Update();
}

void MainScene::PreRender()
{
}

void MainScene::Render()
{
	mainBG->Render();
}

void MainScene::PostRender()
{
	menuBG->Render();
	storeBtn->Render();
	equipBtn->Render();
	missionBtn->Render();
}

void MainScene::GUIRender()
{
	mainBG->GUIRender();
	menuBG->GUIRender();
	storeBtn->GUIRender();
	equipBtn->GUIRender();
	missionBtn->GUIRender();
}

void MainScene::ClickStoreIcon()
{
	SCENE->Add("Store");
	SCENE->Remove("Start");
}

void MainScene::ClickEquipIcon()
{
}

void MainScene::ClickMissionIcon()
{
	SCENE->ActiveScene("Mission");
	SCENE->Add("Mission");
	SCENE->GetScene("Mission")->SetCamera();
	SCENE->Remove("Start");
}

