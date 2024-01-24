#include "Framework.h"

GBuffer::GBuffer()
{
    diffuseTarget = new RenderTarget();
    specularTarget = new RenderTarget();
    normalTarget = new RenderTarget();

    depthStencil = new DepthStencil(WIN_WIDTH, WIN_HEIGHT, true);

    rtvs[0] = diffuseTarget;
    rtvs[1] = specularTarget;
    rtvs[2] = normalTarget;

    srvs[0] = depthStencil->GetSRV();
    srvs[1] = diffuseTarget->GetSRV();
    srvs[2] = specularTarget->GetSRV();
    srvs[3] = normalTarget->GetSRV();

    FOR(4)
    {
        quads[i] = new Quad(Float2(200, 200));
        quads[i]->SetLocalPosition(100 + i * 200, 100, 0);
        Texture* texture = Texture::Add(L"G" + to_wstring(i), srvs[i]);
        quads[i]->GetMaterial()->SetDiffuseMap(texture);
        quads[i]->UpdateWorld();
    }
}

GBuffer::~GBuffer()
{
    delete diffuseTarget;
    delete specularTarget;
    delete normalTarget;
    delete depthStencil;

    for (Quad* quad : quads)
        delete quad;
}

void GBuffer::SetMultiRenderTarget()
{
    RenderTarget::SetMulti(rtvs, 3, depthStencil, Float4(1, 1, 0, 1));
}

void GBuffer::PostRender()
{
    for (Quad* quad : quads)
        quad->Render();
}

void GBuffer::SetSRVs()
{
    FOR(4)
    {
        DC->PSSetShaderResources(10 + i, 1, &srvs[i]);
    }
}
