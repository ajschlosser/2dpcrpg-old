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

#ifndef INC_ENGINE_H
#define INC_ENGINE_H

//#include "SDL2/SDL.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "time.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include "rapidxml-1.13/rapidxml.hpp"
#include "surface.h"
#include "unit.h"
#include "tile.h"
#include "thing.h"
#include "interface.h"
#include "effect.h"
#include "map.h"

enum Keys {
    DOWN = 1,
    LEFT,
    UP,
    RIGHT,
    ENTER,
    SPACE,
    GREATER_THAN,
    LESSER_THAN,
    EXAMINE,
    ESC
};

class Unit;

class Engine
{
private:
    const char *window_title_;
    int frame_rate_;
    int tile_size_;
    int fog_time_;
    int map_width_;
    int map_height_;
    int map_depth_;
    int chunk_width_;
    int chunk_height_;
    int num_turns_;
    bool is_turn_based_;
    bool is_paused_;
    bool is_running_;
    bool is_debugging_;
    char output_buffer_[1000];
    Mix_Music *music_;
    SurfaceManager surface_manager_;
    Interface interface_;
    Unit player_;
    Map::MapIterator map_iterator_;
    long oldseed_;
public:
    Engine () {
        window_title_ =             "2dpcrpg Alpha";
        frame_rate_ =				284;
        tile_size_ =				32;
        fog_time_ =					1;
        num_turns_ =                0;
        is_turn_based_ =            true;
        is_running_ =               false;
        is_paused_ =                true;
        is_debugging_ =             false;
    }
    Map::MapVector vec_maps_;
    Aspects::AspectsMap map_aspects_;
    WorldObject::ObjectMap map_objects_;
    int get_frame_rate () {
        return frame_rate_;
    }
    int get_tile_size () {
        return tile_size_;
    }
    int get_map_width () {
        return map_width_;
    }
    void set_map_width ( int map_width ) {
        map_width_ = map_width;
    }
    int get_map_height () {
        return map_height_;
    }
    void set_map_height ( int map_height ) {
        map_height_ = map_height;
    }
    int get_map_depth () {
        return map_depth_;
    }
    void set_map_depth ( int map_depth ) {
        map_depth_ = map_depth;
    }
    int get_chunk_width () {
        return chunk_width_;
    }
    void set_chunk_width ( int w ) {
        chunk_width_ = w;
    }
    int get_chunk_height () {
        return chunk_height_;
    }
    void set_chunk_height ( int h ) {
        chunk_height_ = h;
    }
    void set_map_dimensions ( int map_width, int map_height, int map_depth ) {
        map_width_ = map_width;
        map_height_ = map_height;
        map_depth_ = map_depth;
        chunk_width_ = 16;
        chunk_height_ = 10;
    }
    bool get_is_turn_based () {
        return is_turn_based_;
    }
    void set_is_turn_based ( bool is_turn_based ) {
        is_turn_based_ = is_turn_based;
    }
    bool get_is_paused () {
        return is_paused_;
    }
    void set_is_paused ( bool is_paused ) {
        is_paused_ = is_paused;
    }
    bool get_is_running() {
        return is_running_;
    }
    void set_interface ( Interface interface ) {
        interface_ = interface;
    }
    void set_player ( Unit player ) {
        player_ = player;
    }
    Unit get_player () {
        return player_;
    }
    void set_map_iterator ( Map::MapIterator m ) {
        map_iterator_ = m;
    }
    Map::MapIterator get_map_iterator () {
        return map_iterator_;
    }
    void LoadAspects ();
    void LoadObjects ();
    // Video
    void Init ();
    void Quit ();
    void ToggleMusic () {
        //Mix_PlayMusic(music_, -1);
    }
    int SetMusic ( const char *music ) {
        if (!(music_ = Mix_LoadMUS( music ))) printf("- Error: Could not load music.\n");
        else {
            printf("- Music loaded.\n");
        }
        ToggleMusic();
        printf("  - Playing: %s\n", music);
        return 1;
    }
    void ToggleFullscreen () {
        if ( surface_manager_.get_screen_flag() == SDL_FULLSCREEN ) {
            surface_manager_.set_screen_flag(SDL_HWSURFACE);
            printf("Disabling fullscreen and enabling windowed mode.\n");
        } else {
            surface_manager_.set_screen_flag(SDL_FULLSCREEN);
            printf("Enabling fullscreen mode.\n");
        }
        surface_manager_.set_screen(SDL_SetVideoMode( surface_manager_.get_screen_width(),
                                    surface_manager_.get_screen_height(),
                                    surface_manager_.get_screen_bpp(),
                                    surface_manager_.get_screen_flag() ));
    }
    // World
    void GenerateRooms ( Map&, int, int );
    void CreateWorld ();
    WorldObject& DetectObject ( Unit& );
    bool DetectCollision ( Unit );
    Unit MoveUnit ( Unit&, int );
    Map::MapIterator GetMapIteratorByZ ( int z ) {
        for (Map::MapIterator m = vec_maps_.begin(); m != vec_maps_.end(); m++) {
            if (m->get_z() == z) {
                return m;
            }
        }
        printf("Error: Could not find match between specified z-level and map.\n");
        return vec_maps_.end();
    }
    // Graphics
    void DrawText ( int x, int y, const char* text, SDL_Color foreground ) {
        SDL_Surface * text_surface = TTF_RenderUTF8_Solid(surface_manager_.get_font("standard"), text, foreground);
        SDL_Rect destination;
        destination.w = 0;
        destination.h = 0;
        destination.x = x;
        destination.y = y;
        SDL_BlitSurface( text_surface, NULL, surface_manager_.get_screen(), &destination );
        SDL_FreeSurface( text_surface );
        text_surface = NULL;
        text = NULL;
    }
    void DrawText ( int x, int y, const char* text, SDL_Color foreground, SDL_Color background ) {
        SDL_Surface * text_surface = TTF_RenderUTF8_Shaded(surface_manager_.get_font("standard"), text, foreground, background);
        SDL_Rect destination;
        destination.w = 0;
        destination.h = 0;
        destination.x = x;
        destination.y = y;
        SDL_BlitSurface( text_surface, NULL, surface_manager_.get_screen(), &destination );
        SDL_FreeSurface( text_surface );
        text_surface = NULL;
        text = NULL;
    }
    void DrawThing ( Thing );
    void DrawTile ( Tile );
    void DrawUnit ( Unit );
    void DrawWindow ( Window );
    void AnimateUnit ( Unit &unit ) {
        if (unit.get_frame() < 1) {
            unit.set_frame(unit.get_frame() + 1);
        } else {
            unit.set_frame(0);
        }
    }
    void UpdateScreen () {
        SDL_Flip(surface_manager_.get_screen());
    }
    // Interface
    int DisplayWindow ( Window& );
    int DisplayUnitInventory ( Unit& );
    int GetPlayerInput();
    int HandlePlayerInput ();
    void PrintSystemMessage ( int x, int line, const char * message ) {
        SDL_Color black = { 0, 0, 0 }, white = { 255, 255, 255 };
        DrawText(x, 16*line, output_buffer_, white, black);
        UpdateScreen();
    }
    // Processing
    rapidxml::xml_document<>* LoadXMLFileToDocument (rapidxml::xml_document<> * doc, const char * file_name) {
        std::ifstream xml_file(file_name);
        std::vector<char> buffer((std::istreambuf_iterator<char>(xml_file)), std::istreambuf_iterator<char>( ));
        buffer.push_back('\0');
        doc->parse<0>(&buffer[0]);
        return doc;
    }
    bool CheckIfOnScreen ( Tile::TileIterator t ) {
        if ( t->get_x() > player_.get_x() - ((surface_manager_.get_screen_width() / tile_size_) / 2) - 1 &&
                t->get_x() < player_.get_x() + ((surface_manager_.get_screen_width() / tile_size_) / 2) + 1 ) {
            if ( t->get_y() > player_.get_y() - ((surface_manager_.get_screen_height() / tile_size_) / 2) - 1 &&
                    t->get_y() < player_.get_y() + ((surface_manager_.get_screen_height() / tile_size_) / 2) + 1 ) {
                return true;
            }
        }
        return false;
    }
    bool CheckIfOnScreen ( Tile tile ) {
        if ( tile.get_x() > player_.get_x() - ((surface_manager_.get_screen_width() / tile_size_) / 2) - 1 &&
                tile.get_x() < player_.get_x() + ((surface_manager_.get_screen_width() / tile_size_) / 2) + 1 ) {
            if ( tile.get_y() > player_.get_y() - ((surface_manager_.get_screen_height() / tile_size_) / 2) - 1 &&
                    tile.get_y() < player_.get_y() + ((surface_manager_.get_screen_height() / tile_size_) / 2) + 1 ) {
                return true;
            }
        }
        return false;
    }
    bool CheckIfOnScreen ( Thing::ThingIterator t ) {
        if ( t->get_x() > player_.get_x() - ((surface_manager_.get_screen_width() / tile_size_) / 2) - 1 &&
                t->get_x() < player_.get_x() + ((surface_manager_.get_screen_width() / tile_size_) / 2) + 1 ) {
            if ( t->get_y() > player_.get_y() - ((surface_manager_.get_screen_height() / tile_size_) / 2) - 1 &&
                    t->get_y() < player_.get_y() + ((surface_manager_.get_screen_height() / tile_size_) / 2) + 1 ) {
                return true;
            }
        }
        return false;
    }
    bool CheckIfOnScreen ( Unit::UnitIterator u ) {
        if ( u->get_x() > player_.get_x() - ((surface_manager_.get_screen_width() / tile_size_) / 2) - 1 &&
                u->get_x() < player_.get_x() + ((surface_manager_.get_screen_width() / tile_size_) / 2) + 1 ) {
            if ( u->get_y() > player_.get_y() - ((surface_manager_.get_screen_height() / tile_size_) / 2) - 1 &&
                    u->get_y() < player_.get_y() + ((surface_manager_.get_screen_height() / tile_size_) / 2) + 1 ) {
                return true;
            }
        }
        return false;
    }
    int GetRandomNumber ( int min, int max ) {
        time_t seed;
        seed = time(NULL) + oldseed_;
        oldseed_ = seed;

        srand(seed);

        int n = max - min + 1;
        int i = rand() % n;

        if(i < 0)
            i = -i;

        return min + i;
    }
    int RenderWindows ();
    int RenderTiles ();
    int RenderThings ();
    int RenderUnits ();
    void AnimateEverything () {
        RenderEverything();
        Map::MapIterator m = GetMapIteratorByZ(player_.get_z());
        Unit::UnitIterator i = m->vec_units_.begin();
        while ( i != m->vec_units_.end() ) {
            if ( i->get_z() == player_.get_z() ) {
                if (i->get_time_since_last_animated() > frame_rate_) {
                    AnimateUnit(*i);
                    i->reset_time_since_last_animated();
                }
                if (player_.CheckIfUnitCanSee (i->get_x(), i->get_y())) {
                    i->set_is_revealed(true);
                    i->reset_time_since_last_seen();
                }
                if (i->get_is_revealed()) {
                    if (i->get_z() < 0) {
                        if (i->get_time_since_last_seen() < fog_time_) DrawUnit (*i);
                    } else {
                        DrawUnit(*i);
                    }
                }
            }
            ++i;
        }
        if (player_.get_time_since_last_animated() > frame_rate_) {
            AnimateUnit(player_);
            DrawUnit(player_);
            player_.reset_time_since_last_animated();
        }
    }
    void RenderEverything () {
        if (RenderTiles()) printf("Error rendering the map and/or tiles.\n");
        RenderThings ();
        RenderUnits ();
        sprintf(output_buffer_, "Turn %d. Player coordinates are (%d, %d, %d). HP: %d. ST: %d. DF: %d. DX: %d.",
                num_turns_, player_.get_x(), player_.get_y(), player_.get_z(), player_.get_attributes().get_hit_points(),
                player_.get_attributes().get_strength(), player_.get_attributes().get_defense(),
                player_.get_attributes().get_dexterity() );
        SDL_Color white = { 255, 255, 255 };
        DrawText(0, 0, output_buffer_, white);
        if (!is_turn_based_) DrawText(0, 16+surface_manager_.get_font_line_spacing(), "Free play mode.", white);
        else DrawText(0, 16+surface_manager_.get_font_line_spacing(), "Tactical mode.", white);
        DrawText(0, (surface_manager_.get_screen_height()-16), "2dpcrpg Alpha, 2012", white);
        UpdateScreen();
        if (!is_paused_) num_turns_++;
        is_paused_ = false;
    }
};

#endif
