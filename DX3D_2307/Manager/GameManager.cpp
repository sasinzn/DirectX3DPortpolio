#include "Framework.h"

#include "Scenes/GridScene.h"
#include "Scenes/TerrainScene.h"
#include "Scenes/TerrainEditorScene.h"
#include "Scenes/CollisionScene.h"
#include "Scenes/MineCraftScene.h"
#include "Scenes/ModelExportScene.h"
#include "Scenes/ModelRenderScene.h"
#include "Scenes/ModelAnimationScene.h"
#include "Scenes/SwordScene.h"
#include "Scenes/InstancingScene.h"
#include "Scenes/ModelInstancingScene.h"
#include "Scenes/LightScene.h"
#include "Scenes/RenderTargetScene.h"
#include "Scenes/AStarScene.h"
#include "Scenes/SwordScene.h"
#include "Scenes/ParticleScene.h"
#include "Scenes/ParticleToolScene.h"
#include "Scenes/StoreScene.h"
#include "Scenes/MainScene.h"
#include "Scenes/Mission1Scene.h"
#include "Scenes/BuildTestScene.h"


GameManager::GameManager()
{
    Create();

    SCENE->Create("Grid", new GridScene());
    //SCENE->Create("Export", new ModelExportScene());
    //SCENE->Create("Start", new TextureScene());
    //SCENE->Create("Start", new CubeScene());
    //SCENE->Create("Start", new RobotScene());
    //SCENE->Create("Start", new TerrainScene());
    //SCENE->Create("Start", new TerrainEditorScene());
    //SCENE->Create("Start", new CarScene());
    //SCENE->Create("Start", new CollisionScene());
    //SCENE->Create("Start", new MineCraftScene());
    //SCENE->Create("Start", new ModelRenderScene());
    //SCENE->Create("Start", new ModelAnimationScene());
    //SCENE->Create("Start", new SwordScene());
    //SCENE->Create("Start", new InstancingScene());
    //SCENE->Create("Start", new ModelInstancingScene());
    //SCENE->Create("Start", new LightScene());
    //SCENE->Create("Start", new RenderTargetScene());
    //SCENE->Create("Start", new DeferredScene());
    //SCENE->Create("Start", new AStarScene());
    //SCENE->Create("Start", new BuildTestScene());
    //SCENE->Create("Start", new ParticleScene());
    //SCENE->Create("Start", new ParticleToolScene());
    SCENE->Create("Mission", new Mission1Scene());
    SCENE->Create("Store", new StoreScene());
    SCENE->Create("Start", new MainScene());

    SCENE->Add("Grid");    
    SCENE->Add("Start");
}

GameManager::~GameManager()
{
    Delete();
}

void GameManager::Update()
{
    Keyboard::Get()->Update();
    Timer::Get()->Update();

    SCENE->Update();

    Environment::Get()->Update();
}

void GameManager::Render()
{
    SCENE->PreRender();

    Device::Get()->Clear();

    Environment::Get()->Set();
    Environment::Get()->SetViewport();
    SCENE->Render();

    Environment::Get()->PostSet();
    Font::Get()->GetDC()->BeginDraw();

    SCENE->PostRender();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
    ImGui::Text(fps.c_str());

    Environment::Get()->GUIRender();
    SCENE->GUIRender();

    Font::Get()->GetDC()->EndDraw();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    Device::Get()->Present();
}

void GameManager::Create()
{
    Device::Get();
    Keyboard::Get();
    Timer::Get();
    Environment::Get();
    SceneManager::Get();
    Font::Get();

    Font::Get()->AddColor("White", 1, 1, 1);
    Font::Get()->AddStyle("Default", L"±Ã¼­Ã¼");

    Font::Get()->SetColor("White");
    Font::Get()->SetStyle("Default");

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(DEVICE, DC);
}

void GameManager::Delete()
{
    Device::Delete();
    Keyboard::Delete();
    Timer::Delete();
    Environment::Delete();
    SceneManager::Delete();
    Shader::Delete();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();
}
