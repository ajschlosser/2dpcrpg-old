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

#ifndef INC_MAP_H
#define INC_MAP_H
#include <vector>
#include "tile.h"
#include "thing.h"
#include "unit.h"

class Room
{
    int x_;
    int y_;
    int w_;
    int h_;
    bool is_connected_;
    int connection_direction_;
    int connection_coordinates_[2];
    int chunk_coordinates_[2];
public:
    typedef std::vector<Room> RoomVector;
    typedef std::vector<Room>::iterator RoomIterator;
    int get_x() {
        return x_;
    }
    void set_x ( int x ) {
        x_ = x;
    }
    int get_y() {
        return y_;
    }
    void set_y ( int y ) {
        y_ = y;
    }
    int get_w() {
        return w_;
    }
    void set_w ( int w ) {
        w_ = w;
    }
    int get_h() {
        return h_;
    }
    void set_h ( int h ) {
        h_ = h;
    }
    bool get_is_connected () {
        return is_connected_;
    }
    void set_is_connected ( bool is_connected ) {
        is_connected_ = is_connected;
    }
    int get_connection_direction () {
        return connection_direction_;
    }
    void set_connection_direction ( int connection_direction ) {
        connection_direction_ = connection_direction;
    }
    int get_connection_coordinate_x () {
        return connection_coordinates_[0];
    }
    int get_connection_coordinate_y () {
        return connection_coordinates_[1];
    }
    void set_connection_coordinates ( int x, int y ) {
        connection_coordinates_[0] = x;
        connection_coordinates_[1] = y;
    }
    int get_chunk_coordinate_x () {
        return chunk_coordinates_[0];
    }
    int get_chunk_coordinate_y () {
        return chunk_coordinates_[1];
    }
    void set_chunk_coordinates ( int x, int y ) {
        chunk_coordinates_[0] = x;
        chunk_coordinates_[1] = y;
    }
    Room () {}
    Room ( int x, int y, int w, int h ) {
        x_ = x;
        y_ = y;
        w_ = w;
        h_ = h;
    }
};

class Map
{
    int width_;
    int height_;
    int z_;
public:
    Map ( int width, int height, int z ) {
        width_ = width;
        height_ = height;
        z_ = z;
        vec_things_.reserve(height*width);
        vec_tiles2d_.resize(width);
        for(int i = 0; i < width; i++) {
            vec_tiles2d_.at(i).resize(height);
        }
    }
    Map () {};
    typedef std::vector<Map> MapVector;
    typedef std::vector<Map>::iterator MapIterator;
    Thing::ThingVector vec_things_;
    Tile::TileVector2D vec_tiles2d_;
    Unit::UnitVector vec_units_;
    Room::RoomVector vec_rooms_;
    int get_width () {
        return width_;
    }
    void set_width ( int w ) {
        width_ = w;
    }
    int get_height () {
        return height_;
    }
    void set_height ( int h ) {
        height_ = h;
    }
    int get_z () {
        return z_;
    }
    void set_z ( int z ) {
        z_ = z;
    }
    void SetTile ( int x, int y, Tile tile ) {
        vec_tiles2d_[x][y] = tile;
    }
    Tile GetTile ( int x, int y ) {
        return vec_tiles2d_[x][y];
    }
    void CreateThing ( Thing thing ) {
        //printf("Thing created and placed at coordinates (%d, %d, %d).\n",
        //       thing.get_x(), thing.get_y(), thing.get_z());
        vec_things_.push_back(thing);
    }
    void CreateUnit ( Unit unit ) {
        if ( !unit.get_is_player() ) {
            vec_units_.push_back(unit);
            //printf("Unit created and placed at coordinates (%d, %d, %d).\n", unit.get_x(), unit.get_y(), unit.get_z());
        } else {
            printf("Player created.\n");
        }
    }
    void CreateRoom ( Room room ) {
        vec_rooms_.push_back(room);
    }
    bool CheckIfCoordsOccupiedByThing ( int x, int y ) {
        for (Thing::ThingIterator t = vec_things_.begin(); t != vec_things_.end(); t++) {
            if ( (t->get_x() == x) && (t->get_y() == y) ) return true;
        }
        return false;
    }
    bool CheckIfCoordsOccupiedByUnit ( int x, int y ) {
        for (Unit::UnitIterator u = vec_units_.begin(); u != vec_units_.end(); u++) {
            if ( (u->get_x() == x) && (u->get_y() == y) ) return true;
        }
        return false;
    }
    int GetTileTypeOfCoords ( int x, int y ) {
        return vec_tiles2d_[x][y].get_type();
    }
    void SetTileTypeOfCoords ( int x, int y, int type ) {
        vec_tiles2d_[x][y].set_type(type);
    }
    int GetNumberOfNeighboringTilesByType ( int x, int y, int type ) {
        int num_tiles = 0;
        if (vec_tiles2d_[x+1][y].get_type() == type) num_tiles++;
        if (vec_tiles2d_[x-1][y].get_type() == type) num_tiles++;
        if (vec_tiles2d_[x][y+1].get_type() == type) num_tiles++;
        if (vec_tiles2d_[x][y-1].get_type() == type) num_tiles++;
        if (vec_tiles2d_[x+1][y+1].get_type() == type) num_tiles++;
        if (vec_tiles2d_[x+1][y-1].get_type() == type) num_tiles++;
        if (vec_tiles2d_[x-1][y+1].get_type() == type) num_tiles++;
        if (vec_tiles2d_[x-1][y-1].get_type() == type) num_tiles++;
        return num_tiles;
    }
    bool DetectCollision ( Unit unit ) {
        int direction_offset[2] = { 0, 0 };
        switch (unit.get_direction()) {
        case NORTH: {
            direction_offset[0] = 0;
            direction_offset[1] = -1;
            break;
        }
        case EAST: {
            direction_offset[0] = 1;
            direction_offset[1] = 0;
            break;
        }
        case SOUTH: {
            direction_offset[0] = 0;
            direction_offset[1] = 1;
            break;
        }
        case WEST: {
            direction_offset[0] = -1, direction_offset[1] = 0;
            break;
        }
        default: {
            printf("[ERROR]: Could not detect collision. Direction of detecting unit could not be determined.\n");
            return false;
        }
        }
        if (!vec_tiles2d_[unit.get_x()+direction_offset[0]][unit.get_y()+direction_offset[1]].get_is_passable()) return false;

        for (Thing::ThingIterator th = vec_things_.begin(); th != vec_things_.end(); th++) {
            if ( (th->get_x() == unit.get_x() + direction_offset[0]) && (th->get_y() == unit.get_y() + direction_offset[1]) ) return th->get_is_passable();
        }
        for (Unit::UnitIterator u = vec_units_.begin(); u != vec_units_.end(); u++) {
            if ( (u->get_x() == unit.get_x() + direction_offset[0]) && (u->get_y() == unit.get_y() + direction_offset[1]) ) return u->get_is_passable();
        }
    }
};

#endif
