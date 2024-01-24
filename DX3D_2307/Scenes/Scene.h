#pragma once

class Scene
{
public:
    virtual ~Scene() = default;

    virtual void Update() = 0;

    virtual void PreRender() = 0;
    virtual void Render() = 0;
    virtual void PostRender() = 0;
    virtual void GUIRender() = 0;

    virtual void Start() {}
    virtual void End() {}
    void SetActive(bool active) { isActive = active; }
    bool Active() { return isActive; }

    virtual void SetCamera() {}
private:
    bool isActive = false;
};