#include "Framework.h"

GameObject::GameObject(wstring shaderFile)
{
    material = new Material(shaderFile);
    worldBuffer = new WorldBuffer();
}

GameObject::~GameObject()
{
    delete material;
    delete worldBuffer;
}

void GameObject::SetRender()
{
    worldBuffer->Set(world);
    worldBuffer->SetVS(0);

    material->Set();
}
