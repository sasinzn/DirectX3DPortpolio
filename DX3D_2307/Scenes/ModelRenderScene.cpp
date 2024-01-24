#include "Framework.h"
#include "ModelRenderScene.h"

ModelRenderScene::ModelRenderScene()
{
    model = new Model("missileTower");
    //collider = new BoxCollider();
    //player = new Player();
}

ModelRenderScene::~ModelRenderScene()
{
    delete model;
    //delete collider;
    //delete player;
}

void ModelRenderScene::Update()
{
    model->UpdateWorld();
    //player->Update();
    //collider->UpdateWorld();
}

void ModelRenderScene::PreRender()
{
}

void ModelRenderScene::Render()
{
    model->Render();
    //player->Render();
    //collider->Render();
}

void ModelRenderScene::PostRender()
{
}

void ModelRenderScene::GUIRender()
{
    model->GUIRender();
    //player->GUIRender();
    //collider->GUIRender();
}
