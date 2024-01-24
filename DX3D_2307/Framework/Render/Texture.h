#pragma once

class Texture
{
private:
    Texture(ID3D11ShaderResourceView* srv, ScratchImage& image, wstring file);
    ~Texture();

public:
    static Texture* Add(wstring file);
    static Texture* Add(wstring file, wstring key);
    static Texture* Add(wstring key, ID3D11ShaderResourceView* srv);

public:
    void SetPS(UINT slot = 0);
    void SetDS(UINT slot = 0);

    void ReadPixels(vector<Float4>& pixels);

    Float2 GetSize()
    {
        return Float2(image.GetMetadata().width, image.GetMetadata().height);
    }

    wstring GetFile() { return file; }
    ID3D11ShaderResourceView* GetSRV() { return srv; }
private:
    wstring file;

    ScratchImage image;
    ID3D11ShaderResourceView* srv;

    bool isReferenced = false;

    static unordered_map<wstring, Texture*> textures;
};