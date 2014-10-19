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

#ifndef INC_TILE_H
#define INC_TILE_H
#include "objects.h"
#include "unit.h"

enum TileTypes {
    GRASS = 1,
    ROUGH,
    WATER,
    DIRT,
    FOG,
    UNREVEALED
};

class Unit;

class Tile : public WorldObject
{
public:
    typedef std::vector<Tile> TileVector;
    typedef std::vector< std::vector< Tile > > TileVector2D;
    typedef std::vector<Tile>::iterator TileIterator;
    typedef std::vector< std::vector< Tile > >::iterator TileIterator2D;
    void set_type ( int );
    void ImportDetailsOfObject ( WorldObject );
    Tile () {
        type_ = 0;
        is_passable_ = true;
    }
};

#endif
