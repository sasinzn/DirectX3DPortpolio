#include "Framework.h"

Button::Button(wstring textureFile)
    : Quad(textureFile)
{
}

void Button::Update()
{
    if (!isActive) return;

    if (IsContainPos(mousePos))
    {
        if (KEY->Down(VK_LBUTTON))
            isDownCheck = true;

        if (KEY->Press(VK_LBUTTON))
            state = DOWN;
        else
            state = OVER;

        if (isDownCheck && KEY->Up(VK_LBUTTON))
        {
            if (event != nullptr)
                event();

            if (paramEvent != nullptr)
                paramEvent(object);

            isDownCheck = false;
        }
    }
    else
    {
        state = NONE;

        if (KEY->Up(VK_LBUTTON))
            isDownCheck = false;
    }

    switch (state)
    {
    case Button::NONE:
        material->GetData()->diffuse = NONE_COLOR;
        break;
    case Button::DOWN:
        material->GetData()->diffuse = DOWN_COLOR;
        break;
    case Button::OVER:
        material->GetData()->diffuse = OVER_COLOR;
        break;    
    }

    UpdateWorld();
}

bool Button::IsContainPos(Vector3 screenPos)
{
    float left = GetGlobalPosition().x - GetSize().x * 0.5f;
    float right = GetGlobalPosition().x + GetSize().x * 0.5f;
    float bottom = GetGlobalPosition().y - GetSize().y * 0.5f;
    float top = GetGlobalPosition().y + GetSize().y * 0.5f;

    if (screenPos.x > left && screenPos.x < right)
    {
        if (screenPos.y > bottom && screenPos.y < top)
            return true;
    }

    return false;
}
