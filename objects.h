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

#ifndef INC_OBJECTS_H
#define INC_OBJECTS_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <vector>
#include <map>
#include <string>
#include "attributes.h"

enum Directions {
    SOUTH=1,
    WEST,
    NORTH,
    EAST
};

class WorldObject
{
protected:
    int x_;
    int y_;
    int z_;
    int previous_x_;
    int previous_y_;
    int type_;
    const char* name_;
    const char* description_;
    int time_since_last_seen_;
    bool is_connected_;
    bool is_passable_;
    bool is_revealed_;
    bool is_under_fog_;
    bool is_destroyed_;
    bool is_damaged_;
    int tileset_x_;
    int tileset_y_;
    int camera_x_;
    int camera_y_;
    Attributes attributes_;
public:
    typedef std::map<std::string, WorldObject> ObjectMap;
    typedef std::map<std::string, WorldObject>::iterator ObjectMapIterator;
    Aspects::AspectsMap map_aspects_;
    WorldObject () {
        x_ = 0;
        y_ = 0;
        z_ = 0;
        type_ = 0;
        name_ = "Unnamed";
        description_ = "No description";
        is_connected_ = false;
        is_passable_ = true;
        is_revealed_ = false;
        is_under_fog_ = true;
        is_destroyed_ = false;
        is_damaged_ = false;
        tileset_x_ = 0;
        tileset_y_ = 0;
        time_since_last_seen_ = SDL_GetTicks();
    }
    void LoadFromVector(std::vector<const char*>);
    WorldObject* ReturnCopy () {
        return this;
    }
    int get_x () {
        return x_;
    }
    void set_x ( int x ) {
        previous_x_ = x_;
        x_ = x;
    }
    int get_y () {
        return y_;
    }
    void set_y ( int y ) {
        previous_y_ = y_;
        y_ = y;
    }
    int get_z () {
        return z_;
    }
    void set_z ( int z ) {
        z_ = z;
    }
    void set_coordinates ( int x, int y, int z ) {
        previous_x_ = x_;
        x_ = x;
        previous_y_ = y_;
        y_ = y;
        z_ = z;
        if (z == 0) {
            set_is_revealed(true);
            set_is_under_fog(false);
        }
    }
    int get_previous_x () {
        return previous_x_;
    }
    void set_previous_x (int previous_x ) {
        previous_x_ = previous_x;
    }
    int get_previous_y () {
        return previous_y_;
    }
    void set_previous_y (int previous_y) {
        previous_y_ = previous_y;
    }
    void set_previous_coordinates ( int previous_x, int previous_y ) {
        previous_x_ = previous_x;
        previous_y_ = previous_y;
    }
    int get_type () {
        return type_;
    }
    void set_type ( int );
    const char* get_name () {
        return name_;
    }
    void set_name ( const char *name ) {
        name_ = name;
    }
    const char* get_description () {
        return description_;
    }
    void set_description ( const char *description ) {
        description_ = description;
    }
    int get_time_since_last_seen () {
        return (SDL_GetTicks() - time_since_last_seen_);
    }
    void reset_time_since_last_seen () {
        time_since_last_seen_ = SDL_GetTicks();
    }
    void set_is_connected ( bool is_connected ) {
        is_connected_ = is_connected;
    }
    bool get_is_connected () {
        return is_connected_;
    }
    void set_is_passable ( bool is_passable ) {
        is_passable_ = is_passable;
    }
    bool get_is_passable () {
        if (map_aspects_.find("unpassable") == map_aspects_.end()) return true;
        else return false;
        //if (map_aspects_.at("unpassable").get_id() == "fart") return false;
        return is_passable_;
    }
    void set_is_revealed ( bool is_revealed ) {
        is_revealed_ = is_revealed;
    }
    bool get_is_revealed () {
        return is_revealed_;
    }
    void set_is_under_fog ( bool is_under_fog ) {
        is_under_fog_ = is_under_fog;
    }
    bool get_is_under_fog () {
        return is_under_fog_;
    }
    void set_is_destroyed ( bool is_destroyed ) {
        is_destroyed_ = is_destroyed;
    }
    bool get_is_destroyed () {
        return is_destroyed_;
    }
    void set_is_damaged ( bool is_damaged ) {
        is_damaged_ = is_damaged;
    }
    bool get_is_damaged () {
        return is_damaged_;
    }
    int get_tileset_x () {
        return tileset_x_;
    }
    int get_tileset_y () {
        return tileset_y_;
    }
    void set_tileset_coordinates ( int x, int y ) {
        tileset_x_ = x;
        tileset_y_ = y;
    }
    int get_camera_x () {
        return camera_x_;
    }
    int get_camera_y () {
        return camera_y_;
    }
    void set_camera_coordinates ( int x, int y ) {
        camera_x_ = x;
        camera_y_ = y;
    }
    Attributes& get_attributes () {
        return attributes_;
    }
    void set_attributes ( Attributes &attributes ) {
        attributes_ = attributes;
    }
    bool operator == (WorldObject&);
};

#endif
