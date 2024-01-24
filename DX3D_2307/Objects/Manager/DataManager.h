#pragma once

struct ItemData
{
    int key;
    string name;
    int price;
    int power;
    int health;
    string explain;
    int resell;
};

struct RoundData
{
    int roundNum;
    string monsterModelName;
    int monsterNum;
    int monsterHp;
};

class DataManager : public Singleton<DataManager>
{
private:
    friend class Singleton;

    DataManager();
    ~DataManager();

    void LoadItemData();
    void LoadRoundData();
public:
    map<int, ItemData> GetItemDatas() { return itemDatas; }
    map<int, RoundData> GetRoundDatas() { return roundDatas; }

private:
    map<int, ItemData> itemDatas;
    map<int, RoundData> roundDatas;
};