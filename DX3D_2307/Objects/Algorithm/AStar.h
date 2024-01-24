#pragma once

class AStar
{
public:
    AStar(UINT width = 20, UINT height = 20);
    ~AStar();

    void Update();
    void Render();

    void SetNode(Terrain* terrain);
    void SetObstacle();

    int FindCloseNode(Vector3 pos);
    Vector3 FindCloseNodePos(Vector3 pos);

    void GetPath(IN int start, IN int end, OUT vector<Vector3>& path);

    bool IsCollisionObstacle(Vector3 start, Vector3 end);

    void AddObstacle(Collider* collider) { obstacles.push_back(collider); }

private:
    void Reset();

    float GetDiagonalManhattanDistance(int start, int end);

    void Extend(int center, int end);
    int GetMinNode();

    void SetEdge();

private:
    UINT width, height;
    Vector3 interval;

    vector<Node*> nodes;
    //vector<int> openNodes;
    Heap* heap;

    vector<Collider*> obstacles;
};