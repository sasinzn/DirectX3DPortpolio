#pragma once

class GridScene : public Scene
{
public:
    GridScene();
    ~GridScene();
        
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    void MakeMesh();

private:
    float width, height;

    Material* material;
    Mesh<VertexColor>* mesh;    

    MatrixBuffer* worldBuffer;
};