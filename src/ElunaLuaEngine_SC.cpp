/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Chat.h"
#include "ElunaEventMgr.h"
#include "ElunaConfig.h"
#include "ElunaLoader.h"
#include "Log.h"
#include "LuaEngine.h"
#include "Pet.h"
#include "Player.h"
#include "Vehicle.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"

class Eluna_AllCreatureScript : public AllCreatureScript
{
public:
    Eluna_AllCreatureScript() : AllCreatureScript("Eluna_AllCreatureScript") { }

    // Creature
    bool CanCreatureGossipHello(Player* player, Creature* creature) override
    {
        if (Eluna* e = creature->GetEluna())
            if (e->OnGossipHello(player, creature))
                return true;

        return false;
    }

    bool CanCreatureGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        if (Eluna* e = creature->GetEluna())
            if (e->OnGossipSelect(player, creature, sender, action))
                return true;

        return false;
    }

    bool CanCreatureGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, const char* code) override
    {
        if (Eluna* e = creature->GetEluna())
            if (e->OnGossipSelectCode(player, creature, sender, action, code))
                return true;

        return false;
    }

    void OnCreatureAddWorld(Creature* creature) override
    {
        if (Eluna* e = creature->GetEluna())
        {
            e->OnAddToWorld(creature);
            if (creature->IsGuardian() && creature->ToTempSummon() && creature->ToTempSummon()->GetSummonerGUID().IsPlayer())
                e->OnPetAddedToWorld(creature->ToTempSummon()->GetSummonerUnit()->ToPlayer(), creature);
        }
    }

    void OnCreatureRemoveWorld(Creature* creature) override
    {
        if (Eluna* e = creature->GetEluna())
            e->OnRemoveFromWorld(creature);
    }

    bool CanCreatureQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if(Eluna * e = creature->GetEluna())
            if (e->OnQuestAccept(player, creature, quest))
                return false;

        return true;
    }

    bool CanCreatureQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 opt) override
    {
        if (Eluna* e = creature->GetEluna())
        {
            if(e->OnQuestReward(player, creature, quest, opt))
            {
                ClearGossipMenuFor(player);
                return true;
            }
        }

        return false;
    }

    CreatureAI* GetCreatureAI(Creature* creature) const override
    {
        if(Eluna * e = creature->GetEluna())
            if (CreatureAI* luaAI = e->GetAI(creature))
                return luaAI;

        return nullptr;
    }
};

class Eluna_AllGameObjectScript : public AllGameObjectScript
{
public:
    Eluna_AllGameObjectScript() : AllGameObjectScript("Eluna_AllGameObjectScript") { }

    void OnGameObjectAddWorld(GameObject* go) override
    {
        if(Eluna * e = go->GetEluna())
            e->OnAddToWorld(go);
    }

    void OnGameObjectRemoveWorld(GameObject* go) override
    {
        if(Eluna * e = go->GetEluna())
            e->OnRemoveFromWorld(go);
    }

    void OnGameObjectUpdate(GameObject* go, uint32 diff) override
    {
        if(Eluna * e = go->GetEluna())
            e->UpdateAI(go, diff);
    }

    bool CanGameObjectGossipHello(Player* player, GameObject* go) override
    {
        if(Eluna * e = go->GetEluna())
        {
            if(e->OnGossipHello(player, go))
                return true;

            if (e->OnGameObjectUse(player, go))
                return true;
        }
        
        return false;
    }

    void OnGameObjectDamaged(GameObject* go, Player* player) override
    {
        if(Eluna * e = go->GetEluna())
            e->OnDamaged(go, player);
    }

    void OnGameObjectDestroyed(GameObject* go, Player* player) override
    {
        if(Eluna * e = go->GetEluna())
            e->OnDestroyed(go, player);
    }

    void OnGameObjectLootStateChanged(GameObject* go, uint32 state, Unit* /*unit*/) override
    {
        if(Eluna * e = go->GetEluna())
            e->OnGameObjectStateChanged(go, state);
    }

    void OnGameObjectStateChanged(GameObject* go, uint32 state) override
    {
        if(Eluna * e = go->GetEluna())
            e->OnGameObjectStateChanged(go, state);
    }

