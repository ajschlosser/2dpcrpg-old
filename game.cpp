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

#include "engine.h"

int main( int argc, char **argv )
{
    Engine TileEngine;
    TileEngine.Init();
    TileEngine.CreateWorld();
    Unit unit;
    unit.set_name("Player");
    unit.set_is_player(true);
    unit.set_is_revealed(true);
    unit.set_line_of_sight(5);
    unit.set_coordinates(TileEngine.get_map_width()/2, TileEngine.get_map_height()/2, 0);
    unit.set_weight_limit(1000);
    unit.get_attributes().set_strength(GODLIKE);
    unit.get_attributes().set_defense(GODLIKE);
    unit.get_attributes().set_max_hit_points(200);
    unit.get_attributes().set_hit_points(200);
    Thing thing;
    thing.set_category(WEAPON);
    thing.set_type(SHORTSWORD);
    unit.GiveThing(thing);
    thing.set_category(BLOCK);
    thing.set_type(MUDSTONE);
    unit.GiveThing(thing);
    TileEngine.set_player(unit);
    TileEngine.set_is_paused(false);
    TileEngine.RenderEverything();
    do {
        if (TileEngine.get_is_turn_based()) {
            if (TileEngine.HandlePlayerInput()) {
                TileEngine.RenderEverything();
            } else {
                TileEngine.set_is_paused(true);
                TileEngine.AnimateEverything();
            }
        } else {
            TileEngine.AnimateEverything();
            TileEngine.RenderEverything();
            TileEngine.HandlePlayerInput();
        }
    } while (TileEngine.get_is_running());
    TileEngine.Quit();
    return 0;
}
