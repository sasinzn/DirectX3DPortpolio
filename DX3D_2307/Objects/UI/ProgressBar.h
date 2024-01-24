#pragma once

class ProgressBar : public Quad
{
public:
    ProgressBar(wstring frontImageFile, wstring backImageFile);
    ~ProgressBar();

    void Render();

    void SetPercent(float per) { percent = per; progressBuffer->UpdatePercent(percent); UpdateWorld(); }
    void SetPos(Vector3 pos) { this->pos = pos; }

private:
    ProgressBuffer* progressBuffer;
    Texture* backImage;
    Vector3 pos;
    float percent = 1.0f;
};