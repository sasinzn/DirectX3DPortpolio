#include "Framework.h"

TerrainEditor::TerrainEditor()
    : GameObject(L"Landscape/TerrainEditor.hlsl"), width(MAX_SIZE), height(MAX_SIZE)
{
    material->SetDiffuseMap(L"Textures/Landscape/Dirt2.png");
    
    secondMap = Texture::Add(L"Textures/Landscape/Stones.png");
    thirdMap = Texture::Add(L"Textures/Landscape/Wall.png");

    mesh = new Mesh<VertexType>();
    MakeMesh();
    MakeNormal();
    mesh->CreateMesh();

    brushBuffer = new BrushBuffer();

    char path[128];
    GetCurrentDirectoryA(128, path);
    projectPath = path;
}

TerrainEditor::~TerrainEditor()
{
    delete mesh;
    delete brushBuffer;
}

void TerrainEditor::Update()
{
    Picking();

    if (KEY->Press(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
    {
        switch (editType)
        {
        case TerrainEditor::HEIGHT:
            AdjustHeight();
            break;
        case TerrainEditor::ALPHA:
            AdjustAlpha();
            break;
        default:
            break;
        }        
    }
}

void TerrainEditor::Render()
{
    brushBuffer->SetPS(10);
    secondMap->SetPS(11);
    thirdMap->SetPS(12);

    SetRender();
    mesh->Draw();
}

void TerrainEditor::GUIRender()
{
    ImGui::Text("TerrainEdit Option");
    ImGui::Text("x : %.1f, y : %.1f, z : %.1f",
        brushBuffer->GetData()->pickingPos.x,
        brushBuffer->GetData()->pickingPos.y,
        brushBuffer->GetData()->pickingPos.z);

    if (ImGui::DragInt("Width", (int*)&width, 1.0f, 2, MAX_SIZE))
        Resize();
    if (ImGui::DragInt("Height", (int*)&height, 1.0f, 2, MAX_SIZE))
        Resize();

    const char* editList[] = { "Height", "Alpha" };
    ImGui::Combo("EditType", (int*)&editType, editList, 2);

    ImGui::SliderFloat("BrushRange", &brushBuffer->GetData()->range, 1, 30);
    ImGui::ColorEdit3("BrushColor", (float*)&brushBuffer->GetData()->color);

    ImGui::SliderFloat("AdjustValue", &adjustValue, -50, 50);
    ImGui::SliderInt("SelectMap", &selectMap, 0, 1);

    SaveHeightMap();
    ImGui::SameLine();
    LoadHeightMap();

    SaveAlphaMap();
    ImGui::SameLine();
    LoadAlphaMap();
}

void TerrainEditor::Picking()
{
    Ray ray = CAM->ScreenPointToRay(mousePos);

    for (UINT z = 0; z < height - 1; z++)
    {
        for (UINT x = 0; x < width - 1; x++)
        {
            UINT index[4];
            index[0] = width * z + x;
            index[1] = width * z + x + 1;
            index[2] = width * (z + 1) + x;
            index[3] = width * (z + 1) + x + 1;

            vector<VertexType>& vertices = mesh->GetVertices();

            Vector3 p[4];
            FOR(4)
                p[i] = vertices[index[i]].pos;

            float distance = 0.0f;
            if (Intersects(ray.origin, ray.direction, p[0], p[1], p[2], distance))
            {
                brushBuffer->GetData()->pickingPos = ray.origin + ray.direction * distance;
                return;
            }
            if (Intersects(ray.origin, ray.direction, p[3], p[1], p[2], distance))
            {
                brushBuffer->GetData()->pickingPos = ray.origin + ray.direction * distance;
                return;
            }
        }
    }
}

void TerrainEditor::MakeMesh()
{
    vector<Float4> pixels(width * height, Float4(0, 0, 0, 0));

    if (heightMap)
    {
        width = (UINT)heightMap->GetSize().x;
        height = (UINT)heightMap->GetSize().y;

        heightMap->ReadPixels(pixels);
    }    

    //Vertices
    vector<VertexType>& vertices = mesh->GetVertices();
    vertices.clear();

    vertices.reserve(width * height);
    for (UINT z = 0; z < height; z++)
    {
        for (UINT x = 0; x < width; x++)
        {
            VertexType vertex;
            vertex.pos = Vector3(x, 0.0f, height - z - 1);
            //vertex.pos = Vector3(x, 0.0f, z);
            vertex.uv.x = x / (float)(width - 1);
            vertex.uv.y = z / (float)(height - 1);

            UINT index = width * z + x;
            vertex.pos.y = pixels[index].x * MAX_HEIGHT;

            vertices.push_back(vertex);
        }
    }

    //Indices
    vector<UINT>& indices = mesh->GetIndices();
    indices.clear();

    indices.reserve((width - 1) * (height - 1) * 6);
    for (UINT z = 0; z < height - 1; z++)
    {
        for (UINT x = 0; x < width - 1; x++)
        {
            indices.push_back(width * z + x);//0                
            indices.push_back(width * z + x + 1);//2                        
            indices.push_back(width * (z + 1) + x);//1

            indices.push_back(width * (z + 1) + x);//1            
            indices.push_back(width * z + x + 1);//2                        
            indices.push_back(width * (z + 1) + x + 1);//3
        }
    }
}

void TerrainEditor::MakeNormal()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    vector<UINT>& indices = mesh->GetIndices();

    FOR(indices.size() / 3)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        Vector3 v0 = vertices[index0].pos;
        Vector3 v1 = vertices[index1].pos;
        Vector3 v2 = vertices[index2].pos;

        Vector3 e0 = v1 - v0;
        Vector3 e1 = v2 - v0;

        Vector3 normal = Cross(e0, e1).GetNormalized();

        vertices[index0].normal = normal + vertices[index0].normal;
        vertices[index1].normal = normal + vertices[index1].normal;
        vertices[index2].normal = normal + vertices[index2].normal;
    }
}

