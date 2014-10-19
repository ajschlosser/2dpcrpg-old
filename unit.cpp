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

#include "unit.h"

void Unit::set_type ( int type )
{
    type_ = type;
    switch (type) {
    case SLIME_BLUE:
        set_name("Blue Slime");
        attributes_.set_hit_points(10);
        set_tileset_coordinates(0, 0);
        break;
    case SLIME_RED:
        set_name("Red Slime");
        attributes_.set_hit_points(20);
        attributes_.set_strength(GODLIKE);
        attributes_.set_line_of_sight(GODLIKE);
        set_tileset_coordinates(64, 0);
        break;
    }
}

int Unit::GetNewDirection ( int target_x, int target_y, int target_direction )
{

    if ( x_ > target_x ) return WEST;
    else if ( x_ < target_x ) return EAST;
    else if ( y_ > target_y ) return NORTH;
    else if ( y_ < target_y ) return SOUTH;
    else return target_direction;

}
