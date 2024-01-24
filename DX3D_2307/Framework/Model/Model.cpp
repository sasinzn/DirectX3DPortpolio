#include "Framework.h"

Model::Model(string name) : name(name)
{
    SetTag(name);

    ReadMaterial();
    ReadMesh();

    worldBuffer = new WorldBuffer();
}

Model::~Model()
{
    for (Material* material : materials)
        delete material;

    for (ModelMesh* mesh : meshes)
        delete mesh;

    delete worldBuffer;
}

void Model::Render()
{
    worldBuffer->Set(world);
    worldBuffer->SetVS(0);

    for (ModelMesh* mesh : meshes)
        mesh->Render();
}

void Model::GUIRender()
{
    Transform::GUIRender();

    for (Material* material : materials)
        material->GUIRender();

    if (ImGui::Button("SaveMaterials"))
    {
        SaveMaterials();
    }
}

void Model::SetShader(wstring file)
{
    for (Material* material : materials)
        material->SetShader(file);
}

void Model::SetBox(Vector3& minBox, Vector3& maxBox)
{
    meshes[0]->SetBox(minBox, maxBox);

    for (UINT i = 1; i < meshes.size(); i++)
    {
        Vector3 minPos, maxPos;

        meshes[1]->SetBox(minPos, maxPos);

        minBox.x = min(minBox.x, minPos.x);
        minBox.y = min(minBox.y, minPos.y);
        minBox.z = min(minBox.z, minPos.z);

        maxBox.x = max(maxBox.x, minPos.x);
        maxBox.y = max(maxBox.y, minPos.y);
        maxBox.z = max(maxBox.z, minPos.z);
    }
}

void Model::ReadMaterial()
{
    string file = "Models/Materials/" + name + "/" + name + ".mats";

    BinaryReader* reader = new BinaryReader(file);

    if (reader->IsFailed()) assert(false);

    UINT size = reader->UInt();

    materials.reserve(size);

    FOR(size)
    {
        Material* material = new Material();
        material->Load(reader->String());
        materials.push_back(material);
    }

    delete reader;
}

void Model::ReadMesh()
{
    string file = "Models/Meshes/" + name + ".mesh";

    BinaryReader* reader = new BinaryReader(file);

    if (reader->IsFailed()) assert(false);

    UINT size = reader->UInt();

    meshes.reserve(size);

    FOR(size)
    {
        ModelMesh* mesh = new ModelMesh(reader->String());
        mesh->SetMaterial(materials[reader->UInt()]);

        UINT vertexCount = reader->UInt();
        ModelVertex* vertices = new ModelVertex[vertexCount];
        reader->Byte((void**)&vertices, sizeof(ModelVertex) * vertexCount);

        UINT indexCount = reader->UInt();
        UINT* indices = new UINT[indexCount];
        reader->Byte((void**)&indices, sizeof(UINT) * indexCount);

        mesh->CreateMesh(vertices, vertexCount, indices, indexCount);

        meshes.push_back(mesh);
    }

    size = reader->UInt();
    nodes.resize(size);
    for (NodeData& node : nodes)
    {
        node.index = reader->Int();
        node.name = reader->String();
        node.parent = reader->Int();
        node.transform = reader->Matrix();
    }

    size = reader->UInt();
    bones.resize(size);
    for (BoneData& bone : bones)
    {
        bone.index = reader->Int();
        bone.name = reader->String();
        bone.offset = reader->Matrix();

        boneMap[bone.name] = bone.index;
    }

    delete reader;

    MakeBoneTransforms();
}

void Model::MakeBoneTransforms()
{
    UINT nodeIndex = 0;

    for (NodeData node : nodes)
    {
        Matrix parent;
        int parentIndex = node.parent;

        if (parentIndex < 0)
            parent = XMMatrixIdentity();
        else
            parent = boneTransforms[parentIndex];

        boneTransforms[nodeIndex] = node.transform * parent;
        nodeTransforms[node.name] = boneTransforms[nodeIndex];

        nodeIndex++;
    }

    for (ModelMesh* mesh : meshes)
    {
        Matrix transform = nodeTransforms[mesh->GetName()];
        mesh->GetBone()->Set(transform);
    }
}

void Model::SaveMaterials()
{
    string savePath = "Models/Materials/" + name + "/";
    string file = name + ".mats";

    CreateFolders(savePath);

    BinaryWriter* writer = new BinaryWriter(savePath + file);

    writer->UInt(materials.size());

    for (Material* material : materials)
    {
        string path = savePath + material->GetName() + ".mat";
        material->Save(path);

        writer->String(path);
    }

    delete writer;
}

