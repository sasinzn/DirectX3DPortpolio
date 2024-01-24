#include "Framework.h"
#include "ModelInstancingScene.h"

ModelInstancingScene::ModelInstancingScene()
{
    modelInstancing = new ModelAnimatorInstancing("Mutant");
    modelInstancing->ReadClip("Idle");
    modelInstancing->ReadClip("RunF");
    modelInstancing->ReadClip("Attack");

    for (float z = 0; z < 10; z++)
    {
        for (float x = 0; x < 10; x++)
        {
            Transform* transform = modelInstancing->Add();
            transform->SetLocalPosition(x, 0, z);
            float scale = Random(0.005f, 0.01f);
            transform->SetLocalScale(scale, scale, scale);
        }
    }
}

ModelInstancingScene::~ModelInstancingScene()
{
    delete modelInstancing;
}

void ModelInstancingScene::Update()
{
    modelInstancing->Update();
}

void ModelInstancingScene::PreRender()
{
}

void ModelInstancingScene::Render()
{
    modelInstancing->Render();
}

void ModelInstancingScene::PostRender()
{
}

void ModelInstancingScene::GUIRender()
{
    ImGui::SliderInt("Index", &instanceIndex, 0, 100);
    ImGui::SliderInt("Clip", &clip, 0, 2);

    if (ImGui::Button("Play"))
        modelInstancing->PlayClip(instanceIndex, clip);

    modelInstancing->GUIRender();
}
