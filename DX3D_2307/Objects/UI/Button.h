#pragma once

class Button : public Quad
{
protected:
    enum State
    {
        NONE, DOWN, OVER
    };

    const Float4 NONE_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
    const Float4 DOWN_COLOR = { 0.5f, 0.5f, 0.5f, 1.0f };
    const Float4 OVER_COLOR = { 0.9f, 0.9f, 0.9f, 1.0f };

public:
    Button(wstring textureFile);
    ~Button() = default;

    void Update();    

    void SetEvent(Event event) { this->event = event; }
    void SetParamEvent(ParamEvent event) { this->paramEvent = event; }
    void SetObejct(void* object) { this->object = object; }

    bool IsContainPos(Vector3 screenPos);

protected:
    State state = NONE;

    bool isDownCheck = false;

    Event event = nullptr;
    ParamEvent paramEvent = nullptr;

    void* object;
};