void TerrainEditor::Resize()
{
    MakeMesh();
    MakeNormal();

    mesh->UpdateVertex();
    mesh->UpdateIndex();
}

void TerrainEditor::AdjustHeight()
{
    vector<VertexType>& vertices = mesh->GetVertices();

    for (VertexType& vertex : vertices)
    {
        Vector3 pos = Vector3(vertex.pos.x, 0.0f, vertex.pos.z);
        Vector3 pickingPos = brushBuffer->GetData()->pickingPos;
        pickingPos.y = 0.0f;

        float distance = Distance(pos, pickingPos);

        if (distance <= brushBuffer->GetData()->range)
        {
            vertex.pos.y += adjustValue * DELTA;
            vertex.pos.y = Clamp(0.0f, MAX_HEIGHT, vertex.pos.y);
        }
    }

    UpdateHeight();
}

void TerrainEditor::AdjustAlpha()
{
    vector<VertexType>& vertices = mesh->GetVertices();

    for (VertexType& vertex : vertices)
    {
        Vector3 pos = Vector3(vertex.pos.x, 0.0f, vertex.pos.z);
        Vector3 pickingPos = brushBuffer->GetData()->pickingPos;
        pickingPos.y = 0.0f;

        float distance = Distance(pos, pickingPos);

        if (distance <= brushBuffer->GetData()->range)
        {
            vertex.alpha[selectMap] += adjustValue * DELTA;
            vertex.alpha[selectMap] = Clamp(0.0f, 1.0f, vertex.alpha[selectMap]);
        }
    }

    mesh->UpdateVertex();
}

void TerrainEditor::SaveHeightMap()
{
    if (ImGui::Button("SaveHeight"))
        DIALOG->OpenDialog("SaveHeight", "SaveHeight", ".png", ".");

    if (DIALOG->Display("SaveHeight"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            UINT size = width * height * 4;
            uint8_t* pixels = new uint8_t[size];

            vector<VertexType>& vertices = mesh->GetVertices();

            FOR(size / 4)
            {
                float y = vertices[i].pos.y;

                uint8_t height = y / MAX_HEIGHT * 255;

                pixels[i * 4 + 0] = height;
                pixels[i * 4 + 1] = height;
                pixels[i * 4 + 2] = height;
                pixels[i * 4 + 3] = 255;
            }

            Image image;
            image.width = width;
            image.height = height;
            image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
            image.rowPitch = width * 4;
            image.slicePitch = size;
            image.pixels = pixels;

            SaveToWICFile(image, WIC_FLAGS_FORCE_RGB,
                GetWICCodec(WIC_CODEC_PNG), ToWString(file).c_str());

            delete[] pixels;
        }

        DIALOG->Close();
    }
}

void TerrainEditor::LoadHeightMap()
{
    if (ImGui::Button("LoadHeight"))
        DIALOG->OpenDialog("LoadHeight", "LoadHeight", ".png", ".");

    if (DIALOG->Display("LoadHeight"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            heightMap = Texture::Add(ToWString(file));

            Resize();
        }

        DIALOG->Close();
    }
}

void TerrainEditor::SaveAlphaMap()
{
    if (ImGui::Button("SaveAlpha"))
        DIALOG->OpenDialog("SaveAlpha", "SaveAlpha", ".png", ".");

    if (DIALOG->Display("SaveAlpha"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            UINT size = width * height * 4;
            uint8_t* pixels = new uint8_t[size];

            vector<VertexType>& vertices = mesh->GetVertices();

            FOR(size / 4)
            {
                pixels[i * 4 + 0] = vertices[i].alpha[0] * 255;
                pixels[i * 4 + 1] = vertices[i].alpha[1] * 255;
                pixels[i * 4 + 2] = vertices[i].alpha[2] * 255;
                pixels[i * 4 + 3] = 255;
            }

            Image image;
            image.width = width;
            image.height = height;
            image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
            image.rowPitch = width * 4;
            image.slicePitch = size;
            image.pixels = pixels;

            SaveToWICFile(image, WIC_FLAGS_FORCE_RGB,
                GetWICCodec(WIC_CODEC_PNG), ToWString(file).c_str());

            delete[] pixels;
        }

        DIALOG->Close();
    }
}

void TerrainEditor::LoadAlphaMap()
{
    if (ImGui::Button("LoadAlpha"))
        DIALOG->OpenDialog("LoadAlpha", "LoadAlpha", ".png", ".");

    if (DIALOG->Display("LoadAlpha"))
    {
        if (DIALOG->IsOk())
        {
            string file = DIALOG->GetFilePathName();

            file = file.substr(projectPath.size() + 1, file.size());

            Texture* alphaMap = Texture::Add(ToWString(file));

            vector<Float4> pixels;
            alphaMap->ReadPixels(pixels);

            vector<VertexType>& vertices = mesh->GetVertices();

            FOR(vertices.size())
            {
                vertices[i].alpha[0] = pixels[i].z;
                vertices[i].alpha[1] = pixels[i].y;
                vertices[i].alpha[2] = pixels[i].x;
                vertices[i].alpha[3] = pixels[i].w;
            }

            mesh->UpdateVertex();
        }

        DIALOG->Close();
    }
}

void TerrainEditor::UpdateHeight()
{
    vector<VertexType>& vertices = mesh->GetVertices();
    for (VertexType& vertex : vertices)
        vertex.normal = {};    

    MakeNormal();
    mesh->UpdateVertex();
}
