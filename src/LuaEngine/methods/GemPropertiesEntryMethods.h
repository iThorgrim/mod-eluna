/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef GEMPROPERTIESENTRYMETHODS_H
#define GEMPROPERTIESENTRYMETHODS_H

namespace LuaGemPropertiesEntry
{

    /**
     * Returns the ID of a [GemPropertiesEntry].
     *
     * This method retrieves the ID from a given GemPropertiesEntry instance 
     * and pushes it onto the Lua stack.
     *
     * @return uint32 id : The ID of the specified GemPropertiesEntry.
     */
    int GetId(Eluna* E, GemPropertiesEntry* gemProperties)
    {
        E->Push(gemProperties->ID);
        return 1;
    }

    /**
     * Returns the spell item enchantment of a [GemPropertiesEntry].
     *
     * This function retrieves the `spellitemenchantement` attribute from the provided `GemPropertiesEntry`.
     *
     * @return uint32 spellitemenchantement : The spell item enchantment ID.
     */
    int GetSpellItemEnchantement(Eluna* E, GemPropertiesEntry* entry)
    {
        E->Push(entry->spellitemenchantement);
        return 1;
    }

    ElunaRegister<GemPropertiesEntry> GemPropertiesEntryMethods[] =
    {
        // Getters
        { "GetId", &LuaGemPropertiesEntry::GetId },
        { "GetSpellItemEnchantement", &LuaGemPropertiesEntry::GetSpellItemEnchantement }
    };
}
#endif

