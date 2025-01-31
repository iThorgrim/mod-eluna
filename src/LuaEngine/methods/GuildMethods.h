/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef GUILDMETHODS_H
#define GUILDMETHODS_H

/***
 * Inherits all methods from: none
 */
namespace LuaGuild
{
    /**
     * Returns a table with the [Player]s in this [Guild]
     *
     * Only the players that are online and on some map.
     *
     * @return table guildPlayers : table of [Player]s
     */
    int GetMembers(Eluna* E, Guild* guild)
    {
        lua_newtable(E->L);
        int tbl = lua_gettop(E->L);
        uint32 i = 0;

        {
            std::shared_lock<std::shared_mutex> lock(*HashMapHolder<Player>::GetLock());
            const HashMapHolder<Player>::MapType& m = eObjectAccessor()GetPlayers();
            for (HashMapHolder<Player>::MapType::const_iterator it = m.begin(); it != m.end(); ++it)
            {
                if (Player* player = it->second)
                {
                    if (player->IsInWorld() && player->GetGuildId() == guild->GetId())
                    {
                        E->Push(player);
                        lua_rawseti(E->L, tbl, ++i);
                    }
                }
            }
        }

        lua_settop(E->L, tbl); // push table to top of stack
        return 1;
    }

    /**
     * Returns the member count of this [Guild]
     *
     * @return uint32 memberCount
     */
    int GetMemberCount(Eluna* E, Guild* guild)
    {
        E->Push(guild->GetMemberCount());
        return 1;
    }

    /**
     * Finds and returns the [Guild] leader by their GUID if logged in
     *
     * @return [Player] leader
     */
    int GetLeader(Eluna* E, Guild* guild)
    {
        E->Push(eObjectAccessor()FindPlayer(guild->GetLeaderGUID()));
        return 1;
    }

    /**
     * Returns [Guild] leader GUID
     *
     * @return ObjectGuid leaderGUID
     */
    int GetLeaderGUID(Eluna* E, Guild* guild)
    {
        E->Push(guild->GetLeaderGUID());
        return 1;
    }

    /**
     * Returns the [Guild]s entry ID
     *
     * @return uint32 entryId
     */
    int GetId(Eluna* E, Guild* guild)
    {
        E->Push(guild->GetId());
        return 1;
    }

    /**
     * Returns the [Guild]s name
     *
     * @return string guildName
     */
    int GetName(Eluna* E, Guild* guild)
    {
        E->Push(guild->GetName());
        return 1;
    }

    /**
     * Returns the [Guild]s current Message Of The Day
     *
     * @return string guildMOTD
     */
    int GetMOTD(Eluna* E, Guild* guild)
    {
        E->Push(guild->GetMOTD());
        return 1;
    }

    /**
     * Returns the [Guild]s current info
     *
     * @return string guildInfo
     */
    int GetInfo(Eluna* E, Guild* guild)
    {
        E->Push(guild->GetInfo());
        return 1;
    }

    /**
     * Sets the leader of this [Guild]
     *
     * @param [Player] leader : the [Player] leader to change
     */
    int SetLeader(Eluna* E, Guild* guild)
    {
        Player* player = E->CHECKOBJ<Player>(2);

        guild->HandleSetLeader(player->GetSession(), player->GetName());
        return 0;
    }

    /**
     * Sets the information of the bank tab specified
     *
     * @param uint8 tabId : the ID of the tab specified
     * @param string info : the information to be set to the bank tab
     */
    int SetBankTabText(Eluna* E, Guild* guild)
    {
        uint8 tabId = E->CHECKVAL<uint8>(2);
        const char* text = E->CHECKVAL<const char*>(3);
        guild->SetBankTabText(tabId, text);
        return 0;
    }

    // SendPacketToGuild(packet)
    /**
     * Sends a [WorldPacket] to all the [Player]s in the [Guild]
     *
     * @param [WorldPacket] packet : the [WorldPacket] to be sent to the [Player]s
     */
    int SendPacket(Eluna* E, Guild* guild)
    {
        WorldPacket* data = E->CHECKOBJ<WorldPacket>(2);

        guild->BroadcastPacket(data);
        return 0;
    }

    // SendPacketToRankedInGuild(packet, rankId)
    /**
     * Sends a [WorldPacket] to all the [Player]s at the specified rank in the [Guild]
     *
     * @param [WorldPacket] packet : the [WorldPacket] to be sent to the [Player]s
     * @param uint8 rankId : the rank ID
     */
    int SendPacketToRanked(Eluna* E, Guild* guild)
    {
        WorldPacket* data = E->CHECKOBJ<WorldPacket>(2);
        uint8 ranked = E->CHECKVAL<uint8>(3);

        guild->BroadcastPacketToRank(data, ranked);
        return 0;
    }

