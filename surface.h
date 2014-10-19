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

#ifndef INC_SURFACE_H
#define INC_SURFACE_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "rapidxml-1.13/rapidxml.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

class TileSet
{
    private:
        const char *id_;
        const char *path_;
        SDL_Surface *tileset_;
    public:
        typedef std::map<std::string, TileSet> TileSetMap;
        typedef std::map<std::string, TileSet>::iterator TileSetMapIterator;
        TileSet ();
        TileSet( const char* id, const char* path, SDL_Surface *tileset ) {
            id_ = id;
            path_ = path;
            tileset_ = tileset;
        }
        const char* get_id () {
            return id_;
        }
        void set_id ( const char *id ) {
            id_ = id;
        }
        const char* get_path () {
            return path_;
        }
        void set_path ( const char *path ) {
            path_ = path;
        }
        SDL_Surface* get_tileset () {
            return tileset_;
        }
        void set_tileset ( SDL_Surface *tileset ) {
            tileset_ = tileset;
        }
};

class Font
{
    private:
        const char* id_;
        const char* path_;
        int size_;
        TTF_Font* font_;
    public:
        typedef std::map<std::string, Font> FontMap;
        typedef std::map<std::string, Font>::iterator FontMapIterator;
        Font ();
        Font ( const char* id, const char* path, int size, TTF_Font* font ) {
            id_ = id;
            path_ = path;
            size_ = size;
            font_ = font;
        }
        const char* get_id () {
            return id_;
        }
        void set_id ( const char* id ) {
            id_ = id;
        }
        const char* get_path () {
            return path_;
        }
        void set_path ( const char* path ) {
            path_ = path;
        }
        int get_size () {
            return size_;
        }
        void set_size ( int size ) {
            size_ = size;
        }
        TTF_Font* get_font () {
            return font_;
        }
        void set_font ( TTF_Font* font ) {
            font_ = font;
        }
};

class SurfaceManager
{
private:
    TileSet::TileSetMap map_tilesets_;
    Font::FontMap map_fonts_;
    rapidxml::xml_document<>* xml_settings_;
    int screen_bpp_;
    int screen_width_;
    int screen_height_;
    Uint32 screen_flag_;
    int font_line_spacing_;
    SDL_Surface* screen_;
public:
    SurfaceManager () {
        font_line_spacing_ =		4;
    }
    Font::FontMap get_font_map () {
        return map_fonts_;
    }
    void set_xml_settings ( rapidxml::xml_document<>* xml_settings ) {
        xml_settings_ = xml_settings;
    }
    int get_screen_bpp () {
        return screen_bpp_;
    }
    void set_screen_bpp ( int screen_bpp ) {
        screen_bpp_ = screen_bpp;
    }
    int get_screen_width () {
        return screen_width_;
    }
    void set_screen_width ( int screen_width ) {
        screen_width_ = screen_width;
    }
    int get_screen_height () {
        return screen_height_;
    }
    void set_screen_height ( int screen_height ) {
        screen_height_ = screen_height;
    }
    Uint32 get_screen_flag () {
        return screen_flag_;
    }
    void set_screen_flag ( Uint32 screen_flag ) {
        screen_flag_ = screen_flag;
    }
    int get_font_line_spacing () {
        return font_line_spacing_;
    }
    void set_font_line_spacing ( int font_line_spacing ) {
        font_line_spacing_ = font_line_spacing;
    }
    SDL_Surface* get_screen () {
        return screen_;
    }
    void set_screen ( SDL_Surface *screen ) {
        screen_ = screen;
    }
    SDL_Surface* get_tileset ( std::string id ) {
        return map_tilesets_.at(id).get_tileset();
    }
    TTF_Font* get_font ( std::string id ) {
        return map_fonts_.at(id).get_font();
    }
    int ParseSettingToInt (const char *);
    void LoadTileSets ();
    void LoadFonts ();
    void Init ();
    void Quit () {
        for (TileSet::TileSetMapIterator i = map_tilesets_.begin(); i != map_tilesets_.end(); i++) {
            SDL_FreeSurface(i->second.get_tileset());
        }
        printf("All surface manager surfaces released.\n");
    }
};

#endif