    bool CanGameObjectQuestAccept(Player* player, GameObject* go, Quest const* quest) override
    {
        if(Eluna * e = player->GetEluna())
            if (e->OnQuestAccept(player, go, quest))
                return false;

        return true;
    }

    bool CanGameObjectGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 action) override
    {
        if(Eluna * e = go->GetEluna())
            if(e->OnGossipSelect(player, go, sender, action))
                return true;

        return false;
    }

    bool CanGameObjectGossipSelectCode(Player* player, GameObject* go, uint32 sender, uint32 action, const char* code) override
    {
        if(Eluna * e = go->GetEluna())
            if(e->OnGossipSelectCode(player, go, sender, action, code))
                return true;

        return false;
    }

    bool CanGameObjectQuestReward(Player* player, GameObject* go, Quest const* quest, uint32 opt) override
    {
        if(Eluna * e = player->GetEluna())
        {
            if (e->OnQuestAccept(player, go, quest))
                return false;

            if (e->OnQuestReward(player, go, quest, opt))
                return false;
        }

        return true;
    }

    GameObjectAI* GetGameObjectAI(GameObject* go) const override
    {
        if(Eluna * e = go->GetEluna())
            e->OnSpawn(go);
        return nullptr;
    }
};

class Eluna_AllItemScript : public AllItemScript
{
public:
    Eluna_AllItemScript() : AllItemScript("Eluna_AllItemScript") { }

    bool CanItemQuestAccept(Player* player, Item* item, Quest const* quest) override
    {
        if (Eluna* e = player->GetEluna())
            if (e->OnQuestAccept(player, item, quest))
                return false;

        return true;
    }

    bool CanItemUse(Player* player, Item* item, SpellCastTargets const& targets) override
    {
        if (Eluna* e = player->GetEluna())
            if (!e->OnUse(player, item, targets))
                return true;

        return false;
    }

    bool CanItemExpire(Player* player, ItemTemplate const* proto) override
    {
        if (Eluna* e = player->GetEluna())
            if (e->OnExpire(player, proto))
                return false;

        return true;
    }

    bool CanItemRemove(Player* player, Item* item) override
    {
        if (Eluna* e = player->GetEluna())
            if (e->OnRemove(player, item))
                return false;

        return true;
    }

    void OnItemGossipSelect(Player* player, Item* item, uint32 sender, uint32 action) override
    {
        if (Eluna* e = player->GetEluna())
            e->HandleGossipSelectOption(player, item, sender, action, "");
    }

    void OnItemGossipSelectCode(Player* player, Item* item, uint32 sender, uint32 action, const char* code) override
    {
        if (Eluna* e = player->GetEluna())
            e->HandleGossipSelectOption(player, item, sender, action, code);
    }
};

class Eluna_AllMapScript : public AllMapScript
{
public:
    Eluna_AllMapScript() : AllMapScript("Eluna_AllMapScript") { }

    void OnBeforeCreateInstanceScript(InstanceMap* instanceMap, InstanceScript** instanceData, bool /*load*/, std::string /*data*/, uint32 /*completedEncounterMask*/) override
    {
        if (instanceData)
            if(Eluna * e = instanceMap->GetEluna())
                *instanceData = e->GetInstanceData(instanceMap);
    }

    void OnDestroyInstance(MapInstanced* /*mapInstanced*/, Map* map) override
    {
        if(Eluna * e = map->GetEluna())
            e->FreeInstanceId(map->GetInstanceId());
    }

    void OnCreateMap(Map* map) override
    {
        if (Eluna* e = map->GetEluna())
            e->OnMapCreate(map);
    }

    void OnDestroyMap(Map* map) override
    {
        if (Eluna* e = map->GetEluna())
            e->OnDestroy(map);
    }

    void OnPlayerEnterAll(Map* map, Player* player) override
    {
        if (Eluna* e = map->GetEluna())
            e->OnPlayerEnter(map, player);
    }

    void OnPlayerLeaveAll(Map* map, Player* player) override
    {
        if (Eluna* e = map->GetEluna())
            e->OnPlayerLeave(map, player);
    }