    /**
     * Disbands the [Guild]
     */
    int Disband(Eluna* /*E*/, Guild* guild)
    {
        guild->Disband();
        return 0;
    }

    /**
     * Adds the specified [Player] to the [Guild] at the specified rank.
     *
     * If no rank is specified, defaults to none.
     *
     * @param [Player] player : the [Player] to be added to the guild
     * @param uint8 rankId : the rank ID
     */
    int AddMember(Eluna* E, Guild* guild)
    {
        Player* player = E->CHECKOBJ<Player>(2);
        uint8 rankId = E->CHECKVAL<uint8>(3, GUILD_RANK_NONE);

        guild->AddMember(player->GET_GUID(), rankId);
        return 0;
    }

    /**
     * Removes the specified [Player] from the [Guild].
     *
     * @param [Player] player : the [Player] to be removed from the guild
     * @param bool isDisbanding : default 'false', should only be set to 'true' if the guild is triggered to disband
     */
    int DeleteMember(Eluna* E, Guild* guild)
    {
        Player* player = E->CHECKOBJ<Player>(2);
        bool isDisbanding = E->CHECKVAL<bool>(3, false);

        guild->DeleteMember(player->GET_GUID(), isDisbanding);
        return 0;
    }

    /**
     * Promotes/demotes the [Player] to the specified rank.
     *
     * @param [Player] player : the [Player] to be promoted/demoted
     * @param uint8 rankId : the rank ID
     */
    int SetMemberRank(Eluna* E, Guild* guild)
    {
        Player* player = E->CHECKOBJ<Player>(2);
        uint8 newRank = E->CHECKVAL<uint8>(3);

        guild->ChangeMemberRank(player->GET_GUID(), newRank);
        return 0;
    }

    /**
     * Sets the new name of the specified [Guild].
     *
     * @param string name : new name of this guild
     */
    int SetName(Eluna* E, Guild* guild)
    {
        std::string name = E->CHECKVAL<std::string>(2);
        
        guild->SetName(name);
        return 0;
    }

    /**
     * Update [Player] data in [Guild] member list.
     * 
     *     enum GuildMemberData
     *     {
     *         GUILD_MEMBER_DATA_ZONEID =  0
     *         GUILD_MEMBER_DATA_LEVEL  =  1
     *     };
     * 
     *  @param [Player] player = plkayer you need to update data
     *  @param [GuildMemberData] dataid = data you need to update
     *  @param uint32 value
     */
    int UpdateMemberData(Eluna* E, Guild* guild)
    {
        Player* player = E->CHECKOBJ<Player>(2);
        uint8 dataid = E->CHECKVAL<uint8>(3);
        uint32 value = E->CHECKVAL<uint32>(4);

        guild->UpdateMemberData(player, dataid, value);
        return 0;
    }

    /**
     * Send message to [Guild] from specific [Player].
     * 
     * @param [Player] player = the [Player] is the author of the message
     * @param bool officierOnly = send message only on officier channel
     * @param string msg = the message you need to send
     * @param uint32 lang = language the [Player] will speak
     */
    int SendMessage(Eluna* E, Guild* guild)
    {
        Player* player = E->CHECKOBJ<Player>(2);
        bool officierOnly = E->CHECKVAL<bool>(3, false);
        std::string msg = E->CHECKVAL<std::string>(4);
        uint32 language = E->CHECKVAL<uint32>(5, false);

        guild->BroadcastToGuild(player->GetSession(), officierOnly, msg, language);
        return 0;
    }

    /**
     * Invites [Guild] members to events based on level and rank filters.
     * 
     * @param Player player = who sends the invitation
     * @param minLevel = the required min level
     * @param maxLevel = the required max level
     * @param minRank = the required min rank
     */
    int MassInviteToEvent(Eluna* E, Guild* guild)
    { 
        Player* player = E->CHECKOBJ<Player>(2);
        uint32 minLevel = E->CHECKVAL<uint32>(3);
        uint32 maxLevel = E->CHECKVAL<uint32>(4);
        uint32 minRank = E->CHECKVAL<uint32>(5);

        guild->MassInviteToEvent(player->GetSession(), minLevel, maxLevel, minRank);
        return 0;
    }

    /**
     * Swap item from a specific tab and slot [Guild] bank to another one.
     * 
     * @param [Player] player = who Swap the item
     * @param uint8 tabId = source tab id
     * @param uint8 slotId = source slot id
     * @param uint8 destTabId = destination tab id
     * @param uint8 destSlotId = destination slot id
     * @param uint8 splitedAmount = if the item is stackable, how much should be swaped
     */
    int SwapItems(Eluna* E, Guild* guild)
    { 
        Player* player = E->CHECKOBJ<Player>(2);
        uint8 tabId = E->CHECKVAL<uint32>(3);
        uint8 slotId = E->CHECKVAL<uint32>(4);
        uint8 destTabId = E->CHECKVAL<uint32>(5);
        uint8 destSlotId = E->CHECKVAL<uint32>(6);
        uint32 splitedAmount = E->CHECKVAL<uint32>(7);

        guild->SwapItems(player, tabId, slotId, destTabId, destSlotId, splitedAmount);
        return 0;
    }

