#include "Material.h"
#include "Framework.h"

Material::Material()
{
    SetShader(L"Light/Light.hlsl");

    diffuseMap = Texture::Add(L"Textures/Solid/White.png", L"DM");
    specularMap = Texture::Add(L"Textures/Solid/White.png", L"SM");
    normalMap = Texture::Add(L"Textures/Solid/White.png", L"NM");

    buffer = new MaterialBuffer();

    char path[128];
    GetCurrentDirectoryA(128, path);
    projectPath = path;
}

Material::Material(wstring shaderFile)
{
    diffuseMap = Texture::Add(L"Textures/Solid/White.png", L"DM");
    specularMap = Texture::Add(L"Textures/Solid/White.png", L"SM");
    normalMap = Texture::Add(L"Textures/Solid/White.png", L"NM");

    SetShader(shaderFile);

    buffer = new MaterialBuffer();

    char path[128];
    GetCurrentDirectoryA(128, path);
    projectPath = path;
}

Material::~Material()
{
    delete buffer;
}

void Material::Set()
{
    if (diffuseMap)
        diffuseMap->SetPS(0);

    if (specularMap)
        specularMap->SetPS(1);

    if (normalMap)
        normalMap->SetPS(2);

    buffer->SetPS(1);

    vertexShader->Set();
    pixelShader->Set();
}

void Material::GUIRender()
{
    string title = name + "_Material";

    if (ImGui::TreeNode(title.c_str()))
    {
        char str[128];
        strcpy_s(str, 128, editName.c_str());
        ImGui::PushItemWidth(100);
        ImGui::InputText("Name", str, 128);
        ImGui::PopItemWidth();
        editName = str;

        ImGui::SameLine();
        if (ImGui::Button("Edit"))
            name = editName;

        SelectShader();
        ImGui::SameLine();
        ImGui::Text(shaderFile.c_str());

        ImGui::ColorEdit3("Diffsue", (float*)&buffer->GetData()->diffuse);
        ImGui::ColorEdit3("Specular", (float*)&buffer->GetData()->specular);
        ImGui::ColorEdit3("Ambient", (float*)&buffer->GetData()->ambient);
        ImGui::ColorEdit4("Emissive", (float*)&buffer->GetData()->emissive);

        ImGui::SliderFloat("Shininess", &buffer->GetData()->shininess, 1, 50);

        SelectMap("DM", DIFFUSE);
        ImGui::SameLine();
        UnselectMap(DIFFUSE);

        SelectMap("SM", SPECULAR);
        ImGui::SameLine();
        UnselectMap(SPECULAR);

        SelectMap("NM", NORMAL);
        ImGui::SameLine();
        UnselectMap(NORMAL);

        SaveDialog();
        LoadDialog();

        ImGui::TreePop();
    }
}

void Material::SetShader(wstring shaderFile)
{
    this->shaderFile = ToString(shaderFile);

    vertexShader = Shader::AddVS(shaderFile);
    pixelShader = Shader::AddPS(shaderFile);
}

void Material::SetDiffuseMap(wstring textureFile)
{
    if (textureFile.length() > 0)
        diffuseMap = Texture::Add(textureFile);
    else
        diffuseMap = Texture::Add(L"Textures/Solid/WhiteDM.png");
}

void Material::SetSpecularMap(wstring textureFile)
{
    if (textureFile.length() > 0)
        specularMap = Texture::Add(textureFile);
    else
        specularMap = Texture::Add(L"Textures/Solid/WhiteSM.png");
}

void Material::SetNormalMap(wstring textureFile)
{
    if (textureFile.length() > 0)
    {
        normalMap = Texture::Add(textureFile);
        buffer->GetData()->hasNormalMap = 1;
    }
    else
    {
        normalMap = Texture::Add(L"Textures/Solid/WhiteNM.png");
        buffer->GetData()->hasNormalMap = 0;
    }
}

