#pragma once

class Block : public Transform
{
public:
    Block(Vector3 size = Vector3(1, 1, 1));
    ~Block();

    void Update();
    void Render();
    void GUIRender();

    void Save(string file);
    void Load(string file);


private:
    void AddSide(float width, float height, Vector3 direction, Vector3 rotation);
    void SetSideTexture(string key, int index);

    void GUISave();
    void GUILoad();    

private:
    MatrixBuffer* worldBuffer;

    vector<Quad*> sides;
};