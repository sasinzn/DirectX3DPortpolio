#pragma once

class Device : public Singleton<Device>
{
private:
    friend class Singleton;

    Device();
    ~Device();

public:
    void Clear();
    void Present();

    ID3D11Device* GetDevice() { return device; }
    ID3D11DeviceContext* GetDeviceContext() { return deviceContext; } 
    IDXGISwapChain* GetSwapChain() { return swapChain; }

private:
    ID3D11Device* device;//CPU
    ID3D11DeviceContext* deviceContext;//GPU-출력

    IDXGISwapChain* swapChain;//백버퍼 관리
    ID3D11RenderTargetView* renderTargetView;//VRAM에 할당되는 백버퍼 관리
    ID3D11DepthStencilView* depthStencilView;//VRAM에 깊이 버퍼 관리
};