void Material::Save(string file)
{
    tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
    tinyxml2::XMLElement* material = document->NewElement("Material");
    material->SetAttribute("Name", name.c_str());
    material->SetAttribute("Shader", shaderFile.c_str());
    document->InsertFirstChild(material);

    tinyxml2::XMLElement* texture = document->NewElement("Texture");
    string textureFile = ToString(diffuseMap->GetFile());
    texture->SetAttribute("Diffuse", textureFile.c_str());
    textureFile = ToString(specularMap->GetFile());
    texture->SetAttribute("Specular", textureFile.c_str());
    textureFile = ToString(normalMap->GetFile());
    texture->SetAttribute("Normal", textureFile.c_str());
    material->InsertEndChild(texture);

    tinyxml2::XMLElement* property = document->NewElement("Property");
    tinyxml2::XMLElement* diffuse = document->NewElement("Diffuse");
    diffuse->SetAttribute("R", buffer->GetData()->diffuse.x);
    diffuse->SetAttribute("G", buffer->GetData()->diffuse.y);
    diffuse->SetAttribute("B", buffer->GetData()->diffuse.z);
    diffuse->SetAttribute("A", buffer->GetData()->diffuse.w);
    property->InsertEndChild(diffuse);

    tinyxml2::XMLElement* specular = document->NewElement("Specular");
    specular->SetAttribute("R", buffer->GetData()->specular.x);
    specular->SetAttribute("G", buffer->GetData()->specular.y);
    specular->SetAttribute("B", buffer->GetData()->specular.z);
    specular->SetAttribute("A", buffer->GetData()->specular.w);
    property->InsertEndChild(specular);

    tinyxml2::XMLElement* ambient = document->NewElement("Ambient");
    ambient->SetAttribute("R", buffer->GetData()->ambient.x);
    ambient->SetAttribute("G", buffer->GetData()->ambient.y);
    ambient->SetAttribute("B", buffer->GetData()->ambient.z);
    ambient->SetAttribute("A", buffer->GetData()->ambient.w);
    property->InsertEndChild(ambient);

    tinyxml2::XMLElement* emissive = document->NewElement("Emissive");
    emissive->SetAttribute("R", buffer->GetData()->emissive.x);
    emissive->SetAttribute("G", buffer->GetData()->emissive.y);
    emissive->SetAttribute("B", buffer->GetData()->emissive.z);
    emissive->SetAttribute("A", buffer->GetData()->emissive.w);
    property->InsertEndChild(emissive);

    property->SetAttribute("Shininess", buffer->GetData()->shininess);
    property->SetAttribute("HasNormalMap", buffer->GetData()->hasNormalMap);

    material->InsertEndChild(property);

    document->SaveFile(file.c_str());

    delete document;
}

void Material::Load(string file)
{
    this->file = file;

    tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
    document->LoadFile(file.c_str());
    tinyxml2::XMLElement* material = document->FirstChildElement();
    name = material->Attribute("Name");
    shaderFile = material->Attribute("Shader");
    SetShader(ToWString(shaderFile));

    tinyxml2::XMLElement* texture = material->FirstChildElement();
    wstring textureFile = ToWString(texture->Attribute("Diffuse"));
    SetDiffuseMap(textureFile);
    textureFile = ToWString(texture->Attribute("Specular"));
    SetSpecularMap(textureFile);
    textureFile = ToWString(texture->Attribute("Normal"));
    SetNormalMap(textureFile);

    tinyxml2::XMLElement* property = texture->NextSiblingElement();
    buffer->GetData()->shininess = property->FloatAttribute("Shininess");
    buffer->GetData()->hasNormalMap = property->IntAttribute("HasNormalMap");

    tinyxml2::XMLElement* diffuse = property->FirstChildElement();
    buffer->GetData()->diffuse.x = diffuse->FloatAttribute("R");
    buffer->GetData()->diffuse.y = diffuse->FloatAttribute("G");
    buffer->GetData()->diffuse.z = diffuse->FloatAttribute("B");
    buffer->GetData()->diffuse.w = diffuse->FloatAttribute("A");

    tinyxml2::XMLElement* specular = diffuse->NextSiblingElement();
    buffer->GetData()->specular.x = specular->FloatAttribute("R");
    buffer->GetData()->specular.y = specular->FloatAttribute("G");
    buffer->GetData()->specular.z = specular->FloatAttribute("B");
    buffer->GetData()->specular.w = specular->FloatAttribute("A");

    tinyxml2::XMLElement* ambient = specular->NextSiblingElement();
    buffer->GetData()->ambient.x = ambient->FloatAttribute("R");
    buffer->GetData()->ambient.y = ambient->FloatAttribute("G");
    buffer->GetData()->ambient.z = ambient->FloatAttribute("B");
    buffer->GetData()->ambient.w = ambient->FloatAttribute("A");

    tinyxml2::XMLElement* emissive = ambient->NextSiblingElement();
    buffer->GetData()->emissive.x = emissive->FloatAttribute("R");
    buffer->GetData()->emissive.y = emissive->FloatAttribute("G");
    buffer->GetData()->emissive.z = emissive->FloatAttribute("B");
    buffer->GetData()->emissive.w = emissive->FloatAttribute("A");

    delete document;
}