    void OnMapUpdate(Map* map, uint32 diff) override
    {
        if (Eluna* e = map->GetEluna())
        {
            if(!sElunaConfig->IsElunaCompatibilityMode())
                e->UpdateEluna(diff);

            e->OnUpdate(map, diff);
        }
    }
};

class Eluna_AuctionHouseScript : public AuctionHouseScript
{
public:
    Eluna_AuctionHouseScript() : AuctionHouseScript("Eluna_AuctionHouseScript") { }

    void OnAuctionAdd(AuctionHouseObject* ah, AuctionEntry* entry) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnAdd(ah, entry);
    }

    void OnAuctionRemove(AuctionHouseObject* ah, AuctionEntry* entry) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnRemove(ah, entry);
    }

    void OnAuctionSuccessful(AuctionHouseObject* ah, AuctionEntry* entry) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnSuccessful(ah, entry);
    }

    void OnAuctionExpire(AuctionHouseObject* ah, AuctionEntry* entry) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnExpire(ah, entry);
    }
};

class Eluna_BGScript : public BGScript
{
public:
    Eluna_BGScript() : BGScript("Eluna_BGScript") { }

    void OnBattlegroundStart(Battleground* bg) override
    {
        if(Map* m = bg->FindBgMap())
            if (Eluna* e = m->GetEluna())
                e->OnBGStart(bg, bg->GetBgTypeID(), bg->GetInstanceID());
    }

    void OnBattlegroundEnd(Battleground* bg, TeamId winnerTeam) override
    {
        if (Map* m = bg->FindBgMap())
            if (Eluna* e = m->GetEluna())
                e->OnBGEnd(bg, bg->GetBgTypeID(), bg->GetInstanceID(), winnerTeam);
    }

    void OnBattlegroundDestroy(Battleground* bg) override
    {
        if (Map* m = bg->FindBgMap())
            if (Eluna* e = m->GetEluna())
                e->OnBGDestroy(bg, bg->GetBgTypeID(), bg->GetInstanceID());
    }

    void OnBattlegroundCreate(Battleground* bg) override
    {
        if (Map* m = bg->FindBgMap())
            if (Eluna* e = m->GetEluna())
                e->OnBGCreate(bg, bg->GetBgTypeID(), bg->GetInstanceID());
    }
};

class Eluna_CommandSC : public CommandSC
{
public:
    Eluna_CommandSC() : CommandSC("Eluna_CommandSC") { }

    bool OnTryExecuteCommand(ChatHandler& handler, std::string_view cmdStr) override
    {
        if (Eluna* e = sWorld->GetEluna())
            if(!e->OnCommand(handler, std::string(cmdStr).c_str()))
                return false;

        return true;
    }
};

class Eluna_ElunaScript : public ElunaScript
{
public:
    Eluna_ElunaScript() : ElunaScript("Eluna_ElunaScript") { }

    // Weather
    void OnWeatherChange(Weather* weather, WeatherState state, float grade) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnChange(weather, weather->GetZone(), state, grade);
    }

    // AreaTriger
    bool CanAreaTrigger(Player* player, AreaTrigger const* trigger) override
    {
        if (Eluna* e = player->GetEluna())
            if (e->OnAreaTrigger(player, trigger))
                return true;

        return false;
    }
};

class Eluna_GameEventScript : public GameEventScript
{
public:
    Eluna_GameEventScript() : GameEventScript("Eluna_GameEventScript") { }

    void OnStart(uint16 eventID) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnGameEventStart(eventID);
    }

    void OnStop(uint16 eventID) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnGameEventStop(eventID);
    }
};

class Eluna_GroupScript : public GroupScript
{
public:
    Eluna_GroupScript() : GroupScript("Eluna_GroupScript") { }

    void OnAddMember(Group* group, ObjectGuid guid) override
    {
        if (Eluna* e = group->GetLeader()->GetEluna())
            e->OnAddMember(group, guid);
    }

    void OnInviteMember(Group* group, ObjectGuid guid) override
    {
        if (Eluna* e = group->GetLeader()->GetEluna())
            e->OnInviteMember(group, guid);
    }

    void OnRemoveMember(Group* group, ObjectGuid guid, RemoveMethod method, ObjectGuid /* kicker */, const char* /* reason */) override
    {
        if (Eluna* e = group->GetLeader()->GetEluna())
            e->OnRemoveMember(group, guid, method);
    }

