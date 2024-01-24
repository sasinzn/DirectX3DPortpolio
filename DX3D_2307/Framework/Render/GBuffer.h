#pragma once

class GBuffer
{
public:
    GBuffer();
    ~GBuffer();

    void SetMultiRenderTarget();
    void PostRender();
    void SetSRVs();

private:
    RenderTarget* diffuseTarget;
    RenderTarget* specularTarget;
    RenderTarget* normalTarget;

    DepthStencil* depthStencil;

    RenderTarget* rtvs[3];

    class Quad* quads[4];
    ID3D11ShaderResourceView* srvs[4];
};