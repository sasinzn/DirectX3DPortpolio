#pragma once

class ConstBuffer
{
public:
    ConstBuffer(void* data, UINT dataSize);
    ~ConstBuffer();

    void SetVS(UINT slot);
    void SetPS(UINT slot);
    void SetHS(UINT slot);
    void SetDS(UINT slot);
    void SetCS(UINT slot);

private:
    ID3D11Buffer* buffer;

    void* data;
    UINT dataSize;
};