    /**
     * Swap an item from a specific tab and location in the [guild] bank to the bags and locations in the inventory of a specific [player] and vice versa.
     * 
     * @param [Player] player = who Swap the item
     * @param bool toChar = the item goes to the [Player]'s inventory or comes from the [Player]'s inventory
     * @param uint8 tabId = tab id
     * @param uint8 slotId = slot id
     * @param uint8 playerBag = bag id
     * @param uint8 playerSlotId = slot id
     * @param uint32 splitedAmount = if the item is stackable, how much should be swaped
     */
    int SwapItemsWithInventory(Eluna* E, Guild* guild)
    { 
        Player* player = E->CHECKOBJ<Player>(2);
        bool toChar = E->CHECKVAL<bool>(3, false);
        uint8 tabId = E->CHECKVAL<uint8>(4);
        uint8 slotId = E->CHECKVAL<uint8>(5);
        uint8 playerBag = E->CHECKVAL<uint8>(6);
        uint8 playerSlotId = E->CHECKVAL<uint8>(7);
        uint32 splitedAmount = E->CHECKVAL<uint32>(8);

        guild->SwapItemsWithInventory(player, toChar, tabId, slotId, playerBag, playerSlotId, splitedAmount);
        return 0;
    }

    /**
     * Return the total bank money.
     * 
     * @return number totalBankMoney
     */
    int GetTotalBankMoney(Eluna* E, Guild* guild)
    { 
        E->Push(guild->GetTotalBankMoney());
        return 1;
    }

    /**
     * Return the created date.
     * 
     * @return uint64 created date
     */
    int GetCreatedDate(Eluna* E, Guild* guild)
    { 
        E->Push(guild->GetCreatedDate());
        return 1;
    }

    /**
     * Resets the number of item withdraw in all tab's for all [Guild] members.
     */
    int ResetTimes(Eluna* E, Guild* guild)
    { 
        guild->ResetTimes();
        return 0;
    }

    /**
     * Modify the [Guild] bank money. You can deposit or withdraw.
     * 
     * @param uint64 amount = amount to add or remove
     * @param bool add = true (add money) | false (withdraw money)
     * @return bool is_applied
     */
    int ModifyBankMoney(Eluna* E, Guild* guild)
    { 
        uint64 amount = E->CHECKVAL<uint64>(2);
        bool add = E->CHECKVAL<bool>(2);

        CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
        E->Push(guild->ModifyBankMoney(trans, amount, add));

        CharacterDatabase.CommitTransaction(trans);
        return 1;
    }

    ElunaRegister<Guild> GuildMethods[] =
    {
        // Getters
        { "GetMembers", &LuaGuild::GetMembers },
        { "GetLeader", &LuaGuild::GetLeader },
        { "GetLeaderGUID", &LuaGuild::GetLeaderGUID },
        { "GetId", &LuaGuild::GetId },
        { "GetName", &LuaGuild::GetName },
        { "GetMOTD", &LuaGuild::GetMOTD },
        { "GetInfo", &LuaGuild::GetInfo },
        { "GetMemberCount", &LuaGuild::GetMemberCount },
        { "GetCreatedDate", &LuaGuild::GetCreatedDate },
        { "GetTotalBankMoney", &LuaGuild::GetTotalBankMoney },

        // Setters
        { "SetBankTabText", &LuaGuild::SetBankTabText },
        { "SetMemberRank", &LuaGuild::SetMemberRank },
        { "SetLeader", &LuaGuild::SetLeader },
        { "SetName", &LuaGuild::SetName },

        // Other
        { "SendPacket", &LuaGuild::SendPacket },
        { "SendPacketToRanked", &LuaGuild::SendPacketToRanked },
        { "Disband", &LuaGuild::Disband },
        { "AddMember", &LuaGuild::AddMember },
        { "DeleteMember", &LuaGuild::DeleteMember },
        { "SendMessage", &LuaGuild::SendMessage },
        { "UpdateMemberData", &LuaGuild::UpdateMemberData },
        { "MassInviteToEvent", &LuaGuild::MassInviteToEvent },
        { "SwapItems", &LuaGuild::SwapItems },
        { "SwapItemsWithInventory", &LuaGuild::SwapItemsWithInventory },
        { "ResetTimes", &LuaGuild::ResetTimes },
        { "ModifyBankMoney", &LuaGuild::ModifyBankMoney }
    };
};
#endif

