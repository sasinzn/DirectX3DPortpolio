#pragma once

class Mission1Scene : public Scene
{
public:
    Mission1Scene();
    ~Mission1Scene();

    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

public:
    virtual void SetCamera() override;

private:
    void CreateDest();
    void ClickUIIcon(void* panel);
private:
    Player* player;
    Terrain* terrain;
    SkyBox* skyBox;

    AStar* aStar;

    Quad* dest[4];
   
};