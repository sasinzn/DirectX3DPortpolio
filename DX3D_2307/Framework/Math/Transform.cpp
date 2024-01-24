#include "Framework.h"

Transform::Transform()
{
    world = XMMatrixIdentity();
}

void Transform::UpdateWorld()
{
    world = XMMatrixTransformation(pivot,
        XMQuaternionIdentity(), localScale, pivot,
        XMQuaternionRotationRollPitchYawFromVector(localRotation),
        localPosition);

    if (parent)
        world *= parent->GetWorld();

    XMStoreFloat4x4(&matWorld, world);
    right = Vector3(matWorld._11, matWorld._12, matWorld._13);
    up = Vector3(matWorld._21, matWorld._22, matWorld._23);
    forward = Vector3(matWorld._31, matWorld._32, matWorld._33);
        
    XMMatrixDecompose(globalScale.GetValue(),
        globalRotation.GetValue(), globalPosition.GetValue(), world);
}

void Transform::GUIRender()
{
    if (ImGui::TreeNode((tag + "_Transform").c_str()))
    {
        char str[128];
        strcpy_s(str, 128, editTag.c_str());
        ImGui::PushItemWidth(100);
        ImGui::InputText("Name", str, 128);
        ImGui::PopItemWidth();
        editTag = str;

        ImGui::SameLine();
        if (ImGui::Button("Edit"))
            tag = editTag;

        ImGui::Checkbox("Active", &isActive);

        string temp = tag + "_Pos";
        ImGui::DragFloat3(temp.c_str(), (float*)&localPosition, 0.1f);

        temp = tag + "_Rot";
        Vector3 degree = localRotation * 180.0f / XM_PI;

        ImGui::DragFloat3(temp.c_str(), (float*)&degree, 1.0f, -180, 180);

        localRotation = degree / 180.0f * XM_PI;

        temp = tag + "_Scale";
        ImGui::DragFloat3(temp.c_str(), (float*)&localScale, 0.1f);

        if (ImGui::Button("Save"))
            Save();

        ImGui::SameLine();

        if (ImGui::Button("Load"))
            Load();

        ImGui::TreePop();
    }
}

Transform* Transform::GetParent()
{
    if (parent != nullptr)
        return parent->GetParent();

    return this;
}

void Transform::Translate(Vector3 direction)
{
    localPosition += direction;
}

void Transform::Rotate(Vector3 direction)
{
    localRotation += direction;
}

bool Transform::Active()
{
    if (parent == nullptr)
        return isActive;

    if (isActive == false)
        return false;

    return parent->Active();
}

void Transform::Save()
{
    BinaryWriter* writer = new BinaryWriter("TextData/Transforms/" + tag + ".srt");

    writer->Vector(localPosition);
    writer->Vector(localRotation);
    writer->Vector(localScale);
    writer->Vector(pivot);

    delete writer;
}

void Transform::Load()
{
    BinaryReader* reader = new BinaryReader("TextData/Transforms/" + tag + ".srt");

    if (reader->IsFailed()) return;

    localPosition = reader->Vector();
    localRotation = reader->Vector();
    localScale = reader->Vector();
    pivot = reader->Vector();

    UpdateWorld();

    delete reader;
}

void Transform::Load(string tag)
{
    BinaryReader* reader = new BinaryReader("TextData/Transforms/" + tag + ".srt");

    if (reader->IsFailed()) return;

    localPosition = reader->Vector();
    localRotation = reader->Vector();
    localScale = reader->Vector();
    pivot = reader->Vector();

    UpdateWorld();

    delete reader;
}
