/*
    Copyright © 2013, 2014 Aaron John Schlosser

    This file is part of 2dpcrpg-old by Aaron John Schlosser.

    2dpcrpg is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    2dpcrpg is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 2dpcrpg.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INC_THING_H
#define INC_THING_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "objects.h"

enum ThingCategory {
    FLORA = 1,
    BLOCK,
    WEAPON
};

enum WeaponTypes {

    // FLORA
    TREE_PINE = 1,
    ROCK,

    // BLOCKS
    MUDSTONE,
    MUDSTONE_BROKEN,
    GREYSTONE,
    GREYSTONE_BROKEN,
    SANDSTONE,
    SANDSTONE_BROKEN,
    BLACKSTONE,
    BLACKSTONE_BROKEN,
    BLOODSTONE,
    BLOODSTONE_BROKEN,
    BLUESTONE,
    BLUESTONE_BROKEN,
    FLUXSTONE,
    FLUXSTONE_BROKEN,

    // WEAPONS
    BROKEN_DAGGER,
    BROKEN_SHORTSWORD,
    BROKEN_BROADSWORD,
    SHORTSWORD,
    CHIPPED_SHORTSWORD,
    RAPIER,
    LONGSWORD,
    LONGSWORD_2,
    BASTARD_SWORD,
    SABRE,
    LONGSWORD_3,
    SCIMITAR,
    TWOHANDED_SWORD,
    BROADSWORD,
    SCIMITAR_2,
    BROADSWORD_2,
    RARE_SWORD,
    ENCHANTED_RARE_SWORD,
    WHIP,
    HAMMER,
    KNOBBED_MACE,
    STAFF,
    FLAIL,
    FLAIL_2,
    FLANGED_MACE,
    BALL_AND_CHAIN,
    FLAIL_3,
    ENCHANTED_MACE,
    SPEAR,
    TRIDENT,
    SPEAR_2,
    LANCE,
    WARHAMMER,
    HATCHET,
    POLEARM,
    AXE,
    BATTLEAXE,
    GLAIVE,
    POLEAXE,
    HALBERD,
    POLEAXE_2,
    SCYTHE,
    SCYTHE_2,
    SHORTBOW,
    LONGBOW,
    CROSSBOW,
    CROSSBOW_2,
    SLING,
    RARE_WARHAMMER,
    ARROW,
    STEEL_ARROW,
    BOLT,
    STEEL_BOLT,
    BUCKSHOT,
    SHOT,
    RARE_STAFF,
    RARE_TRIDENT,
    SPEAR_3,
    DAGGER,
    SHORTSWORD_2,
    RARE_CROSSBOW,
    RARE_LONGBOW,
    RARE_SHORTSWORD,
    RARE_HAMMER,
    RARE_RAPIER,
    RARE_LONGSWORD,
    RARE_BROADSWORD,
    RARE_ENCHANTED_LONGSWORD,
    RARE_ENCHANTED_LONGSWORD_2,
    RARE_POLEAXE,
    RARE_TWOHANDED_SWORD,
    RARE_FLAIL,
    RARE_SPEAR,
    RARE_TRIDENT_2,
    NUNCHUCK,
    RARE_NUNCHUCK,
    DART,
    DART_2,
    DART_3,
    THROWING_STAR,
    FLAMING_ARROW,
    POISON_ARROW,
    WATER_ARROW,
    ENCHANTED_ARROW,
    RARE_ARROW,
    FLAMING_STEEL_ARROW,
    POISON_STEEL_ARROW,
    WATER_STEEL_ARROW,
    ENCHANTED_STEEL_ARROW,
    RARE_STEEL_ARROW,
    FLAMING_BOLT,
    POISON_BOLT,
    WATER_BOLT,
    ENCHANTED_BOLT,
    RARE_BOLT,
    FLAMING_STEEL_BOLT,
    POISON_STEEL_BOLT,
    WATER_STEEL_BOLT,
    ENCHANTED_STEEL_BOLT,
    RARE_STEEL_BOLT
};

enum EquipTypes {
    HEAD = 1,
    CHEST,
    ARMS,
    LEGS,
    FEET,
    LEFT_HAND,
    RIGHT_HAND
};

class Thing : public WorldObject
{
    int category_;
    int weight_;
    int equip_type_;
    bool is_equippable_;
    bool is_equipped_;
public:
    typedef std::vector<Thing> ThingVector;
    typedef std::vector< std::vector< Thing > > ThingVector2D;
    typedef std::vector<Thing>::iterator ThingIterator;
    typedef std::vector< std::vector< Thing > > ThingIterator2D;
    Thing () {
        category_ = 0;
        weight_ = 5;
        equip_type_ = 0;
        is_passable_ = true;
        is_equippable_ = false;
        is_equipped_ = false;
    }
    int get_category () {
        return category_;
    }
    void set_category ( int category ) {
        category_ = category;
    }
    int get_type () {
        return type_;
    }
    void set_type ( int );
    int get_weight () {
        return weight_;
    }
    void set_weight ( int weight ) {
        weight_ = weight;
    }
    bool get_is_equippable () {
        return is_equippable_;
    }
    void set_is_equippable ( bool is_equippable ) {
        is_equippable_ = is_equippable;
    }
    bool get_is_equipped () {
        return is_equipped_;
    }
    void set_is_equipped ( bool is_equipped ) {
        is_equipped_ = is_equipped;
    }
};

#endif
