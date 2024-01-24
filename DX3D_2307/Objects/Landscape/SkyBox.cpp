#include "Framework.h"

SkyBox::SkyBox(wstring textureFile)
{
    material->SetShader(L"Landscape/SkyBox.hlsl");

    cubeMap = Texture::Add(textureFile);

    rasterizerState[0] = new RasterizerState();
    rasterizerState[1] = new RasterizerState();
    rasterizerState[1]->FrontCounterClockwise(true);

    depthState[0] = new DepthStencilState();
    depthState[1] = new DepthStencilState();
    depthState[1]->DepthEnable(false);
}

SkyBox::~SkyBox()
{
    delete rasterizerState[0];
    delete rasterizerState[1];

    delete depthState[0];
    delete depthState[1];
}

void SkyBox::Render()
{
    cubeMap->SetPS(10);

    rasterizerState[1]->SetState();
    depthState[1]->SetState();

    Sphere::Render();

    rasterizerState[0]->SetState();
    depthState[0]->SetState();
}
