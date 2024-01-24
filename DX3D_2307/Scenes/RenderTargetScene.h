#pragma once

class RenderTargetScene : public Scene
{
public:
    RenderTargetScene();
    ~RenderTargetScene();

    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    void CreateObjects();

private:
    RenderTarget* renderTarget;
    DepthStencil* depthStencil;

    Quad* quad;
    Quad* quad2;

    Sphere* sphere;
    Model* model;
    Bot* bot;

    FloatValueBuffer* valueBuffer;
};