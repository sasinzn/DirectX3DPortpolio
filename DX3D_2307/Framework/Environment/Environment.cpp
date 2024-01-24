#include "Framework.h"

Environment::Environment()
{
    SetViewport();
    CreateProjection();
    CreateState();

    mainCamera = new Camera();
    mainCamera->Load();

    lightBuffer = new LightBuffer();
    uiViewBuffer = new ViewBuffer();
}

Environment::~Environment()
{
    mainCamera->Save();

    delete projectionBuffer;
    delete mainCamera;

    delete samplerState;
    delete rasterizerState[0];
    delete rasterizerState[1];

    delete lightBuffer;
    delete uiViewBuffer;
}

void Environment::Update()
{
    if (KEY->Down(VK_F1))
        isWireMode = !isWireMode;

    mainCamera->Update();
}

void Environment::GUIRender()
{
    mainCamera->GUIRender();

    if (ImGui::TreeNode("LightOption"))
    {
        if (ImGui::Button("Add"))
            lightBuffer->GetData()->lightCount++;

        FOR(lightBuffer->GetData()->lightCount)
        {
            EditLight(&lightBuffer->GetData()->lights[i], i);
        }        

        ImGui::ColorEdit3("AmbientLight", (float*)&lightBuffer->GetData()->ambientLight);
        ImGui::ColorEdit3("AmbientCeil", (float*)&lightBuffer->GetData()->ambientCeil);

        ImGui::TreePop();
    }    
}

void Environment::Set()
{
    mainCamera->SetView();
    rasterizerState[isWireMode]->SetState();    
    blendState[0]->SetState();
    depthStencilState[0]->SetState();

    lightBuffer->SetPS(0);

    SetPerspective();    
}

void Environment::PostSet()
{
    blendState[1]->SetState();
    depthStencilState[1]->SetState();

    uiViewBuffer->SetVS(1);
    SetOrthographic();
}

void Environment::SetViewport(UINT width, UINT height)
{
    //ViewPort 변환 : VS에서 뽑은 NDC좌표를 Screen좌표로 변환하는 과정    
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    DC->RSSetViewports(1, &viewport);
}

void Environment::CreateProjection()
{
    projectionBuffer = new MatrixBuffer();    

    perspective = XMMatrixPerspectiveFovLH(XM_PIDIV4,
        WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 1000.0f);
    projectionBuffer->Set(perspective);
    projectionBuffer->SetVS(2);
    projectionBuffer->SetPS(2);

    orthographic = XMMatrixOrthographicOffCenterLH(
        0.0f, WIN_WIDTH, 0.0f, WIN_HEIGHT, -1.0f, 1.0f);
}

void Environment::CreateState()
{
    samplerState = new SamplerState();
    samplerState->SetState();

    rasterizerState[0] = new RasterizerState();
    rasterizerState[1] = new RasterizerState();
    rasterizerState[1]->FillMode(D3D11_FILL_WIREFRAME);

    blendState[0] = new BlendState();
    blendState[1] = new BlendState();
    blendState[1]->Alpha(true);    

    depthStencilState[0] = new DepthStencilState();
    depthStencilState[1] = new DepthStencilState();
    depthStencilState[1]->DepthEnable(false);
}

void Environment::SetPerspective()
{
    projectionBuffer->Set(perspective);
    projectionBuffer->SetVS(2);
}

void Environment::SetOrthographic()
{
    projectionBuffer->Set(orthographic);
    projectionBuffer->SetVS(2);
}

void Environment::EditLight(LightBuffer::Light* light, int index)
{
    string label = "Light_" + to_string(index);

    if (ImGui::TreeNode(label.c_str()))
    {
        ImGui::Checkbox("Active", (bool*)&light->active);

        const char* list[] = { "Directional", "Point", "Spot", "Capsule" };
        ImGui::Combo("Type", &light->type, list, 4);

        ImGui::ColorEdit3("Color", (float*)&light->color);
        ImGui::SliderFloat3("Direction",
            (float*)&light->direction, -1, 1);

        ImGui::DragFloat3("Position", (float*)&light->position);
        ImGui::SliderFloat("Range", &light->range, 1, 1000);

        ImGui::SliderFloat("Inner", &light->inner, 0, light->outer);
        ImGui::SliderFloat("Outer", &light->outer, light->inner, 180.0f);

        ImGui::SliderFloat("Length", &light->length, 0, 500.0f);

        ImGui::TreePop();
    }
}
