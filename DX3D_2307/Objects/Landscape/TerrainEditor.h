#pragma once

class TerrainEditor : public GameObject
{
private:
    struct VertexType
    {
        Float3 pos;
        Float2 uv;
        Float3 normal;
        float alpha[4];

        VertexType()
            : pos(0, 0, 0), uv(0, 0), normal(0, 0, 0), alpha{}
        {
        }
    };

    enum EditType
    {
        HEIGHT, ALPHA
    };
    
    const float MAX_HEIGHT = 20.0f;
    const float MAX_SIZE = 100;

public:
    TerrainEditor();
    ~TerrainEditor();

    void Update();
    void Render();
    void GUIRender();

    void Picking();

private:
    void MakeMesh();
    void MakeNormal();

    void Resize();

    void AdjustHeight();
    void AdjustAlpha();

    void SaveHeightMap();
    void LoadHeightMap();

    void SaveAlphaMap();
    void LoadAlphaMap();

    void UpdateHeight();

private:
    Mesh<VertexType>* mesh;
    Texture* heightMap;
    Texture* secondMap;
    Texture* thirdMap;

    BrushBuffer* brushBuffer;

    string projectPath;
    UINT width, height;
    float adjustValue = 10.0f;    
    int selectMap = 0;
    EditType editType;
};