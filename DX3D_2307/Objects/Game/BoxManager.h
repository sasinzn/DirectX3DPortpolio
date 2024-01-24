#pragma once

class BoxManager : public Singleton<BoxManager>
{
private:
    friend class Singleton;

    BoxManager();
    ~BoxManager();

public:
    void Update();
    void Render();

    void CreateFloor(UINT x, UINT y, UINT z);

    float GetHeight(Vector3 pos);

    Box* GetCollision();
    bool AddBox(Box* box);

private:
    list<Box*> boxes;
    vector<wstring> textures;
};