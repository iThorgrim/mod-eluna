/*
 * Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
 * This program is free software licensed under GPL version 3
 * Please see the included DOCS/LICENSE.md for more information
 */

#include "Hooks.h"
#include "HookHelpers.h"
#include "LuaEngine.h"
#include "BindingMap.h"
#include "ElunaIncludes.h"
#include "ElunaTemplate.h"

using namespace Hooks;

#define START_HOOK(EVENT, ENTRY) \
    auto key = EntryKey<SpellEvents>(EVENT, ENTRY);\
    if (!SpellEventBindings->HasBindingsFor(key))\
        return;

#define START_HOOK_WITH_RETVAL(EVENT, ENTRY, RETVAL) \
    auto key = EntryKey<SpellEvents>(EVENT, ENTRY);\
    if (!SpellEventBindings->HasBindingsFor(key))\
        return RETVAL;

void Eluna::OnSpellCastCancel(Unit* caster, Spell* spell, SpellInfo const* spellInfo, bool bySelf)
{
    START_HOOK(SPELL_EVENT_ON_CAST_CANCEL, spellInfo->Id);
    HookPush(caster);
    HookPush(spell);
    HookPush(bySelf);

    CallAllFunctions(SpellEventBindings, key);
}

void Eluna::OnSpellCast(Unit* caster, Spell* spell, SpellInfo const* spellInfo, bool skipCheck)
{
    START_HOOK(SPELL_EVENT_ON_CAST, spellInfo->Id);
    HookPush(caster);
    HookPush(spell);
    HookPush(skipCheck);

    CallAllFunctions(SpellEventBindings, key);
}

void Eluna::OnSpellPrepare(Unit* caster, Spell* spell, SpellInfo const* spellInfo)
{
    START_HOOK(SPELL_EVENT_ON_PREPARE, spellInfo->Id);
    HookPush(caster);
    HookPush(spell);

    CallAllFunctions(SpellEventBindings, key);
}

