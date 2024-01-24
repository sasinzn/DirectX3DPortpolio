#include "Framework.h"

MineCraftUI::MineCraftUI()
{
    crossHair = new Quad(Float2(100, 100));
    crossHair->SetLocalPosition({ CENTER_X, CENTER_Y, 0.0f });
    crossHair->UpdateWorld();
    crossHair->GetMaterial()->SetDiffuseMap(L"Textures/UI/cursor.png");
}

MineCraftUI::~MineCraftUI()
{
    delete crossHair;
}

void MineCraftUI::Update()
{
}

void MineCraftUI::Render()
{
    crossHair->Render();
}

void MineCraftUI::Mining()
{
    Box* box = BoxManager::Get()->GetCollision();

    if (box == nullptr) return;

    boxes.push_back(box);
}

void MineCraftUI::Bulid()
{
    if (boxes.size() == 0) return;

    Box* box = boxes.back();
    boxes.pop_back();

    BoxManager::Get()->AddBox(box);
}