void Material::SelectShader()
{
    string key = "Shader";

    if (ImGui::Button(key.c_str()))
        DIALOG->OpenDialog(key.c_str(), key.c_str(), ".hlsl", ".");

    if (DIALOG->Display(key.c_str()))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();
            string path = projectPath + "/Shaders";
            file = file.substr(path.size() + 1, file.size());

            SetShader(ToWString(file));
        }

        DIALOG->Close();
    }
}

void Material::SelectMap(string name, MapType type)
{
    ImGui::SetWindowFontScale(2.0f);
    ImGui::TextColored(ImVec4(1, 0.5f, 0.8f, 1), name.c_str());
    ImGui::SetWindowFontScale(1.0f);
    ImGui::SameLine();

    ImTextureID textureID = nullptr;

    switch (type)
    {
    case Material::DIFFUSE:
        textureID = diffuseMap->GetSRV();
        break;
    case Material::SPECULAR:
        textureID = specularMap->GetSRV();
        break;
    case Material::NORMAL:
        textureID = normalMap->GetSRV();
        break;
    default:
        break;
    }

    if (ImGui::ImageButton(textureID, ImVec2(50, 50)))
        DIALOG->OpenDialog(this->name + name, name, ".png,.jpg,.tga", ".");

    if (DIALOG->Display(this->name + name))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            char path[128];
            GetCurrentDirectoryA(128, path);
            file = file.substr(strlen(path) + 1, file.size());

            switch (type)
            {
            case Material::DIFFUSE:
                SetDiffuseMap(ToWString(file));
                break;
            case Material::SPECULAR:
                SetSpecularMap(ToWString(file));
                break;
            case Material::NORMAL:
                SetNormalMap(ToWString(file));
                break;
            default:
                break;
            }
        }

        DIALOG->Close();
    }
}

void Material::UnselectMap(MapType type)
{
    ImTextureID textureID = nullptr;

    switch (type)
    {
    case Material::DIFFUSE:
        textureID = Texture::Add(L"Textures/UI/Cancel.png", L"DMCancel")->GetSRV();
        break;
    case Material::SPECULAR:
        textureID = Texture::Add(L"Textures/UI/Cancel.png", L"SMCancel")->GetSRV();
        break;
    case Material::NORMAL:
        textureID = Texture::Add(L"Textures/UI/Cancel.png", L"NMCancel")->GetSRV();
        break;
    default:
        break;
    }

    if (ImGui::ImageButton(textureID, ImVec2(20, 20)))
    {
        switch (type)
        {
        case Material::DIFFUSE:
            SetDiffuseMap(L"");
            break;
        case Material::SPECULAR:
            SetSpecularMap(L"");
            break;
        case Material::NORMAL:
            SetNormalMap(L"");
            break;
        default:
            break;
        }
    }
}

void Material::SaveDialog()
{
    string key = "Save";

    if (ImGui::Button("Save"))
    {
        if (file.empty())
            Save("TextData/Materials/" + name + ".mat");
        else
            Save(file);
    }

    ImGui::SameLine();

    if (ImGui::Button("SaveAs"))
        DIALOG->OpenDialog(key.c_str(), key.c_str(), ".mat", ".");

    if (DIALOG->Display(key.c_str()))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            Save(file);
        }

        DIALOG->Close();
    }
}

void Material::LoadDialog()
{
    string key = "Load";

    if (ImGui::Button("Load"))
        DIALOG->OpenDialog(key.c_str(), key.c_str(), ".mat", ".");

    if (DIALOG->Display(key.c_str()))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            Load(file);
        }

        DIALOG->Close();
    }
}

