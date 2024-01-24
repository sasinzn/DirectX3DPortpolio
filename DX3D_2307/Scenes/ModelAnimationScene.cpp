#include "Framework.h"
#include "ModelAnimationScene.h"

ModelAnimationScene::ModelAnimationScene()
{
    string name = "Axia";

    model = new ModelAnimator(name);
    //model->ReadClip("Dancing");
    //model->ReadClip("Idle");
    //model->ReadClip("RunF");
    //model->ReadClip("WalkF");
    //model->ReadClip("Dying");
    //model->ReadClip("Attack");
    //model->ReadClip("Swiping");
    //model->ReadClip("Reaction");
    model->ReadClip("GunIdle");
    model->ReadClip("GunFire");
    model->ReadClip("DrawingGun");

    model->PlayClip(0);
}

ModelAnimationScene::~ModelAnimationScene()
{
    delete model;
}

void ModelAnimationScene::Update()
{
    if (KEY->Down('1'))
        model->PlayClip(0);

    if (KEY->Down('2'))
        model->PlayClip(1);

    if (KEY->Down('3'))
        model->PlayClip(2);

    if (KEY->Down('4'))
        model->PlayClip(3);

    if (KEY->Down('5'))
        model->PlayClip(4);

    if (KEY->Down('6'))
        model->PlayClip(5);

    if (KEY->Down('7'))
        model->PlayClip(6);

    if (KEY->Down('8'))
        model->PlayClip(7);

    if (KEY->Down('9'))
        model->PlayClip(8);

    if (KEY->Down('0'))
        model->PlayClip(9);

    model->Update();
}

void ModelAnimationScene::PreRender()
{
}

void ModelAnimationScene::Render()
{
    model->Render();
}

void ModelAnimationScene::PostRender()
{
}

void ModelAnimationScene::GUIRender()
{
    model->GUIRender();
}
