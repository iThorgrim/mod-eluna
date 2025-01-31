/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef CHATHANDLERMETHODS_H
#define CHATHANDLERMETHODS_H

namespace LuaChatHandler
{
    /**
     * Sends text to the chat handler
     *
     * @proto (text)
     * @proto (entry)
     * @param string text : text to display in chat or console
     * @param uint32 entry : id of the string to display
     */
    int SendSysMessage(Eluna* E, ChatHandler* handler)
    {
        if (lua_isnumber(E->L, 2))
        {
            uint32 entry = E->CHECKVAL<uint32>(2);
            handler->SendSysMessage(entry);
        }
        else
        {
            std::string text = E->CHECKVAL<std::string>(2);
            handler->SendSysMessage(text);
        }
        return 0;
    }

    /**
     * Returns `true` if the [ChatHandler] comes from the console, `false` if it comes from a player
     *
     * @return bool isConsole
     */
    int IsConsole(Eluna* E, ChatHandler* handler)
    {
        E->Push(handler->IsConsole());
        return 1;
    }

    /**
     * Returns the [Player] associated with the handler. Returns `nil` in the case of a console handler
     *
     * @return [Player] player
     */
    int GetPlayer(Eluna* E, ChatHandler* handler)
    {
        E->Push(handler->GetPlayer());
        return 1;
    }

    /**
     * Sends a message to all connected players
     *
     * @param string text : text to send
     */
    int SendGlobalSysMessage(Eluna* E, ChatHandler* handler)
    {
        std::string text = E->CHECKVAL<std::string>(2);
        handler->SendGlobalSysMessage(text.c_str());
        return 0;
    }

    /**
     * Sends a message to all connected Game Masters
     *
     * @param string text : text to send
     */
    int SendGlobalGMSysMessage(Eluna* E, ChatHandler* handler)
    {
        std::string text = E->CHECKVAL<std::string>(2);
        handler->SendGlobalGMSysMessage(text.c_str());
        return 0;
    }

    /**
     * Checks if the current security level is lower than the specified [Player]'s account
     *
     * @param [Player] player
     * @param [bool] strong = false : Forces non-player accounts (security level greater than `0`) to go through the regular check if set to `true`.<br>Also, if set to `true`, the current security level will be considered as lower than the [Player]'s security level if the two levels are equal
     * @return [bool] lower
     */
    int HasLowerSecurity(Eluna* E, ChatHandler* handler)
    {
        Player* player = E->CHECKOBJ<Player>(2);
        bool strong = E->CHECKVAL<bool>(3);
        E->Push(handler->HasLowerSecurity(player, ObjectGuid::Empty, strong));
        return 1;
    }

    /**
     * Checks if the current security level is lower than the specified `account`'s level
     *
     * @param [uint32] account : the target account ID to compare security levels with
     * @param [bool] strong = false : Forces non-player accounts (security level greater than `0`) to go through the regular check if set to `true`.<br>Also, if set to `true`, the current security level will be considered as lower than the `account`'s security level if the two levels are equal
     * @return [bool] lower
     */
    int HasLowerSecurityAccount(Eluna* E, ChatHandler* handler)
    {
        uint32 account = E->CHECKVAL<uint32>(2);
        bool strong = E->CHECKVAL<bool>(3);
        E->Push(handler->HasLowerSecurityAccount(nullptr, account, strong));
        return 1;
    }

    /**
     * Returns the selected [Player]
     *
     * @return [Player] player
     */
    int GetSelectedPlayer(Eluna* E, ChatHandler* handler)
    {
        E->Push(handler->getSelectedPlayer());
        return 1;
    }

    /**
     * Returns the selected [Creature]
     *
     * @return [Creature] creature
     */
    int GetSelectedCreature(Eluna* E, ChatHandler* handler)
    {
        E->Push(handler->getSelectedCreature());
        return 1;
    }

    /**
     * Returns the selected [Unit]
     *
     * @return [Unit] unit
     */
    int GetSelectedUnit(Eluna* E, ChatHandler* handler)
    {
        E->Push(handler->getSelectedUnit());
        return 1;
    }

    /**
     * Returns the selected [WorldObject]
     *
     * @return [WorldObject] object
     */
    int GetSelectedObject(Eluna* E, ChatHandler* handler)
    {
        E->Push(handler->getSelectedObject());
        return 1;
    }

    /**
     * Returns the selected [Player] or the current [Player] if nothing is targeted or the target is not a player
     *
     * @return [Player] player
     */
    int GetSelectedPlayerOrSelf(Eluna* E, ChatHandler* handler)
    {
        E->Push(handler->getSelectedPlayerOrSelf());
        return 1;
    }

    /**
     * Checks if the `securityLevel` is available
     *
     * @param [uint32] securityLevel
     * @return [bool] isAvailable
     */
    int IsAvailable(Eluna* E, ChatHandler* handler)
    {
        uint32 securityLevel = E->CHECKVAL<uint32>(2);
        E->Push(handler->IsAvailable(securityLevel));
        return 1;
    }

    /**
     * Returns `true` if other previously called [ChatHandler] methods sent an error
     *
     * @return [bool] sentErrorMessage
     */
    int HasSentErrorMessage(Eluna* E, ChatHandler* handler)
    {
        E->Push(handler->HasSentErrorMessage());
        return 1;
    }
    
    ElunaRegister<ChatHandler> ChatHandlerMethods[] =
    {
        { "SendSysMessage", &LuaChatHandler::SendSysMessage },
        { "IsConsole", &LuaChatHandler::IsConsole },
        { "GetPlayer", &LuaChatHandler::GetPlayer },
        { "SendGlobalSysMessage", &LuaChatHandler::SendGlobalSysMessage },
        { "SendGlobalGMSysMessage", &LuaChatHandler::SendGlobalGMSysMessage },
        { "HasLowerSecurity", &LuaChatHandler::HasLowerSecurity },
        { "HasLowerSecurityAccount", &LuaChatHandler::HasLowerSecurityAccount },
        { "GetSelectedPlayer", &LuaChatHandler::GetSelectedPlayer },
        { "GetSelectedCreature", &LuaChatHandler::GetSelectedCreature },
        { "GetSelectedUnit", &LuaChatHandler::GetSelectedUnit },
        { "GetSelectedObject", &LuaChatHandler::GetSelectedObject },
        { "GetSelectedPlayerOrSelf", &LuaChatHandler::GetSelectedPlayerOrSelf },
        { "IsAvailable", &LuaChatHandler::IsAvailable },
        { "HasSentErrorMessage", &LuaChatHandler::HasSentErrorMessage },
    };
}
#endif

