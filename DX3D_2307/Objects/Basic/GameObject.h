#pragma once

class GameObject : public Transform
{
public:
    GameObject(wstring shaderFile = L"Light/Light.hlsl");
    ~GameObject();

    virtual void Render() {}
    virtual void SetRender();
    
    Material* GetMaterial() { return material; }

protected:
    Material* material;
    WorldBuffer* worldBuffer;
};