    void OnChangeLeader(Group* group, ObjectGuid newLeaderGuid, ObjectGuid oldLeaderGuid) override
    {
        if (Eluna* e = group->GetLeader()->GetEluna())
            e->OnChangeLeader(group, newLeaderGuid, oldLeaderGuid);
    }

    void OnDisband(Group* group) override
    {
        if (Eluna* e = group->GetLeader()->GetEluna())
            e->OnDisband(group);
    }

    void OnCreate(Group* group, Player* leader) override
    {
        if (Eluna* e = leader->GetEluna())
            e->OnCreate(group, leader->GetGUID(), group->GetGroupType());
    }
};

class Eluna_GuildScript : public GuildScript
{
public:
    Eluna_GuildScript() : GuildScript("Eluna_GuildScript") { }

    void OnAddMember(Guild* guild, Player* player, uint8& plRank) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnAddMember(guild, player, plRank);
    }

    void OnRemoveMember(Guild* guild, Player* player, bool isDisbanding, bool /*isKicked*/) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnRemoveMember(guild, player, isDisbanding);
    }

    void OnMOTDChanged(Guild* guild, const std::string& newMotd) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnMOTDChanged(guild, newMotd);
    }

    void OnInfoChanged(Guild* guild, const std::string& newInfo) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnInfoChanged(guild, newInfo);
    }

    void OnCreate(Guild* guild, Player* leader, const std::string& name) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnCreate(guild, leader, name);
    }

    void OnDisband(Guild* guild) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnDisband(guild);
    }

    void OnMemberWitdrawMoney(Guild* guild, Player* player, uint32& amount, bool isRepair) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnMemberWitdrawMoney(guild, player, amount, isRepair);
    }

    void OnMemberDepositMoney(Guild* guild, Player* player, uint32& amount) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnMemberDepositMoney(guild, player, amount);
    }

    void OnItemMove(Guild* guild, Player* player, Item* pItem, bool isSrcBank, uint8 srcContainer, uint8 srcSlotId,
        bool isDestBank, uint8 destContainer, uint8 destSlotId) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnItemMove(guild, player, pItem, isSrcBank, srcContainer, srcSlotId, isDestBank, destContainer, destSlotId);
    }

    void OnEvent(Guild* guild, uint8 eventType, ObjectGuid::LowType playerGuid1, ObjectGuid::LowType playerGuid2, uint8 newRank) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnEvent(guild, eventType, playerGuid1, playerGuid2, newRank);
    }

    void OnBankEvent(Guild* guild, uint8 eventType, uint8 tabId, ObjectGuid::LowType playerGuid, uint32 itemOrMoney, uint16 itemStackCount, uint8 destTabId) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnBankEvent(guild, eventType, tabId, playerGuid, itemOrMoney, itemStackCount, destTabId);
    }
};

class Eluna_LootScript : public LootScript
{
public:
    Eluna_LootScript() : LootScript("Eluna_LootScript") { }

    void OnLootMoney(Player* player, uint32 gold) override
    {
        if (Eluna* e = player->GetEluna())
            e->OnLootMoney(player, gold);
    }
};

class Eluna_MiscScript : public MiscScript
{
public:
    Eluna_MiscScript() : MiscScript("Eluna_MiscScript") { }

    void GetDialogStatus(Player* player, Object* questgiver) override
    {
        if (Eluna* e = player->GetEluna())
        {
            if (questgiver->GetTypeId() == TYPEID_GAMEOBJECT)
                e->GetDialogStatus(player, questgiver->ToGameObject());
            else if (questgiver->GetTypeId() == TYPEID_UNIT)
                e->GetDialogStatus(player, questgiver->ToCreature());
        }

    }
};

class Eluna_PetScript : public PetScript
{
public:
    Eluna_PetScript() : PetScript("Eluna_PetScript") { }

    void OnPetAddToWorld(Pet* pet) override
    {
        if (Eluna* e = pet->GetOwner()->GetEluna())
            e->OnPetAddedToWorld(pet->GetOwner(), pet);
    }
};

