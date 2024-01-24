#pragma once

class Model : public Transform
{
public:
    Model(string name);
    ~Model();

    void Render();
    void GUIRender();

    void SetShader(wstring file);

    void SetBox(Vector3& minBox, Vector3& maxBox);

    Material* GetMaterial(UINT index) { return materials[index]; }
private:
    void ReadMaterial();
    void ReadMesh();
    void MakeBoneTransforms();

    void SaveMaterials();

protected:
    string name;
    vector<NodeData> nodes;
    vector<BoneData> bones;

    map<string, UINT> boneMap;

    vector<Material*> materials;
    vector<ModelMesh*> meshes;

    WorldBuffer* worldBuffer;

    Matrix boneTransforms[MAX_BONE];
    map<string, Matrix> nodeTransforms;
};