#pragma once

class Shader
{
protected:
    Shader() = default;
    virtual ~Shader();

public:
    static class VertexShader* AddVS(wstring file);
    static class PixelShader* AddPS(wstring file);
    static class GeometryShader* AddGS(wstring file);

    static void Delete();

    virtual void Set() = 0;

protected:
    ID3DBlob* blob;

private:
    static unordered_map<wstring, Shader*> shaders;
};