class Eluna_PlayerScript : public PlayerScript
{
public:
    Eluna_PlayerScript() : PlayerScript("Eluna_PlayerScript") { }

    void OnPlayerResurrect(Player* player, float /*restore_percent*/, bool /*applySickness*/) override
    {
        if (Eluna* e = player->GetEluna())
            e->OnResurrect(player);
    }

    bool CanPlayerUseChat(Player* player, uint32 type, uint32 lang, std::string& msg) override
    {
        if (type != CHAT_MSG_SAY && type != CHAT_MSG_YELL && type != CHAT_MSG_EMOTE)
            return true;

        if (Eluna* e = player->GetEluna())
            if (!e->OnChat(player, type, lang, msg))
                return false;

        return true;
    }

    bool CanPlayerUseChat(Player* player, uint32 type, uint32 lang, std::string& msg, Player* target) override
    {
        if (Eluna* e = player->GetEluna())
            if (!e->OnChat(player, type, lang, msg, target))
                return false;

        return true;
    }

    bool CanPlayerUseChat(Player* player, uint32 type, uint32 lang, std::string& msg, Group* group) override
    {
        if (Eluna* e = player->GetEluna())           
            if (!e->OnChat(player, type, lang, msg, group))
                return false;

        return true;
    }

    bool CanPlayerUseChat(Player* player, uint32 type, uint32 lang, std::string& msg, Guild* guild) override
    {
        if (Eluna* e = player->GetEluna()) 
            if (!e->OnChat(player, type, lang, msg, guild))
                return false;

        return true;
    }

    bool CanPlayerUseChat(Player* player, uint32 type, uint32 lang, std::string& msg, Channel* channel) override
    {
        if (Eluna* e = player->GetEluna()) 
            if (!e->OnChat(player, type, lang, msg, channel))
                return false;

        return true;
    }

