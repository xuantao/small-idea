#include <stdio.h>
#include "CfgTabParser.h"
#include "Cfg.h"
#include "CfgUtility.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <map>
#include <sstream>

using namespace cfg;

struct Keyfn
{
    int Key(Struct2& test) { return test.a; }
};

class Region
{
public:
    void Activate()
    {
        // scene object activate
    }
};

class Scene
{
public:
    void Activate()
    {
        int regionCount = m_nRegionWidth * m_nRegionHeight;
        for (int i = 0; i < regionCount; ++i)
        {
            m_pRegions[i].Activate();
        }

        // other scene system activate
    }

    int m_nRegionWidth;
    int m_nRegionHeight;
    Region* m_pRegions;
};

class GameWorld
{
public:
    static const int LOGIC_FPS = 20;
    static const int FRAME_LENGTH = 50;

public:
    void Run()
    {
        while (true)
        {
            int64_t now = TimeNow();
            while (m_nGameTime < now)
            {
                m_nGameTime += FRAME_LENGTH;
                ++m_nGameLoop;

                Activate();
            }
        }
    }

protected:
    void Activate()
    {
        for (auto pair : m_mapScenes)
        {
            pair.second->Activate();
        }

        // other system activate
    }

    int64_t TimeNow() { return 0; }

protected:
    int m_nGameLoop;
    int64_t m_nGameTime;
    std::map<int, Scene*> m_mapScenes;
};

int main(int argc, char* argv[])
{
    GameWorld gameWorld;
    gameWorld.Run();

    return 1;
}
