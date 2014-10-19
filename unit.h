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

#ifndef INC_UNIT_H
#define INC_UNIT_H

#include <vector>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "objects.h"
#include "thing.h"

enum UnitCategory {
    LESSER = 1
};

enum UnitTypes {
    SLIME_BLUE = 1,
    SLIME_RED
};

class Unit : public WorldObject
{
    int direction_;
    int frame_;
    int line_of_sight_;
    int time_since_last_animated_;
    int hit_points_;
    int strength_;
    int defense_;
    int action_points_;
    int weight_limit_;
    int inventory_limit_;
    int last_turn_acted_;
    int target_coordinates_[2];
    bool is_player_;
    WorldObject target_object_;
public:
    Unit () {
        Attributes attributes;
        attributes_ = attributes;
        is_player_ = false;
        frame_ = 0;
        direction_ = EAST;
        line_of_sight_ = 3;
        hit_points_ = 100;
        strength_ = 10;
        defense_ = 1;
        action_points_ = 100;
        weight_limit_ = 100;
        last_turn_acted_ = 0;
        target_coordinates_[0] = 0;
        target_coordinates_[1] = 0;
        time_since_last_animated_ = SDL_GetTicks();
    }
    typedef std::vector<Unit> UnitVector;
    typedef std::vector<Unit>::iterator UnitIterator;
    Thing::ThingVector vec_things_;
    int get_type () {
        return type_;
    }
    void set_type ( int );
    int get_direction () {
        return direction_;
    }
    void set_direction ( int direction ) {
        direction_ = direction;
    }
    int get_frame () {
        return frame_;
    }
    void set_line_of_sight ( int line_of_sight ) {
        line_of_sight_ = line_of_sight;
    }
    int get_line_of_sight () {
        return line_of_sight_;
    }
    int get_time_since_last_animated () {
        return (SDL_GetTicks() - time_since_last_animated_);
    }
    void reset_time_since_last_animated () {
        time_since_last_animated_ = SDL_GetTicks();
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
    int get_action_points () {
        return action_points_;
    }
    void set_action_points ( int action_points ) {
        action_points_ = action_points;
    }
    int get_weight_limit () {
        return weight_limit_;
    }
    void set_weight_limit ( int weight_limit ) {
        weight_limit_ = weight_limit;
    }
    void set_last_turn_acted ( int last_turn_acted ) {
        last_turn_acted_ = last_turn_acted;
    }
    int get_last_turn_acted () {
        return last_turn_acted_;
    }
    void set_target_coordinates ( int target_x, int target_y ) {
        target_coordinates_[0] = target_x;
        target_coordinates_[1] = target_y;
    }
    int get_target_x () {
        return target_coordinates_[0];
    }
    int get_target_y () {
        return target_coordinates_[1];
    }
    bool get_is_player () {
        return is_player_;
    }
    void set_is_player ( bool is_player ) {
        is_player_ = is_player;
    }
    void set_frame ( int frame ) {
        frame_ = frame;
    }
    void GiveThing ( Thing thing ) {
        int total_weight = 0;
        for (Thing::ThingIterator th = vec_things_.begin(); th != vec_things_.end(); th++) {
            total_weight += th->get_weight();
        }
        if (thing.get_weight() + total_weight < weight_limit_) vec_things_.push_back(thing);
    }
    int GetNewDirection ( int, int, int );
    void AttackObject ( WorldObject &target_object ) {
        int damage = 0;
        if ( (attributes_.get_strength() / target_object.get_attributes().get_defense()) < 1) damage = 0;
        else damage = (attributes_.get_strength() / target_object.get_attributes().get_defense());
        target_object.get_attributes().set_hit_points( target_object.get_attributes().get_hit_points() - damage  );
        if (target_object.get_attributes().get_hit_points() <= 0) {
            target_object.set_is_destroyed(true);
        }
        printf( "%s attacking %s for %d damage. %s's HP reduced to %d.\n",
                name_, target_object.get_name(), damage, target_object.get_name(), target_object.get_attributes().get_hit_points());
    }
    void set_target_object ( WorldObject target_object ) {
        target_object_ = target_object;
    }
    WorldObject get_target_object () {
        return target_object_;
    }
    bool CheckIfUnitCanSee ( int seen_x, int seen_y ) {
        if ( (seen_x < x_ + line_of_sight_) && (seen_x > x_ - line_of_sight_)) {
            if ( (seen_y < y_ + line_of_sight_) && (seen_y > y_ - line_of_sight_)) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
};

#endif