    void OnLootItem(Player* player, Item* item, uint32 count, ObjectGuid lootguid) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnLootItem(player, item, count, lootguid);
    }

    void OnPlayerLearnTalents(Player* player, uint32 talentId, uint32 talentRank, uint32 spellid) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnLearnTalents(player, talentId, talentRank, spellid);
    }

    bool CanUseItem(Player* player, ItemTemplate const* proto, InventoryResult& result) override
    {
        if (Eluna* e = player->GetEluna()) 
            result = e->OnCanUseItem(player, proto->ItemId);
        
        return result ? result != EQUIP_ERR_OK ? false : true : false;
    }

    void OnEquip(Player* player, Item* it, uint8 bag, uint8 slot, bool /*update*/) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnEquip(player, it, bag, slot);
    }

    void OnPlayerEnterCombat(Player* player, Unit* enemy) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnPlayerEnterCombat(player, enemy);
    }

    void OnPlayerLeaveCombat(Player* player) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnPlayerLeaveCombat(player);
    }

    bool CanRepopAtGraveyard(Player* player) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnRepop(player);
        return true;
    }

    void OnQuestAbandon(Player* player, uint32 questId) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnQuestAbandon(player, questId);
    }

    void OnMapChanged(Player* player) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnMapChanged(player);
    }

    void OnGossipSelect(Player* player, uint32 menu_id, uint32 sender, uint32 action) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->HandleGossipSelectOption(player, menu_id, sender, action, "");
    }

    void OnGossipSelectCode(Player* player, uint32 menu_id, uint32 sender, uint32 action, const char* code) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->HandleGossipSelectOption(player, menu_id, sender, action, code);
    }

    void OnPVPKill(Player* killer, Player* killed) override
    {
        if (Eluna* e = killer->GetEluna())
            e->OnPVPKill(killer, killed);
    }

    void OnCreatureKill(Player* killer, Creature* killed) override
    {
        if (Eluna* e = killer->GetEluna())
            e->OnCreatureKill(killer, killed);
    }

    void OnPlayerKilledByCreature(Creature* killer, Player* killed) override
    {
        if (Eluna* e = killed->GetEluna()) 
            e->OnPlayerKilledByCreature(killer, killed);
    }

    void OnLevelChanged(Player* player, uint8 oldLevel) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnLevelChanged(player, oldLevel);
    }

    void OnFreeTalentPointsChanged(Player* player, uint32 points) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnFreeTalentPointsChanged(player, points);
    }

    void OnTalentsReset(Player* player, bool noCost) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnTalentsReset(player, noCost);
    }

    void OnMoneyChanged(Player* player, int32& amount) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnMoneyChanged(player, amount);
    }

    void OnGiveXP(Player* player, uint32& amount, Unit* victim, uint8 xpSource) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnGiveXP(player, amount, victim, xpSource);
    }

    bool OnReputationChange(Player* player, uint32 factionID, int32& standing, bool incremental) override
    {
        if (Eluna* e = player->GetEluna()) 
            return e->OnReputationChange(player, factionID, standing, incremental);

        return true;
    }

    void OnDuelRequest(Player* target, Player* challenger) override
    {
        if (Eluna* e = challenger->GetEluna()) 
            e->OnDuelRequest(target, challenger);
    }

    void OnDuelStart(Player* player1, Player* player2) override
    {
        if (Eluna* e = player1->GetEluna()) 
            e->OnDuelStart(player1, player2);
    }

    void OnDuelEnd(Player* winner, Player* loser, DuelCompleteType type) override
    {
        if (Eluna* e = winner->GetEluna()) 
            e->OnDuelEnd(winner, loser, type);
    }

    void OnEmote(Player* player, uint32 emote) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnEmote(player, emote);
    }

    void OnTextEmote(Player* player, uint32 textEmote, uint32 emoteNum, ObjectGuid guid) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnTextEmote(player, textEmote, emoteNum, guid);
    }

    void OnSpellCast(Player* player, Spell* spell, bool skipCheck) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnPlayerSpellCast(player, spell, skipCheck);
    }

    void OnLogin(Player* player) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnLogin(player);
    }

    void OnLogout(Player* player) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnLogout(player);
    }

    void OnCreate(Player* player) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnCreate(player);
    }

    void OnSave(Player* player) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnSave(player);
    }

    void OnDelete(ObjectGuid guid, uint32 /*accountId*/) override
    {
        if (Eluna* e = sWorld->GetEluna()) 
            e->OnDelete(guid.GetCounter());
    }

    void OnBindToInstance(Player* player, Difficulty difficulty, uint32 mapid, bool permanent) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnBindToInstance(player, difficulty, mapid, permanent);
    }

    void OnUpdateArea(Player* player, uint32 oldArea, uint32 newArea) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnUpdateArea(player, oldArea, newArea);
    }

    void OnUpdateZone(Player* player, uint32 newZone, uint32 newArea) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnUpdateZone(player, newZone, newArea);
    }

    void OnFirstLogin(Player* player) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnFirstLogin(player);
    }

    void OnLearnSpell(Player* player, uint32 spellId) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnLearnSpell(player, spellId);
    }

    void OnAchiComplete(Player* player, AchievementEntry const* achievement) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnAchiComplete(player, achievement);
    }

    void OnFfaPvpStateUpdate(Player* player, bool IsFlaggedForFfaPvp) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnFfaPvpStateUpdate(player, IsFlaggedForFfaPvp);
    }

    bool CanInitTrade(Player* player, Player* target) override
    {
        if (Eluna* e = player->GetEluna()) 
            return e->OnCanInitTrade(player, target);

        return true;
    }

    bool CanSendMail(Player* player, ObjectGuid receiverGuid, ObjectGuid mailbox, std::string& subject, std::string& body, uint32 money, uint32 cod, Item* item) override
    {
        if (Eluna* e = player->GetEluna()) 
            return e->OnCanSendMail(player, receiverGuid, mailbox, subject, body, money, cod, item);

        return true;
    }

    bool CanJoinLfg(Player* player, uint8 roles, lfg::LfgDungeonSet& dungeons, const std::string& comment) override
    {
        if (Eluna* e = player->GetEluna()) 
            return e->OnCanJoinLfg(player, roles, dungeons, comment);

        return true;
    }

    void OnQuestRewardItem(Player* player, Item* item, uint32 count) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnQuestRewardItem(player, item, count);
    }

    void OnGroupRollRewardItem(Player* player, Item* item, uint32 count, RollVote voteType, Roll* roll) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnGroupRollRewardItem(player, item, count, voteType, roll);
    }

    void OnCreateItem(Player* player, Item* item, uint32 count) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnCreateItem(player, item, count);
    }

    void OnStoreNewItem(Player* player, Item* item, uint32 count) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnStoreNewItem(player, item, count);
    }

    void OnPlayerCompleteQuest(Player* player, Quest const* quest) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnPlayerCompleteQuest(player, quest);
    }

    bool CanGroupInvite(Player* player, std::string& memberName) override
    {
        if (Eluna* e = player->GetEluna()) 
            return e->OnCanGroupInvite(player, memberName);

        return true;
    }

    void OnBattlegroundDesertion(Player* player, const BattlegroundDesertionType type) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnBattlegroundDesertion(player, type);
    }

    void OnCreatureKilledByPet(Player* player, Creature* killed) override
    {
        if (Eluna* e = player->GetEluna()) 
            e->OnCreatureKilledByPet(player, killed);
    }
};

