#include "Framework.h"

ProgressBar::ProgressBar(wstring frontImageFile, wstring backImageFile)
    : Quad(frontImageFile)
{
    material->SetShader(L"UI/ProgressBar.hlsl");

    backImage = Texture::Add(backImageFile);

    progressBuffer = new ProgressBuffer(1);
}

ProgressBar::~ProgressBar()
{
    delete progressBuffer;
}

void ProgressBar::Render()
{
   
    progressBuffer->SetPS(10);
    backImage->SetPS(10);

    Quad::Render();
}

