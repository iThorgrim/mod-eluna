/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef ITEMTEMPLATEMETHODS_H
#define ITEMTEMPLATEMETHODS_H

namespace LuaItemTemplate
{
    /**
     * Returns the [ItemTemplate]'s ID.
     *
     * @return uint32 itemId
     */
    int GetItemId(Eluna* E, ItemTemplate* itemTemplate)
    {
        E->Push(itemTemplate->ItemId);
        return 1;
    }

    /**
     * Returns the [ItemTemplate]'s class.
     *
     * @return uint32 class
     */
    int GetClass(Eluna* E, ItemTemplate* itemTemplate)
    {
        E->Push(itemTemplate->Class);
        return 1;
    }

    /**
     * Returns the [ItemTemplate]'s subclass.
     *
     * @return uint32 subClass
     */
    int GetSubClass(Eluna* E, ItemTemplate* itemTemplate)
    {
        E->Push(itemTemplate->SubClass);
        return 1;
    }

    /**
     * Returns the [ItemTemplate]'s name in the [Player]'s locale.
     *
     * @param [LocaleConstant] locale = DEFAULT_LOCALE : locale to return the [ItemTemplate] name in (it's optional default: LOCALE_enUS)
     * 
     * @return string name
     */
    int GetName(Eluna* E, ItemTemplate* itemTemplate)
    {
        uint32 loc_idx = E->CHECKVAL<uint32>(2, LocaleConstant::LOCALE_enUS);

        const ItemLocale* itemLocale = eObjectMgr->GetItemLocale(itemTemplate->ItemId);
        std::string name = itemTemplate->Name1;

        if (itemLocale && !itemLocale->Name[loc_idx].empty())
            name = itemLocale->Name[loc_idx];

        E->Push(name);
        return 1;
    }

    /**
     * Returns the [ItemTemplate]'s display ID.
     *
     * @return uint32 displayId
     */
    int GetDisplayId(Eluna* E, ItemTemplate* itemTemplate)
    {
        E->Push(itemTemplate->DisplayInfoID);
        return 1;
    }

    /**
     * Returns the [ItemTemplate]'s quality.
     *
     * @return uint32 quality
     */
    int GetQuality(Eluna* E, ItemTemplate* itemTemplate)
    {
        E->Push(itemTemplate->Quality);
        return 1;
    }

    /**
     * Returns the [ItemTemplate]'s flags.
     *
     * @return uint32 flags
     */
    int GetFlags(Eluna* E, ItemTemplate* itemTemplate)
    {
        E->Push(itemTemplate->Flags);
        return 1;
    }

    /**
     * Returns the [ItemTemplate]'s extra flags.
     *
     * @return uint32 flags
     */
    int GetExtraFlags(Eluna* E, ItemTemplate* itemTemplate)
    {
        E->Push(itemTemplate->Flags2);
        return 1;
    }

    /**
     * Returns the [ItemTemplate]'s default purchase count.
     *
     * @return uint32 buyCount
     */
    int GetBuyCount(Eluna* E, ItemTemplate* itemTemplate)
    {
        E->Push(itemTemplate->BuyCount);
        return 1;
    }

    /**
     * Returns the [ItemTemplate]'s purchase price.
     *
     * @return int32 buyPrice
     */
    int GetBuyPrice(Eluna* E, ItemTemplate* itemTemplate)
    {
        E->Push(itemTemplate->BuyPrice);
        return 1;
    }

    /**
     * Returns the [ItemTemplate]'s sell price.
     *
     * @return uint32 sellPrice
     */
    int GetSellPrice(Eluna* E, ItemTemplate* itemTemplate)
    {
        E->Push(itemTemplate->SellPrice);
        return 1;
    }

    /**
     * Returns the [ItemTemplate]'s inventory type.
     *
     * @return uint32 inventoryType
     */
    int GetInventoryType(Eluna* E, ItemTemplate* itemTemplate)
    {
        E->Push(itemTemplate->InventoryType);
        return 1;
    }

    /**
     * Returns the [Player] classes allowed to use this [ItemTemplate].
     *
     * @return uint32 allowableClass
     */
    int GetAllowableClass(Eluna* E, ItemTemplate* itemTemplate)
    {
        E->Push(itemTemplate->AllowableClass);
        return 1;
    }

    /**
     * Returns the [Player] races allowed to use this [ItemTemplate].
     *
     * @return uint32 allowableRace
     */
    int GetAllowableRace(Eluna* E, ItemTemplate* itemTemplate)
    {
        E->Push(itemTemplate->AllowableRace);
        return 1;
    }

    /**
     * Returns the [ItemTemplate]'s item level.
     *
     * @return uint32 itemLevel
     */
    int GetItemLevel(Eluna* E, ItemTemplate* itemTemplate)
    {
        E->Push(itemTemplate->ItemLevel);
        return 1;
    }

    /**
     * Returns the minimum level required to use this [ItemTemplate].
     *
     * @return uint32 requiredLevel
     */
    int GetRequiredLevel(Eluna* E, ItemTemplate* itemTemplate)
    {
        E->Push(itemTemplate->RequiredLevel);
        return 1;
    }

    ElunaRegister<ItemTemplate> ItemTemplateMethods[] =
    {
        { "GetItemId", &LuaItemTemplate::GetItemId },
        { "GetClass", &LuaItemTemplate::GetClass },
        { "GetSubClass", &LuaItemTemplate::GetSubClass },
        { "GetName", &LuaItemTemplate::GetName },
        { "GetDisplayId", &LuaItemTemplate::GetDisplayId },
        { "GetQuality", &LuaItemTemplate::GetQuality },
        { "GetFlags", &LuaItemTemplate::GetFlags },
        { "GetExtraFlags", &LuaItemTemplate::GetExtraFlags },
        { "GetBuyCount", &LuaItemTemplate::GetBuyCount },
        { "GetBuyPrice", &LuaItemTemplate::GetBuyPrice },
        { "GetSellPrice", &LuaItemTemplate::GetSellPrice },
        { "GetInventoryType", &LuaItemTemplate::GetInventoryType },
        { "GetAllowableClass", &LuaItemTemplate::GetAllowableClass },
        { "GetAllowableRace", &LuaItemTemplate::GetAllowableRace },
        { "GetItemLevel", &LuaItemTemplate::GetItemLevel },
        { "GetRequiredLevel", &LuaItemTemplate::GetRequiredLevel }
    };
}

#endif