class Eluna_ServerScript : public ServerScript
{
public:
    Eluna_ServerScript() : ServerScript("Eluna_ServerScript") { }

    bool CanPacketSend(WorldSession* session, WorldPacket& packet) override
    {
        if (Eluna* e = sWorld->GetEluna()) 
            if (!e->OnPacketSend(session, packet))
                return false;

        return true;
    }

    bool CanPacketReceive(WorldSession* session, WorldPacket& packet) override
    {
        if (Eluna* e = sWorld->GetEluna()) 
            if (!e->OnPacketReceive(session, packet))
                return false;

        return true;
    }
};

class Eluna_SpellSC : public SpellSC
{
public:
    Eluna_SpellSC() : SpellSC("Eluna_SpellSC") { }

    void OnDummyEffect(WorldObject* caster, uint32 spellID, SpellEffIndex effIndex, GameObject* gameObjTarget) override
    {
        if (Eluna* e = caster->GetEluna()) 
            e->OnDummyEffect(caster, spellID, effIndex, gameObjTarget);
    }

    void OnDummyEffect(WorldObject* caster, uint32 spellID, SpellEffIndex effIndex, Creature* creatureTarget) override
    {
        if (Eluna* e = caster->GetEluna()) 
            e->OnDummyEffect(caster, spellID, effIndex, creatureTarget);
    }

    void OnDummyEffect(WorldObject* caster, uint32 spellID, SpellEffIndex effIndex, Item* itemTarget) override
    {
        if (Eluna* e = caster->GetEluna()) 
            e->OnDummyEffect(caster, spellID, effIndex, itemTarget);
    }

    void OnSpellCastCancel(Spell* spell, Unit* caster, SpellInfo const* spellInfo, bool bySelf) override
    {
        if (Eluna* e = caster->GetEluna()) 
            e->OnSpellCastCancel(caster, spell, spellInfo, bySelf);
    }

    void OnSpellCast(Spell* spell, Unit* caster, SpellInfo const* spellInfo, bool skipCheck) override
    {
        if (Eluna* e = caster->GetEluna()) 
            e->OnSpellCast(caster, spell, spellInfo, skipCheck);
    }

    void OnSpellPrepare(Spell* spell, Unit* caster, SpellInfo const* spellInfo) override
    {
        if (Eluna* e = caster->GetEluna()) 
            e->OnSpellPrepare(caster, spell, spellInfo);
    }
};

class Eluna_UnitScript : public UnitScript
{
public:
    Eluna_UnitScript() : UnitScript("Eluna_UnitScript") { }

    void OnUnitUpdate(Unit* unit, uint32 diff) override
    {
        unit->elunaEvents->Update(diff);
    }
};

class Eluna_VehicleScript : public VehicleScript
{
public:
    Eluna_VehicleScript() : VehicleScript("Eluna_VehicleScript") { }

    void OnInstall(Vehicle* veh) override
    {
        if (Eluna* e = veh->GetBase()->GetEluna()) 
            e->OnInstall(veh);
    }

    void OnUninstall(Vehicle* veh) override
    {
        if (Eluna* e = veh->GetBase()->GetEluna()) 
            e->OnUninstall(veh);
    }

    void OnInstallAccessory(Vehicle* veh, Creature* accessory) override
    {
        if (Eluna* e = veh->GetBase()->GetEluna()) 
            e->OnInstallAccessory(veh, accessory);
    }

