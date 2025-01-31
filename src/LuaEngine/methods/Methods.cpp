/*
* Copyright (C) 2010 - 2024 Eluna Lua Engine <https://elunaluaengine.github.io/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

// Eluna
#include "LuaEngine.h"
#include "ElunaEventMgr.h"
#include "ElunaIncludes.h"
#include "ElunaTemplate.h"
#include "ElunaUtility.h"

// Method includes
#include "AchievementMethods.h"
#include "AuraMethods.h"
#include "BattleGroundMethods.h"
#include "ChatHandlerMethods.h"
#include "CorpseMethods.h"
#include "CreatureMethods.h"
#include "ElunaQueryMethods.h"
#include "GameObjectMethods.h"
#include "GlobalMethods.h"
#include "GroupMethods.h"
#include "GuildMethods.h"
#include "ItemMethods.h"
#include "ItemTemplateMethods.h"
#include "MapMethods.h"
#include "ObjectMethods.h"
#include "PlayerMethods.h"
#include "QuestMethods.h"
#include "RollMethods.h"
#include "SpellMethods.h"
#include "SpellInfoMethods.h"
#include "UnitMethods.h"
#include "VehicleMethods.h"
#include "WorldObjectMethods.h"
#include "WorldPacketMethods.h"

// DBCStores includes
#include "GemPropertiesEntryMethods.h"

// Custom methods
#include "CustomMethods.h"

void RegisterMethods(Eluna* E)
{

    ElunaTemplate<AchievementEntry>::Register(E, "AchievementEntry");
    ElunaTemplate<AchievementEntry>::SetMethods(E, LuaAchievement::AchievementMethods);

    ElunaTemplate<Aura>::Register(E, "Aura");
    ElunaTemplate<Aura>::SetMethods(E, LuaAura::AuraMethods);

    ElunaTemplate<BattleGround>::Register(E, "BattleGround");
    ElunaTemplate<BattleGround>::SetMethods(E, LuaBattleGround::BattleGroundMethods);

    ElunaTemplate<ChatHandler>::Register(E, "ChatHandler");
    ElunaTemplate<ChatHandler>::SetMethods(E, LuaChatHandler::ChatHandlerMethods);

    ElunaTemplate<Corpse>::Register(E, "Corpse");
    ElunaTemplate<Corpse>::SetMethods(E, LuaObject::ObjectMethods);
    ElunaTemplate<Corpse>::SetMethods(E, LuaWorldObject::WorldObjectMethods);
    ElunaTemplate<Corpse>::SetMethods(E, LuaCorpse::CorpseMethods);

    ElunaTemplate<Creature>::Register(E, "Creature");
    ElunaTemplate<Creature>::SetMethods(E, LuaObject::ObjectMethods);
    ElunaTemplate<Creature>::SetMethods(E, LuaWorldObject::WorldObjectMethods);
    ElunaTemplate<Creature>::SetMethods(E, LuaUnit::UnitMethods);
    ElunaTemplate<Creature>::SetMethods(E, LuaCreature::CreatureMethods);

    ElunaTemplate<ElunaQuery>::Register(E, "ElunaQuery");
    ElunaTemplate<ElunaQuery>::SetMethods(E, LuaQuery::QueryMethods);

    ElunaTemplate<GameObject>::Register(E, "GameObject");
    ElunaTemplate<GameObject>::SetMethods(E, LuaObject::ObjectMethods);
    ElunaTemplate<GameObject>::SetMethods(E, LuaWorldObject::WorldObjectMethods);
    ElunaTemplate<GameObject>::SetMethods(E, LuaGameObject::GameObjectMethods);

    ElunaTemplate<GemPropertiesEntry>::Register(E, "GemPropertiesEntry");
    ElunaTemplate<GemPropertiesEntry>::SetMethods(E, LuaGemPropertiesEntry::GemPropertiesEntryMethods);

    ElunaTemplate<>::SetMethods(E, LuaGlobalFunctions::GlobalMethods);

    ElunaTemplate<Group>::Register(E, "Group");
    ElunaTemplate<Group>::SetMethods(E, LuaGroup::GroupMethods);

    ElunaTemplate<Guild>::Register(E, "Guild");
    ElunaTemplate<Guild>::SetMethods(E, LuaGuild::GuildMethods);

    ElunaTemplate<Item>::Register(E, "Item");
    ElunaTemplate<Item>::SetMethods(E, LuaObject::ObjectMethods);
    ElunaTemplate<Item>::SetMethods(E, LuaItem::ItemMethods);

    ElunaTemplate<ItemTemplate>::Register(E, "ItemTemplate");
    ElunaTemplate<ItemTemplate>::SetMethods(E, LuaItemTemplate::ItemTemplateMethods);

    ElunaTemplate<Map>::Register(E, "Map");
    ElunaTemplate<Map>::SetMethods(E, LuaMap::MapMethods);

    ElunaTemplate<Object>::Register(E, "Object");
    ElunaTemplate<Object>::SetMethods(E, LuaObject::ObjectMethods);

    ElunaTemplate<Player>::Register(E, "Player");
    ElunaTemplate<Player>::SetMethods(E, LuaObject::ObjectMethods);
    ElunaTemplate<Player>::SetMethods(E, LuaWorldObject::WorldObjectMethods);
    ElunaTemplate<Player>::SetMethods(E, LuaUnit::UnitMethods);
    ElunaTemplate<Player>::SetMethods(E, LuaPlayer::PlayerMethods);

    ElunaTemplate<Quest>::Register(E, "Quest");
    ElunaTemplate<Quest>::SetMethods(E, LuaQuest::QuestMethods);

    ElunaTemplate<Roll>::Register(E, "Roll");
    ElunaTemplate<Roll>::SetMethods(E, LuaRoll::RollMethods);

    ElunaTemplate<Spell>::Register(E, "Spell");
    ElunaTemplate<Spell>::SetMethods(E, LuaSpell::SpellMethods);

    ElunaTemplate<SpellInfo>::Register(E, "SpellInfo");
    ElunaTemplate<SpellInfo>::SetMethods(E, LuaSpellInfo::SpellInfoMethods);

    ElunaTemplate<Unit>::Register(E, "Unit");
    ElunaTemplate<Unit>::SetMethods(E, LuaObject::ObjectMethods);
    ElunaTemplate<Unit>::SetMethods(E, LuaWorldObject::WorldObjectMethods);
    ElunaTemplate<Unit>::SetMethods(E, LuaUnit::UnitMethods);

    ElunaTemplate<Vehicle>::Register(E, "Vehicle");
    ElunaTemplate<Vehicle>::SetMethods(E, LuaVehicle::VehicleMethods);

    ElunaTemplate<WorldObject>::Register(E, "WorldObject");
    ElunaTemplate<WorldObject>::SetMethods(E, LuaObject::ObjectMethods);
    ElunaTemplate<WorldObject>::SetMethods(E, LuaWorldObject::WorldObjectMethods);

    ElunaTemplate<WorldPacket>::Register(E, "WorldPacket");
    ElunaTemplate<WorldPacket>::SetMethods(E, LuaPacket::PacketMethods);

    ElunaTemplate<long long>::Register(E, "long long");

    ElunaTemplate<unsigned long long>::Register(E, "unsigned long long");

    ElunaTemplate<ObjectGuid>::Register(E, "ObjectGuid");

    // Register custom methods
    LuaCustom::RegisterCustomMethods(E);
}

