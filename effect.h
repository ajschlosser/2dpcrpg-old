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

#ifndef INC_EFFECT_H
#define INC_EFFECT_H
#include <vector>

class Effect : public WorldObject
{
    int direction_;
    int frame_;
    int line_of_sight_;
    int time_since_last_animated_;
public:
    typedef std::vector<Effect> EffectVector;
    typedef std::vector<Effect>::iterator EffectIterator;
    int get_direction () {
        return direction_;
    }
    void set_direction ( int direction ) {
        direction_ = direction;
    }
    int get_frame () {
        return frame_;
    }
    int get_time_since_last_animated () {
        return (SDL_GetTicks() - time_since_last_animated_);
    }
    void reset_time_since_last_animated () {
        time_since_last_animated_ = SDL_GetTicks();
    }
};

#endif