    void OnAddPassenger(Vehicle* veh, Unit* passenger, int8 seatId) override
    {
        if (Eluna* e = veh->GetBase()->GetEluna()) 
            e->OnAddPassenger(veh, passenger, seatId);
    }

    void OnRemovePassenger(Vehicle* veh, Unit* passenger) override
    {
        if (Eluna* e = veh->GetBase()->GetEluna()) 
            e->OnRemovePassenger(veh, passenger);
    }
};

class Eluna_WorldObjectScript : public WorldObjectScript
{
public:
    Eluna_WorldObjectScript() : WorldObjectScript("Eluna_WorldObjectScript") { }

    void OnWorldObjectDestroy(WorldObject* object) override
    {
        delete object->elunaEvents;
        object->elunaEvents = nullptr;
    }

    void OnWorldObjectCreate(WorldObject* object) override
    {
        object->elunaEvents = nullptr;
    }

    void OnWorldObjectSetMap(WorldObject* object, Map* /*map*/) override
    {
        if (!object->elunaEvents)
            object->elunaEvents = new ElunaEventProcessor(object->GetEluna(), object);
    }

    void OnWorldObjectUpdate(WorldObject* object, uint32 diff) override
    {
        object->elunaEvents->Update(diff);
    }
};

class Eluna_WorldScript : public WorldScript
{
public:
    Eluna_WorldScript() : WorldScript("Eluna_WorldScript") { }

    void OnOpenStateChange(bool open) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnOpenStateChange(open);
    }

    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload)
        {
            ///- Initialize Lua Engine
            ELUNA_LOG_INFO("Initialize Eluna Lua Engine...");
            sElunaConfig->Initialize();

            ///- Initialize Lua Engine
            if (sElunaConfig->IsElunaEnabled())
            {
                ELUNA_LOG_INFO("Loading Lua scripts...");
                sElunaLoader->LoadScripts();
            }

            if (sElunaConfig->IsElunaEnabled())
            {
                ELUNA_LOG_INFO("Starting Eluna world state...");
                sWorld->eluna = new Eluna(nullptr, sElunaConfig->IsElunaCompatibilityMode());
            }
        }

        if (Eluna* e = sWorld->GetEluna())
            e->OnConfigLoad(reload, true);
    }

    void OnAfterConfigLoad(bool reload) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnConfigLoad(reload, false);
    }

    void OnShutdownInitiate(ShutdownExitCode code, ShutdownMask mask) override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnShutdownInitiate(code, mask);
    }

    void OnShutdownCancel() override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnShutdownCancel();
    }

    void OnUpdate(uint32 diff) override
    {
        if (Eluna* e = sWorld->GetEluna())
        {
            e->UpdateEluna(diff);
            e->OnWorldUpdate(diff);
        }
    }

    void OnStartup() override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnStartup();
    }

    void OnShutdown() override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnShutdown();
    }

    void OnAfterUnloadAllMaps() override
    {
        // if (Eluna* e = sWorld->GetEluna())
            // e->Uninitialize();
    }

    void OnBeforeWorldInitialized() override
    {
        if (Eluna* e = sWorld->GetEluna())
            e->OnConfigLoad(false, false); // Must be done after Eluna is initialized and scripts have run.
    }
};

// Group all custom scripts
void AddSC_ElunaLuaEngine()
{
    new Eluna_AllCreatureScript();
    new Eluna_AllGameObjectScript();
    new Eluna_AllItemScript();
    new Eluna_AllMapScript();
    new Eluna_AuctionHouseScript();
    new Eluna_BGScript();
    new Eluna_CommandSC();
    new Eluna_ElunaScript();
    new Eluna_GameEventScript();
    new Eluna_GroupScript();
    new Eluna_GuildScript();
    new Eluna_LootScript();
    new Eluna_MiscScript();
    new Eluna_PetScript();
    new Eluna_PlayerScript();
    new Eluna_ServerScript();
    new Eluna_SpellSC();
    new Eluna_UnitScript();
    new Eluna_VehicleScript();
    new Eluna_WorldObjectScript();
    new Eluna_WorldScript();
}
