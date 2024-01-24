#include "Framework.h"

Block::Block(Vector3 size)
{
    Vector3 halfSize = size * 0.5f;

    //front
    AddSide(size.x, size.y,
        Vector3::Back() * halfSize.z, Vector3());
    //back
    AddSide(size.x, size.y,
        Vector3::Forward() * halfSize.z, Vector3::Up() * XM_PI);
    //Right
    AddSide(size.z, size.y,
        Vector3::Right() * halfSize.x, Vector3::Down() * XM_PIDIV2);
    //Left
    AddSide(size.z, size.y,
        Vector3::Left() * halfSize.x, Vector3::Up() * XM_PIDIV2);
    //Up
    AddSide(size.x, size.z,
        Vector3::Up() * halfSize.y, Vector3::Right() * XM_PIDIV2);

    //Down
    AddSide(size.x, size.z,
        Vector3::Down() * halfSize.y, Vector3::Left() * XM_PIDIV2);
}

Block::~Block()
{
    for (Quad* side : sides)
        delete side;
}

void Block::Update()
{
    UpdateWorld();

    for (Quad* side : sides)
        side->UpdateWorld();
}

void Block::Render()
{
    if (!isActive) return;

    for (Quad* side : sides)
        side->Render();
}

void Block::GUIRender()
{
    if (ImGui::TreeNode(tag.c_str()))
    {
        Transform::GUIRender();

        SetSideTexture("Front", 0);
        SetSideTexture("Back", 1);
        SetSideTexture("Right", 2);
        SetSideTexture("Left", 3);
        SetSideTexture("Up", 4);
        SetSideTexture("Down", 5);

        GUISave();
        GUILoad();        

        ImGui::TreePop();
    }
    
}

void Block::AddSide(float width, float height, Vector3 direction, Vector3 rotation)
{
    Quad* side = new Quad(Float2(width, height));
    side->Translate(direction);
    side->Rotate(rotation);
    side->SetParent(this);
    side->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Box.png");
    sides.push_back(side);
}

void Block::SetSideTexture(string key, int index)
{
    if (ImGui::Button(key.c_str()))
        DIALOG->OpenDialog(key.c_str() , key.c_str(), ".png", ".");

    if (DIALOG->Display(key.c_str()))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();
            char path[128];
            GetCurrentDirectoryA(128, path);
            int size = strlen(path);

            file = file.substr(size + 1, file.size());

            sides[index]->GetMaterial()->SetDiffuseMap(ToWString(file));
        }

        DIALOG->Close();
    }
}

void Block::GUISave()
{
    string key = "Save";

    if (ImGui::Button(key.c_str()))
        DIALOG->OpenDialog(key.c_str(), key.c_str(), ".tex", ".");

    if (DIALOG->Display(key.c_str()))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();
            char path[128];
            GetCurrentDirectoryA(128, path);
            int size = strlen(path);

            file = file.substr(size + 1, file.size());

            Save(file);
        }

        DIALOG->Close();
    }
}

void Block::GUILoad()
{
    string key = "Load";

    if (ImGui::Button(key.c_str()))
        DIALOG->OpenDialog(key.c_str(), key.c_str(), ".tex", ".");

    if (DIALOG->Display(key.c_str()))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();
            char path[128];
            GetCurrentDirectoryA(128, path);
            int size = strlen(path);

            file = file.substr(size + 1, file.size());

            Load(file);
        }

        DIALOG->Close();
    }
}

void Block::Save(string file)
{
    BinaryWriter* writer = new BinaryWriter(file);

    for (Quad* side : sides)
    {
        writer->WString(side->GetMaterial()->GetDiffuseMap()->GetFile());
    }

    delete writer;
}

void Block::Load(string file)
{
    BinaryReader* reader = new BinaryReader(file);

    for (Quad* side : sides)
    {
        wstring file = reader->WString();

        side->GetMaterial()->SetDiffuseMap(file);
    }

    delete reader;
}
