#pragma once

class SwordScene : public Scene
{
public:
    SwordScene();
    ~SwordScene();

    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    void ClickUIIcon(void* panel);    

private:
    Bot* bot;
    Terrain* terrain;

    Button* storeBtn;
    Button* invenBtn;

    Quad* store;
    Quad* inventory;
};