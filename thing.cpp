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

#include "thing.h"

void Thing::set_type ( int type )
{
    type_ = type;
    if (category_ == FLORA) {
        switch ( type ) {
        case TREE_PINE: {
            set_name( "Pine Tree" );
            set_description( "A sweet-smelling conifer.");
            is_passable_ = false;
            set_tileset_coordinates (0, 0);
            attributes_.set_hit_points(15);
        }
        break;
        }
    } else if (category_ == WEAPON) {
        set_is_equippable(true);
        switch (type) {
        case BROKEN_DAGGER: {
            set_tileset_coordinates (0, 0);
            set_name( "Broken Dagger" );
            description_ = "A broken dagger. It can still cut.";
        }
        break;
        case BROKEN_SHORTSWORD: {
            set_tileset_coordinates (32, 0);
        }
        break;
        case BROKEN_BROADSWORD: {
            set_tileset_coordinates (64, 0);
        }
        break;
        case SHORTSWORD: {
            set_tileset_coordinates (96, 0);
            name_ = "Short Sword";
            description_ = "A common short sword made from steel.";
        }
        break;
        }
    } else if (category_ == BLOCK) {
        set_is_equippable(false);
        switch ( type ) {
        case MUDSTONE: {
            set_name( "Mudstone" );
            set_description( "A common mud-colored rock.");
            is_passable_ = false;
            set_tileset_coordinates (0, 0);
            attributes_.set_hit_points(10);
        }
        break;
        case GREYSTONE: {
            set_name( "Greystone" );
            set_description( "A sturdy stone good for building.");
            is_passable_ = false;
            set_tileset_coordinates (32, 0);
            attributes_.set_hit_points(25);
        }
        break;
        case SANDSTONE: {
            set_name( "Sandstone" );
            is_passable_ = false;
            set_tileset_coordinates (64, 0);
            attributes_.set_hit_points(5);
        }
        break;
        case BLACKSTONE: {
            set_name( "Blackstone" );
            set_description( "An extremely hard stone made from carbon and ash.");
            is_passable_ = false;
            set_tileset_coordinates (96, 0);
            attributes_.set_hit_points(50);
        }
        break;
        }

    }

}
