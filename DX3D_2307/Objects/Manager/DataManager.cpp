#include "Framework.h"

DataManager::DataManager()
{
    LoadItemData();
    LoadRoundData();
}

DataManager::~DataManager()
{
}

void DataManager::LoadItemData()
{
    ifstream loadFile("TextData/Table/ItemTable.csv");

    string temp;

    bool isFirstLine = true;

    while (true)
    {
        loadFile >> temp;

        if (isFirstLine)
        {
            isFirstLine = false;
            continue;
        }

        vector<string> datas = SplitString(temp, ",");

        ItemData data;
        data.key = stoi(datas[0]);
        data.name = datas[1];
        data.price = stoi(datas[2]);
        data.power = stoi(datas[3]);
        data.health = stoi(datas[4]);
        data.explain = datas[5];
        data.resell = stoi(datas[6]);

        itemDatas[data.key] = data;

        if (loadFile.eof())
            return;
    }
}

void DataManager::LoadRoundData()
{
    ifstream loadFile("TextData/Table/RoundTable.csv");

    string temp;

    bool isFirstLine = true;

    while (true)
    {
        loadFile >> temp;

        if (isFirstLine)
        {
            isFirstLine = false;
            continue;
        }

        vector<string> datas = SplitString(temp, ",");

        RoundData data;
        data.roundNum = stoi(datas[0]);
        data.monsterModelName = datas[1];
        data.monsterNum = stoi(datas[2]);
        data.monsterHp = stoi(datas[3]);

        roundDatas[data.roundNum] = data;

        if (loadFile.eof())
            return;
    }
}
