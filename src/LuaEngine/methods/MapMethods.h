/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef MAPMETHODS_H
#define MAPMETHODS_H

#include "ElunaInstanceAI.h"

/***
 * A game map, e.g. Azeroth, Eastern Kingdoms, the Molten Core, etc.
 *
 * Inherits all methods from: none
 */
namespace LuaMap
{

    /**
     * Returns `true` if the [Map] is an arena [BattleGround], `false` otherwise.
     *
     * @return bool isArena
     */
    int IsArena(Eluna* E, Map* map)
    {
        E->Push(map->IsBattleArena());
        return 1;
    }

    /**
     * Returns `true` if the [Map] is a non-arena [BattleGround], `false` otherwise.
     *
     * @return bool isBattleGround
     */
    int IsBattleground(Eluna* E, Map* map)
    {
        E->Push(map->IsBattleground());
        return 1;
    }

    /**
     * Returns `true` if the [Map] is a dungeon, `false` otherwise.
     *
     * @return bool isDungeon
     */
    int IsDungeon(Eluna* E, Map* map)
    {
        E->Push(map->IsDungeon());
        return 1;
    }

    /**
     * Returns `true` if the [Map] has no [Player]s, `false` otherwise.
     *
     * @return bool IsEmpty
     */
    int IsEmpty(Eluna* E, Map* map)
    {
        E->Push(map->IsEmpty());
        return 1;
    }

    /**
     * Returns `true` if the [Map] is a heroic, `false` otherwise.
     *
     * @return bool isHeroic
     */
    int IsHeroic(Eluna* E, Map* map)
    {
        E->Push(map->IsHeroic());
        return 1;
    }

    /**
     * Returns `true` if the [Map] is a raid, `false` otherwise.
     *
     * @return bool isRaid
     */
    int IsRaid(Eluna* E, Map* map)
    {
        E->Push(map->IsRaid());
        return 1;
    }

    /**
     * Returns the name of the [Map].
     *
     * @return string mapName
     */
    int GetName(Eluna* E, Map* map)
    {
        E->Push(map->GetMapName());
        return 1;
    }

    /**
     * Returns the height of the [Map] at the given X and Y coordinates.
     *
     * In case of no height found nil is returned
     *
     * @param float x
     * @param float y
     * @return float z
     */
    int GetHeight(Eluna* E, Map* map)
    {
        float x = E->CHECKVAL<float>(2);
        float y = E->CHECKVAL<float>(3);
        uint32 phasemask = E->CHECKVAL<uint32>(4, 1);
        float z = map->GetHeight(phasemask, x, y, MAX_HEIGHT);
        if (z != INVALID_HEIGHT)
            E->Push(z);
        return 1;
    }

    /**
     * Returns the difficulty of the [Map].
     *
     * Always returns 0 if the expansion is pre-TBC.
     *
     * @return int32 difficulty
     */
    int GetDifficulty(Eluna* E, Map* map)
    {
        E->Push(map->GetDifficulty());
        return 1;
    }

    /**
     * Returns the instance ID of the [Map].
     *
     * @return uint32 instanceId
     */
    int GetInstanceId(Eluna* E, Map* map)
    {
        E->Push(map->GetInstanceId());
        return 1;
    }

    /**
     * Returns the player count currently on the [Map] (excluding GMs).
     *
     * @return uint32 playerCount
     */
    int GetPlayerCount(Eluna* E, Map* map)
    {
        E->Push(map->GetPlayersCountExceptGMs());
        return 1;
    }

    /**
     * Returns the ID of the [Map].
     *
     * @return uint32 mapId
     */
    int GetMapId(Eluna* E, Map* map)
    {
        E->Push(map->GetId());
        return 1;
    }

