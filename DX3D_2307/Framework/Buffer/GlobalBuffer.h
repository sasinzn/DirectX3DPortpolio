#pragma once
//GlobalBuffer
class MatrixBuffer : public ConstBuffer
{
public:
    MatrixBuffer() : ConstBuffer(&matrix, sizeof(Matrix))
    {
        matrix = XMMatrixIdentity();
    }

    void Set(Matrix value)
    {
        matrix = XMMatrixTranspose(value);
    }

private:
    Matrix matrix;
};

class WorldBuffer : public ConstBuffer
{
private:
    struct Data
    {
        Matrix world = XMMatrixIdentity();

        int type = 0;
        float padding[3];
    };

public:
    WorldBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    }

    void Set(Matrix value)
    {
        data.world = XMMatrixTranspose(value);
    }

    void SetType(int type)
    {
        data.type = type;
    }

private:
    Data data;
};

class ViewBuffer : public ConstBuffer
{
private:
    struct Data
    {
        Matrix view;
        Matrix invView;
    };

public:
    ViewBuffer() : ConstBuffer(&data, sizeof(Data))
    {
        data.view = XMMatrixIdentity();
        data.invView = XMMatrixIdentity();
    }

    void Set(Matrix view, Matrix invView)
    {
        data.view = XMMatrixTranspose(view);
        data.invView = XMMatrixTranspose(invView);
    }

private:
    Data data;
};

class LightBuffer : public ConstBuffer
{
public:
    struct Light
    {
        Float4 color = { 1, 1, 1, 1 };

        Float3 direction = { 0, -1, 1 };
        int type = 0;

        Float3 position = {};
        float range = 100.0f;

        float inner = 55.0f;
        float outer = 70.0f;
        float length = 50.0f;
        int active = 1;
    };

private:
    struct Data
    {
        Light lights[MAX_LIGHT];

        int lightCount = 1;
        Float3 ambientLight = { 0.1f, 0.1f, 0.1f };        
        Float3 ambientCeil = { 0.1f, 0.1f, 0.1f };
        float padding;
    };

public:
    LightBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    };

    Data* GetData() { return &data; }

private:
    Data data;
};

class MaterialBuffer : public ConstBuffer
{
public:
    struct Data
    {
        Float4 diffuse = { 1, 1, 1, 1 };
        Float4 specular = { 1, 1, 1, 1 };
        Float4 ambient = { 0.1f, 0.1f, 0.1f, 1 };
        Float4 emissive = { 0, 0, 0, 0 };

        float shininess = 24.0f;                
        int hasNormalMap = 0;
        float padding[2];
    };

public:
    MaterialBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    };

    Data* GetData() { return &data; }

private:
    Data data;
};

class BrushBuffer : public ConstBuffer
{
public:
    struct Data
    {
        int type = 0;
        Float3 pickingPos;

        float range = 10.0f;
        Float3 color = { 0, 1, 0 };
    };

public:
    BrushBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    };

    Data* GetData() { return &data; }

private:
    Data data;
};

class ProgressBuffer : public ConstBuffer
{
public:
    struct Data
    {
        float percent;
        UINT type;
        float padding[2];
    };
public:
    ProgressBuffer(UINT type) : ConstBuffer(&data, sizeof(data))
    {
        data.percent = 1.0f;
        data.type = type;
    }

    void UpdatePercent(float percent)
    {
        data.percent = percent;
    }

private:
    Data data;
};

class FloatValueBuffer : public ConstBuffer
{
public:
    FloatValueBuffer() : ConstBuffer(values, sizeof(float) * 4)
    {
    };

    float* GetData() { return values; }

private:
    float values[4] = {};
};

class IntValueBuffer : public ConstBuffer
{
public:
    IntValueBuffer() : ConstBuffer(values, sizeof(float) * 4)
    {
    };

    int* GetData() { return values; }

private:
    int values[4] = {};
};

class WeatherBuffer : public ConstBuffer
{
public:
    struct Data
    {
        Float3 velocity = { 0, -1, 0 };
        float distance = 100.0f;

        Float4 color = { 1, 1, 1, 1 };

        Float3 origin = {};
        float time = 0.0f;

        Float3 size = { 50, 50, 50 };
        float turbulence = 0.1f;
    };

public:
    WeatherBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    };

    Data* GetData() { return &data; }

private:
    Data data;
};

class WaterBuffer : public ConstBuffer
{
public:
    struct Data
    {
        Float4 color = { 1, 1, 1, 1 };

        float waveTime = 0.0f;
        float waveSpeed = 0.1f;
        float waveScale = 0.1f;
        float waveShininess = 24.0f;

        float frensnel = 0.5f;
        float padding[3];
    };

public:
    WaterBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    };

    Data* GetData() { return &data; }

private:
    Data data;
};

class RayBuffer : public ConstBuffer
{
public:
    struct Data
    {
        Float3 pos;
        UINT triangleSize;

        Float3 dir;
        float padding;
    };

public:
    RayBuffer() : ConstBuffer(&data, sizeof(Data))
    {
    };

    Data* GetData() { return &data; }

private:
    Data data;
};