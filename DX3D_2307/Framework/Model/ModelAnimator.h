#pragma once

class ModelAnimator : public Model
{
protected:
    struct Frame
    {
        int clip = 0;
        UINT curFrame = 0;
        float time = 0.0f;
        float scale = 1.0f;
    };

public:
   

    struct Motion
    {
        float takeTime = 0.2f;
        float tweenTime = 0.0f;
        float runningTime = 0.0f;
        float duration;

        Frame cur, next;

        Motion()
        {
            next.clip = -1;
        }
    };

    class FrameBuffer : public ConstBuffer
    {
    public:
        FrameBuffer() : ConstBuffer(&motion, sizeof(Motion))
        {
        };

        Motion* GetData() { return &motion; }

    private:
        Motion motion;
    };

public:
    ModelAnimator(string name);
    ~ModelAnimator();

    void Update();
    void Render();
    void GUIRender();

    void ReadClip(string clipName, UINT clipNum = 0);
    void PlayClip(int clip, float scale = 1.0f, float takeTime = 0.2f);
    void Stop() { isPlay = false; }

    Matrix GetTransformByNode(int nodeIndex);

    ModelClip* GetClip(int index) { return clips[index]; }

protected:
    void CreateTexture();
    void CreateClipTransform(UINT index);
    void UpdateFrame();

protected:
    bool isPlay = true;

    vector<ModelClip*> clips;

    ClipTransform* clipTransforms;
    ClipTransform* nodeTransforms;

    ID3D11Texture2D* texture = nullptr;
    ID3D11ShaderResourceView* srv;

    FrameBuffer* frameBuffer;
};