    /**
     * Returns the area ID of the [Map] at the specified X, Y, and Z coordinates.
     *
     * @param float x
     * @param float y
     * @param float z
     * @param uint32 phasemask = PHASEMASK_NORMAL
     * @return uint32 areaId
     */
    int GetAreaId(Eluna* E, Map* map)
    {
        float x = E->CHECKVAL<float>(2);
        float y = E->CHECKVAL<float>(3);
        float z = E->CHECKVAL<float>(4);
        float phasemask = E->CHECKVAL<uint32>(5, PHASEMASK_NORMAL);

        E->Push(map->GetAreaId(phasemask, x, y, z));
        return 1;
    }

    /**
     * Returns a [WorldObject] by its GUID from the map if it is spawned.
     *
     * @param ObjectGuid guid
     * @return [WorldObject] object
     */
    int GetWorldObject(Eluna* E, Map* map)
    {
        ObjectGuid guid = E->CHECKVAL<ObjectGuid>(2);

        switch (guid.GetHigh())
        {
            case HIGHGUID_PLAYER:
                E->Push(eObjectAccessor()GetPlayer(map, guid));
                break;
            case HIGHGUID_TRANSPORT:
            case HIGHGUID_MO_TRANSPORT:
            case HIGHGUID_GAMEOBJECT:
                E->Push(map->GetGameObject(guid));
                break;
            case HIGHGUID_VEHICLE:
            case HIGHGUID_UNIT:
                E->Push(map->GetCreature(guid));
                break;
            case HIGHGUID_PET:
                E->Push(map->GetPet(guid));
                break;
            case HIGHGUID_DYNAMICOBJECT:
                E->Push(map->GetDynamicObject(guid));
                break;
            case HIGHGUID_CORPSE:
                E->Push(map->GetCorpse(guid));
                break;
            default:
                break;
        }
        return 1;
    }

    /**
     * Sets the [Weather] type based on [WeatherType] and grade supplied.
     *
     *     enum WeatherType
     *     {
     *         WEATHER_TYPE_FINE       = 0,
     *         WEATHER_TYPE_RAIN       = 1,
     *         WEATHER_TYPE_SNOW       = 2,
     *         WEATHER_TYPE_STORM      = 3,
     *         WEATHER_TYPE_THUNDERS   = 86,
     *         WEATHER_TYPE_BLACKRAIN  = 90
     *     };
     *
     * @param uint32 zone : id of the zone to set the weather for
     * @param [WeatherType] type : the [WeatherType], see above available weather types
     * @param float grade : the intensity/grade of the [Weather], ranges from 0 to 1
     */
    int SetWeather(Eluna* E, Map* map)
    {
        uint32 zoneId = E->CHECKVAL<uint32>(2);
        uint32 weatherType = E->CHECKVAL<uint32>(3);
        float grade = E->CHECKVAL<float>(4);

        Weather* weather = WeatherMgr::FindWeather(zoneId);
        if (!weather)
            weather = WeatherMgr::AddWeather(zoneId);
        if (weather)
            weather->SetWeather((WeatherType)weatherType, grade);
        return 0;
    }

    /**
     * Gets the instance data table for the [Map], if it exists.
     *
     * The instance must be scripted using Eluna for this to succeed.
     * If the instance is scripted in C++ this will return `nil`.
     *
     * @return table instance_data : instance data table, or `nil`
     */
    int GetInstanceData(Eluna* E, Map* map)
    {
        ElunaInstanceAI* iAI = NULL;
        if (InstanceMap* inst = map->ToInstanceMap())
            iAI = dynamic_cast<ElunaInstanceAI*>(inst->GetInstanceScript());

        if (iAI)
            E->PushInstanceData(iAI, false);
        else
            E->Push(); // nil

        return 1;
    }

    /**
     * Saves the [Map]'s instance data to the database.
     */
    int SaveInstanceData(Eluna* /*E*/, Map* map)
    {
        ElunaInstanceAI* iAI = NULL;
        if (InstanceMap* inst = map->ToInstanceMap())
            iAI = dynamic_cast<ElunaInstanceAI*>(inst->GetInstanceScript());

        if (iAI)
            iAI->SaveToDB();

        return 0;
    }

