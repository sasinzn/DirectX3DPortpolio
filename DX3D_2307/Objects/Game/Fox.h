#pragma once

class Fox : public Transform
{
private:
    enum State
    {
        IDLE, RUN
    };

public:
    Fox();
    ~Fox();

    void Update();
    void Render();    
    
    void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
    void SetAStar(AStar* aStar) { this->aStar = aStar; }

private:
    void Control();
    void Move();
    void Rotate();
        
    void SetState(State state);    

private:
    ModelAnimator* model;
    Terrain* terrain;
    AStar* aStar;

    State curState = IDLE;

    float moveSpeed = 10.0f;
    float rotSpeed = 5.0f;    

    Vector3 velocity;
    Vector3 destPos;
    vector<Vector3> path;
};