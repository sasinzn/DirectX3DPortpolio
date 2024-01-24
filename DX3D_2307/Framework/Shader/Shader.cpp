#include "Framework.h"

unordered_map<wstring, Shader*> Shader::shaders;

Shader::~Shader()
{
    blob->Release();
}

VertexShader* Shader::AddVS(wstring file)
{
    wstring key = file + L"VS";

    if (shaders.count(key) > 0)
        return (VertexShader*)shaders[key];

    wstring path = L"Shaders/" + file;

    shaders[key] = new VertexShader(path);

    return (VertexShader*)shaders[key];
}

PixelShader* Shader::AddPS(wstring file)
{
    wstring key = file + L"PS";

    if (shaders.count(key) > 0)
        return (PixelShader*)shaders[key];

    wstring path = L"Shaders/" + file;

    shaders[key] = new PixelShader(path);

    return (PixelShader*)shaders[key];
}

GeometryShader* Shader::AddGS(wstring file)
{
    wstring key = file + L"GS";

    if (shaders.count(key) > 0)
        return (GeometryShader*)shaders[key];

    wstring path = L"Shaders/" + file;

    shaders[key] = new GeometryShader(path);

    return (GeometryShader*)shaders[key];
}

void Shader::Delete()
{
    for (pair<wstring, Shader*> shader : shaders)
        delete shader.second;
}