    /**
    * Returns a table with all the current [Player]s in the map
    *
    *     enum TeamId
    *     {
    *         TEAM_ALLIANCE = 0,
    *         TEAM_HORDE = 1,
    *         TEAM_NEUTRAL = 2
    *     };
    *
    * @param [TeamId] team : optional check team of the [Player], Alliance, Horde or Neutral (All)
    * @return table mapPlayers
    */
    int GetPlayers(Eluna* E, Map* map)
    {
        uint32 team = E->CHECKVAL<uint32>(2, TEAM_NEUTRAL);

        lua_newtable(E->L);
        int tbl = lua_gettop(E->L);
        uint32 i = 0;

        Map::PlayerList const& players = map->GetPlayers();
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            Player* player = itr->GetSource();
            if (!player)
                continue;
            if (player->GetSession() && (team >= TEAM_NEUTRAL || player->GetTeamId() == team))
            {
                E->Push(player);
                lua_rawseti(E->L, tbl, ++i);
            }
        }

        lua_settop(E->L, tbl);
        return 1;
    }

    /**
     * Returns a table with all the current [Creature]s in the map
     * 
     * @return table mapCreatures
     */
    int GetCreatures(Eluna* E, Map* map)
    {
        const auto& creatures = map->GetCreatureBySpawnIdStore();

        lua_createtable(E->L, creatures.size(), 0);
        int tbl = lua_gettop(E->L);

        for (const auto& pair : creatures)
        {
            Creature* creature = pair.second;

            E->Push(creature);
            lua_rawseti(E->L, tbl, creature->GetSpawnId());
        }

        lua_settop(E->L, tbl);
        return 1;
    }

    /**
     * Returns a table with all the current [Creature]s in the specific area id
     * 
     * @param number areaId : specific area id
     * @return table mapCreatures
     */
    int GetCreaturesByAreaId(Eluna* E, Map* map)
    {
        uint32 areaId = E->CHECKVAL<uint32>(2, -1);
        std::vector<Creature*> filteredCreatures;

        for (const auto& pair : map->GetCreatureBySpawnIdStore())
        {
            Creature* creature = pair.second;
            if (areaId == -1 || creature->GetAreaId() == areaId)
            {
                filteredCreatures.push_back(creature);
            }
        }

        lua_createtable(E->L, filteredCreatures.size(), 0);
        int tbl = lua_gettop(E->L);

        for (Creature* creature : filteredCreatures)
        {
            E->Push(creature);
            lua_rawseti(E->L, tbl, creature->GetSpawnId());
        }

        lua_settop(E->L, tbl);
        return 1;
    }

    ElunaRegister<Map> MapMethods[] =
    {
        // Getters
        { "GetName", &LuaMap::GetName },
        { "GetDifficulty", &LuaMap::GetDifficulty },
        { "GetInstanceId", &LuaMap::GetInstanceId },
        { "GetInstanceData", &LuaMap::GetInstanceData },
        { "GetPlayerCount", &LuaMap::GetPlayerCount },
        { "GetPlayers", &LuaMap::GetPlayers },
        { "GetMapId", &LuaMap::GetMapId },
        { "GetAreaId", &LuaMap::GetAreaId },
        { "GetHeight", &LuaMap::GetHeight },
        { "GetWorldObject", &LuaMap::GetWorldObject },
        { "GetCreatures", &LuaMap::GetCreatures },
        { "GetCreaturesByAreaId", &LuaMap::GetCreaturesByAreaId },


        // Setters
        { "SetWeather", &LuaMap::SetWeather },

        // Boolean
        { "IsArena", &LuaMap::IsArena },
        { "IsBattleground", &LuaMap::IsBattleground },
        { "IsDungeon", &LuaMap::IsDungeon },
        { "IsEmpty", &LuaMap::IsEmpty },
        { "IsHeroic", &LuaMap::IsHeroic },
        { "IsRaid", &LuaMap::IsRaid },

        // Other
        { "SaveInstanceData", &LuaMap::SaveInstanceData },
    };
};
#endif

