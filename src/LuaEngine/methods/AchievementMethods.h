/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef ACHIEVEMENTMETHODS_H
#define ACHIEVEMENTMETHODS_H

namespace LuaAchievement
{
    /**
     * Returns the [Achievement]'s ID.
     *
     * @return uint32 id
     */
    int GetId(Eluna* E, AchievementEntry* const achievement)
    {
        E->Push(achievement->ID);
        return 1;
    }

    /**
     * Returns the [Achievement]'s name.
     *
     *     enum LocaleConstant
     *     {
     *         LOCALE_enUS = 0,
     *         LOCALE_koKR = 1,
     *         LOCALE_frFR = 2,
     *         LOCALE_deDE = 3,
     *         LOCALE_zhCN = 4,
     *         LOCALE_zhTW = 5,
     *         LOCALE_esES = 6,
     *         LOCALE_esMX = 7,
     *         LOCALE_ruRU = 8
     *     };
     *
     * @param [LocaleConstant] locale = DEFAULT_LOCALE : locale to return the [Achievement] name in
     * @return string name
     */
    int GetName(Eluna* E, AchievementEntry* const achievement)
    {
        uint8 locale = E->CHECKVAL<uint8>(2, DEFAULT_LOCALE);
        if (locale >= TOTAL_LOCALES)
        {
            return luaL_argerror(E->L, 2, "valid LocaleConstant expected");
        }

        E->Push(achievement->name[locale]);
        return 1;
    }

    ElunaRegister<AchievementEntry> AchievementMethods[] =
    {
        { "GetId", &LuaAchievement::GetId },
        { "GetName", &LuaAchievement::GetName },
    };
};
#endif

