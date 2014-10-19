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

#include "tile.h"

void Tile::ImportDetailsOfObject ( WorldObject object )
{
    name_ = object.get_name();
    description_ = object.get_description();
    set_tileset_coordinates(object.get_tileset_x(), object.get_tileset_y());
    set_is_passable(object.get_is_passable());
}

void Tile::set_type ( int type )
{
    type_ = type;
    switch ( type ) {
    case GRASS:
        name_ = "Grass";
        description_ = "A lush patch of green grass.";
        set_tileset_coordinates(0, 0);
        break;
    case ROUGH:
        set_tileset_coordinates(0, 32);
        break;
    case WATER:
        set_tileset_coordinates(32, 0);
        set_is_passable(false);
        break;
    case DIRT:
        name_ = "Dirt";
        description_ = "A pile of brown dirt.";
        set_tileset_coordinates(64, 0);
        break;
    case FOG:
        set_tileset_coordinates(288, 32);
        break;
    }
};
