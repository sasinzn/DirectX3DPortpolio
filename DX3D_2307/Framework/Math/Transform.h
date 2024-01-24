#pragma once

class Transform
{
public:
    Transform();
    ~Transform() = default;

    void UpdateWorld();
    void GUIRender();

    Transform* GetParent();

    void Translate(Vector3 direction);
    void Rotate(Vector3 direction);

    bool Active();
    bool ActiveSelf() { return isActive; }

    Vector3 GetRight() const { return right.GetNormalized(); }
    Vector3 GetLeft() const { return right.GetNormalized() * -1; }
    Vector3 GetUp() const { return up.GetNormalized(); }
    Vector3 GetDown() const { return up.GetNormalized() * -1; }
    Vector3 GetForward() const { return forward.GetNormalized(); }
    Vector3 GetBack() const { return forward.GetNormalized() * -1; }

    Vector3 GetLocalPosition() const { return localPosition; }
    Vector3 GetLocalRotation() const { return localRotation; }
    Vector3 GetLocalScale() const { return localScale; }

    Vector3 GetGlobalPosition() const { return globalPosition; }
    Vector3 GetGlobalRotation() const { return globalRotation; }
    Vector3 GetGlobalScale() const { return globalScale; }
    
    Matrix GetWorld() { return world; }

    void SetLocalPosition(Vector3 position) { localPosition = position; }
    void SetLocalRotation(Vector3 rotation) { localRotation = rotation; }
    void SetLocalScale(Vector3 scale) { localScale = scale; }

    void SetLocalPosition(float x, float y, float z) { localPosition = Vector3(x, y, z); }
    void SetLocalRotation(float x, float y, float z) { localRotation = Vector3(x, y, z); }
    void SetLocalScale(float x, float y, float z) { localScale = Vector3(x, y, z); }

    void SetParent(Transform* transform) { parent = transform; }
    void SetPivot(Vector3 pivot) { this->pivot = pivot; }
    void SetWorld(Matrix matrix) { world = matrix; }

    string GetTag() { return tag; }
    void SetTag(string tag) { this->tag = tag; }
    void SetActive(bool isActive) { this->isActive = isActive; }

    void Save();
    void Load();
    void Load(string tag);

protected:
    string tag;
    string editTag;

    bool isActive = true;

    Vector3 localPosition;
    Vector3 localRotation;
    Vector3 localScale = { 1, 1, 1 };

    Matrix world;
private:
    Transform* parent = nullptr;

    Vector3 pivot;

    Vector3 right, up, forward;

    Vector3 globalPosition;
    Vector3 globalRotation;
    Vector3 globalScale;

    Float4x4 matWorld;
};