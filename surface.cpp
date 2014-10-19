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

#include "surface.h"

int SurfaceManager::ParseSettingToInt (const char * node) {
    std::ifstream xml_file("settings.xml");
    std::vector<char> buffer((std::istreambuf_iterator<char>(xml_file)), std::istreambuf_iterator<char>( ));
    buffer.push_back('\0');
    xml_settings_->parse<0>(&buffer[0]);
    rapidxml::xml_node<> *settings = xml_settings_->first_node("settings");
    rapidxml::xml_node<> *settings_node = settings->first_node(node);
    return atoi(settings_node->value());
}
void SurfaceManager::LoadTileSets () {
    printf("- Loading tilesets...\n");
    std::ifstream xml_file("settings.xml");
    std::vector<char> buffer((std::istreambuf_iterator<char>(xml_file)), std::istreambuf_iterator<char>( ));
    buffer.push_back('\0');
    xml_settings_->parse<0>(&buffer[0]);
    if (xml_settings_ !=NULL) printf(" - Parsing tileset information from settings.xml...\n");
    else printf(" - [ERROR]: Cannot parse settings.xml.\n");
    rapidxml::xml_node<> *settings = xml_settings_->first_node("settings");
    rapidxml::xml_node<> *settings_node = settings->first_node("tilesets");
    for (rapidxml::xml_node<> *tileset_node = settings_node->first_node("tileset"); tileset_node; tileset_node = tileset_node->next_sibling()) {
        std::vector<const char*> tileset_info; // [0] = id, [1] = path
        for (rapidxml::xml_node<> *id = tileset_node->first_node(); id; id = id->next_sibling()) {
            tileset_info.push_back(id->value());
        }
        printf("  * Loading '%s' tileset from '%s'... ", tileset_info[0], tileset_info[1]);
        if (tileset_info.size() > 1) {
            SDL_Surface * temp = IMG_Load(tileset_info[1]);
            TileSet tileset(tileset_info[0], tileset_info[1], SDL_DisplayFormatAlpha(temp));
            if (tileset.get_tileset() == NULL) printf("[ERROR]: Could not load tileset!\n");
            else printf("done!\n");
            map_tilesets_.insert(std::pair<std::string, TileSet>(tileset_info[0], tileset));
        }
    }
    printf("- Done!\n");
}
void SurfaceManager::LoadFonts () {
    printf("- Loading fonts...\n");
    std::ifstream xml_file("settings.xml");
    std::vector<char> buffer((std::istreambuf_iterator<char>(xml_file)), std::istreambuf_iterator<char>( ));
    buffer.push_back('\0');
    xml_settings_->parse<0>(&buffer[0]);
    if (xml_settings_ !=NULL) printf(" - Parsing font information from settings.xml...\n");
    else printf(" - [ERROR]: Cannot parse settings.xml.\n");
    rapidxml::xml_node<> *settings = xml_settings_->first_node("settings");
    rapidxml::xml_node<> *settings_node = settings->first_node("fonts");
    for (rapidxml::xml_node<> *font_node = settings_node->first_node("font"); font_node; font_node = font_node->next_sibling()) {
        std::vector<const char*> font_info; // [0] = id, [1] = path; [2] = size
        for (rapidxml::xml_node<> *id = font_node->first_node(); id; id = id->next_sibling()) {
            font_info.push_back(id->value());
        }
        printf("  * Loading '%s' font (size %d) from '%s'... ", font_info[0], atoi(font_info[2]), font_info[1]);
        if (font_info.size() > 1) {
            TTF_Font* temp = TTF_OpenFont(font_info[1], atoi(font_info[2]));
            Font font(font_info[0], font_info[1], atoi(font_info[2]), temp);
            if (font.get_font() == NULL) printf("\n[ERROR]: Could not load font!\n");
            else printf("done!\n");
            map_fonts_.insert(std::pair<std::string, Font>(font_info[0], font));
        }
    }
    printf("- Done!\n");
}
void SurfaceManager::Init () {
    printf("Initializing surface manager...\n");
    printf("- Setting video mode...\n");
    screen_width_ = ParseSettingToInt("width");
    screen_height_ = ParseSettingToInt("height");
    screen_bpp_ = ParseSettingToInt("bpp");
    screen_flag_ = ParseSettingToInt("flag");
    screen_ = SDL_SetVideoMode(screen_width_, screen_height_, screen_bpp_, screen_flag_);
    if ( screen_ == NULL) printf("   - Error: Could not set video mode. Game window could not be created.\n");
    else {
        printf("   - Video mode set. Game window with dimensions %dx%d created.\n",
               screen_width_, screen_height_);
    }
    LoadTileSets();
    LoadFonts();
}
