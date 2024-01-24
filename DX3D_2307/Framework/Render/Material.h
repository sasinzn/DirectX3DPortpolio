#pragma once

class Material
{
private:
    enum MapType
    {
        DIFFUSE, SPECULAR, NORMAL
    };

public:
    Material();
    Material(wstring shaderFile);
    ~Material();

    void Set();

    void GUIRender();

    void SetShader(wstring shaderFile);
    void SetDiffuseMap(wstring textureFile);
    void SetDiffuseMap(Texture* texture) { diffuseMap = texture; }
    void SetSpecularMap(wstring textureFile);
    void SetNormalMap(wstring textureFile);    

    void Save(string file);
    void Load(string file);

    Texture* GetDiffuseMap() { return diffuseMap; }
    MaterialBuffer::Data* GetData() { return buffer->GetData(); }

    string GetName() { return name; }
    void SetName(string name) { this->name = name; }

private:
    void SelectShader();
    void SelectMap(string name, MapType type);
    void UnselectMap(MapType type);

    void SaveDialog();
    void LoadDialog();

private:
    string name;
    string editName;
    string file;
    string projectPath;
    string shaderFile;

    VertexShader* vertexShader;
    PixelShader* pixelShader;

    Texture* diffuseMap = nullptr;
    Texture* specularMap = nullptr;
    Texture* normalMap = nullptr;

    MaterialBuffer* buffer;
};