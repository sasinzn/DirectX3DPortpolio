#pragma once

class MineCraftUI
{
public:
    MineCraftUI();
    ~MineCraftUI();
    
    void Update();
    void Render();

    void Mining();
    void Bulid();

private:
    Quad* crossHair;

    vector<Box*> boxes;
};