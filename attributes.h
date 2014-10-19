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

#ifndef INC_ATTRIBUTES_H
#define INC_ATTRIBUTES_H
#include <string>

enum BaseAttributes {
    POOREST = 1,
    VERY_POOR = 2,
    POOR = 6,
    BELOW_AVERAGE = 12,
    AVERAGE = 18,
    ABOVE_AVERAGE = 26,
    GOOD = 34,
    VERY_GOOD = 42,
    EXCEPTIONAL = 50,
    VERY_EXCEPTIONAL = 60,
    GODLIKE = 75,
    BEST = 99
};

enum BaseSpeeds {
    VERY_MUCH_SLOWER = 10,
    MUCH_SLOWER = 9,
    SLOWER = 8,
    SLOW = 7,
    NORMAL = 6,
    QUICK = 5,
    QUICKER = 4,
    MUCH_QUICKER = 3,
    VERY_MUCH_QUICKER = 2,
    QUICKEST = 0
};

class Aspects
{
    private:
        std::string id_;
        std::string category_;
    public:
        typedef std::map<std::string, Aspects> AspectsMap;
        typedef std::map<std::string, Aspects>::iterator AspectsMapIterator;
        AspectsMap map_aspects_;
        std::string get_id () {
            return id_;
        }
        void set_id ( std::string id ) {
            id_ = id;
        }
        std::string get_category () {
            return category_;
        }
        void set_category ( std::string category ) {
            category_ = category;
        }
        Aspects ( std::string id, std::string category ) {
            id_ = id;
            category_ = category;
        }
};

class Attributes
{
    int strength_;
    int defense_;
    int dexterity_;
    int perception_;
    int max_hit_points_;
    int hit_points_;
    int base_speed_;
    int line_of_sight_;
    int weight_limit_;
public:
    Attributes () {
        strength_ = POOREST;
        defense_ = POOREST;
        dexterity_ = POOREST;
        perception_ = POOREST;
        max_hit_points_ = POOREST + (strength_ * 2);
        hit_points_ = max_hit_points_;
        base_speed_ = NORMAL;
        line_of_sight_ = POOREST;
        weight_limit_ = POOREST + (strength_ * 2);
    };
    void set_line_of_sight ( int line_of_sight ) {
        line_of_sight_ = line_of_sight;
    }
    int get_line_of_sight () {
        return line_of_sight_;
    }
    int get_max_hit_points () {
        return max_hit_points_;
    }
    void set_max_hit_points ( int max_hit_points ) {
        max_hit_points_ = max_hit_points;
    }
    int get_hit_points () {
        return hit_points_;
    }
    void set_hit_points ( int hit_points ) {
        hit_points_ = hit_points;
    }
    int get_strength () {
        return strength_;
    }
    void set_strength ( int strength ) {
        strength_ = strength;
    }
    int get_defense () {
        return defense_;
    }
    void set_defense ( int defense ) {
        defense_ = defense;
    }
    int get_dexterity () {
        return dexterity_;
    }
    void set_dexterity ( int dexterity ) {
        dexterity_ = dexterity;
    }
    int get_perception () {
        return perception_;
    }
    void set_perception ( int perception ) {
        perception_ = perception;
    }
    int get_base_speed () {
        return base_speed_;
    }
    void set_base_speed ( int base_speed ) {
        base_speed_ = base_speed;
    }
    int get_weight_limit () {
        return weight_limit_;
    }
    void set_weight_limit ( int weight_limit ) {
        weight_limit_ = weight_limit;
    }
};

class Status
{

};

#endif
