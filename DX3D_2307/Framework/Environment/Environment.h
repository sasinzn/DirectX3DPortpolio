#pragma once

class Environment : public Singleton<Environment>
{
private:
    friend class Singleton;

    Environment();
    ~Environment();

public:
    void Update();
    void GUIRender();

    void Set();
    void PostSet();

    void SetViewport(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);   

    Matrix GetProjection() { return perspective; }
    Camera* GetMainCamera() { return mainCamera; }
    LightBuffer::Light* GetLight(UINT index) { return &lightBuffer->GetData()->lights[index]; }
private:
    void CreateProjection();
    void CreateState();

    void SetPerspective();
    void SetOrthographic();

    void EditLight(LightBuffer::Light* light, int index);

private:
    Matrix perspective;    
    Matrix orthographic;
    MatrixBuffer* projectionBuffer;
    LightBuffer* lightBuffer;

    ViewBuffer* uiViewBuffer;

    Camera* mainCamera;

    SamplerState* samplerState;
    RasterizerState* rasterizerState[2];
    BlendState* blendState[2];
    DepthStencilState* depthStencilState[2];

    D3D11_VIEWPORT viewport;

    bool isWireMode = false;
};