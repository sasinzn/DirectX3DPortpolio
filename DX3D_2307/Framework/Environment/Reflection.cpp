#include "Framework.h"

Reflection::Reflection(Transform* target) : target(target)
{
    renderTarget = new RenderTarget(2048, 2048);
    depthStencil = new DepthStencil(2048, 2048);

    camera = new Camera();

    quad = new Quad(Float2(200, 200));
    quad->SetLocalPosition(100, 100, 0);
    Texture* texture = Texture::Add(L"Reflection", renderTarget->GetSRV());
    quad->GetMaterial()->SetDiffuseMap(texture);
    quad->UpdateWorld();
}

Reflection::~Reflection()
{
    delete renderTarget;
    delete depthStencil;
    delete camera;
    delete quad;
}

void Reflection::Update()
{
    Vector3 pos = CAM->GetLocalPosition();
    Vector3 rot = CAM->GetLocalRotation();

    pos.y = target->GetGlobalPosition().y * 2.0f - pos.y;
    rot.x *= -1.0f;

    camera->SetLocalPosition(pos);
    camera->SetLocalRotation(rot);

    camera->UpdateWorld();
}

void Reflection::SetReflection()
{
    renderTarget->Set(depthStencil);
    camera->SetView();
}

void Reflection::SetRender()
{
    camera->GetViewBuffer()->SetVS(10);
    DC->PSSetShaderResources(10, 1, &renderTarget->GetSRV());
}

void Reflection::PostRender()
{
    quad->Render();
}
