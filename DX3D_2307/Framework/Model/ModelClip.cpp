#include "Framework.h"

ModelClip::~ModelClip()
{
    //for (auto keyFrame : keyFrames)
    for (pair<string, KeyFrame*> keyFrame : keyFrames)
        delete keyFrame.second;
}

void ModelClip::Init()
{
    duration = frameCount / tickPerSecond;
    eventIter = events.begin();
    playTime = 0.0f;
}

void ModelClip::Excute()
{
    if (events.empty()) return;
    if (eventIter == events.end()) return;

    float ratio = playTime / duration;

    if (eventIter->first > ratio) return;

    eventIter->second();
    eventIter++;
}

KeyFrame* ModelClip::GetKeyFrame(string boneName)
{
    if (keyFrames.count(boneName) == 0)
        return nullptr;

    return keyFrames[boneName];
}

void ModelClip::SetEvent(Event event, float timeRatio)
{
    if (events.count(timeRatio) < 0) return;

    events[timeRatio] = event;
}
