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

#include "objects.h"

void WorldObject::LoadFromVector ( std::vector<const char*> vec )
{
    //id_ = vec[0];
    name_ = vec[2];
    description_ = vec[3];
    tileset_x_ = atoi(vec[6]);
    tileset_y_ = atoi(vec[7]);
    is_passable_ = vec[8];

}

void WorldObject::set_type ( int type )
{
    type_ = type;
}

bool WorldObject::operator== (WorldObject &object)
{
    if (&object == this) return true;
    else {
        return false;
    }
}
