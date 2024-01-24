#pragma once

class SceneManager : public Singleton<SceneManager>
{
private:
    friend class Singleton;

    SceneManager() = default;
    ~SceneManager();

public:
    void Update();

    void PreRender();
    void Render();
    void PostRender();
    void GUIRender();

    void Create(string key, Scene* scene);

    Scene* Add(string key);
    void Remove(string key);

    void ActiveScene(string key) { scenes[key]->SetActive(true); }
    void InactiveScene(string key) { scenes[key]->SetActive(false); }
    Scene* GetScene(string key) { return scenes[key]; }

private:
    void AddScene();
    void RemoveScene();

private:
    map<string, Scene*> scenes;

    list<Scene*> curScenes;

    vector<string> addScenes, removeScenes;
};