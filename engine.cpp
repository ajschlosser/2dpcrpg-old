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

// INITIALIZATION

void Engine::LoadAspects ()
{
    printf("- Loading object properties...\n");
    rapidxml::xml_document<>* xml_settings = new rapidxml::xml_document<>;
    xml_settings = LoadXMLFileToDocument(xml_settings, "settings.xml");
    if (xml_settings !=NULL) printf(" - Parsing object property information from settings.xml...\n");
    else printf(" - [ERROR]: Cannot parse settings.xml.\n");
    rapidxml::xml_node<> *settings = xml_settings->first_node("settings");
    rapidxml::xml_node<> *settings_node = settings->first_node("aspects");
    for (rapidxml::xml_node<> *aspects_node = settings_node->first_node("aspect"); aspects_node; aspects_node = aspects_node->next_sibling()) {
        std::vector<const char*> aspects_info; //0=id;2=category
        for (rapidxml::xml_node<> *id = aspects_node->first_node(); id; id = id->next_sibling()) {
            aspects_info.push_back(id->value());
        }
        printf("  * Loading '%s' object information... \n", aspects_info[0]);
        for (unsigned int i = 0; i < aspects_info.size(); i++) {
            printf("     : %s\n", aspects_info[(int) i]);
        }
        if (aspects_info.size() > 1) {
            Aspects aspects (aspects_info[0], aspects_info[1]);
            if (aspects.get_id() == "") printf("[ERROR]: Could not load object!\n");
            else printf("  * ...done!\n");
            map_aspects_.insert(std::pair<std::string, Aspects>(aspects_info[0], aspects));
        }
    }
    printf("- Done!\n");
}

void Engine::LoadObjects ()
{
    printf("- Loading objects...\n");
    rapidxml::xml_document<>* xml_settings = new rapidxml::xml_document<>;
    xml_settings = LoadXMLFileToDocument(xml_settings, "settings.xml");
    if (xml_settings !=NULL) printf(" - Parsing object information from settings.xml...\n");
    else printf(" - [ERROR]: Cannot parse settings.xml.\n");
    rapidxml::xml_node<> *settings = xml_settings->first_node("settings");
    rapidxml::xml_node<> *settings_node = settings->first_node("objects");
    for (rapidxml::xml_node<> *object_node = settings_node->first_node("object"); object_node; object_node = object_node->next_sibling()) {
        std::vector<const char*> object_info; //0=id;1=class;2=name;3=desc;4=tileset;5=cat;6=x;7=y;8=passable
        std::vector<std::string> object_aspects;
        for (rapidxml::xml_node<> *id = object_node->first_node(); id; id = id->next_sibling()) {
            /**if (strcmp(id->name(), "aspect") == 0)**/ object_info.push_back(id->value());
            //else object_aspects.push_back(id->value());
        }
        printf("  * Loading '%s' object information... \n", object_info[0]);
        for (unsigned int i = 0; i < object_info.size(); i++) {
            printf("     : %s\n", object_info[(int) i]);
        }
        if (object_info.size() > 1) {
            WorldObject object;
            object.LoadFromVector(object_info);
            if (object.get_name() == NULL) printf("[ERROR]: Could not load object!\n");
            else printf("  * ...done!\n");
            map_objects_.insert(std::pair<std::string, WorldObject>(object_info[0], object));
            printf("-----> %s\n", map_objects_.at(object_info[0]).get_name());
        }
    }
    printf("- Done!\n");
}

void Engine::Init ()
{
    //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
    //                     "Missing file",
    //                     "File is missing. Please reinstall the program.",
    //                     NULL);
    printf("\n2dpcrpg alpha\n2012-2013 Aaron J. Schlosser\nMusic by Tom Peter and sauer2\nof OpenGameArt.org\nInitializing game engine.\n");
    SDL_version version;
    SDL_VERSION (&version);
    printf("Initializing SDL compile-time version %u.%u.%u...\n", version.major, version.minor, version.patch);
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) printf("- Error: Could not initialize SDL.\n");
    else {
        version = *SDL_Linked_Version();
        printf("- SDL run-time version %u.%u.%u successfully initialized.\n", version.major, version.minor, version.patch);
    }
    printf("Initializing SDL_ttf library...\n");
    if ( TTF_Init() == -1 ) printf("- Error: Could not initialize SDL_ttf library.\n");
    else {
        printf("- SDL_ttf library successfully initialized.\n");
    }
    SDL_WM_SetCaption( window_title_, NULL );
    SurfaceManager surface_manager;
    surface_manager_ = surface_manager;
    rapidxml::xml_document<> * settings = new rapidxml::xml_document<>;
    settings = LoadXMLFileToDocument(settings, "settings.xml");
    surface_manager_.set_xml_settings(settings);
    surface_manager_.Init();
    if ( SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, 100) == -1) printf("Error: Could not set key repeat delay.\n");
    printf("Initializing SDL_mixer library...\n");
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) printf("- Error: Could not initialize SDL_mixer library.\n");
    else {
        printf("- SDL_mixer library successfully initialized.\n");
    }
    SetMusic("audio/music_world.ogg");
    LoadAspects();
    LoadObjects();
    printf("Creating graphical user interface...\n");
    Interface interface;
    interface.set_screen_dimensions(surface_manager_.get_screen_width(), surface_manager_.get_screen_height());
    interface.set_font_size(16);
    interface.set_font_line_spacing(4);
    interface_ = interface;
    if (interface_.get_screen_height() != surface_manager_.get_screen_height()) printf("- Error creating interface.\n");
    else {
        printf("- Interface with dimensions %dx%d created.\n", interface_.get_screen_width(), interface_.get_screen_height());
    }
    is_running_ = true;
    printf("Done initializing game engine.\n");
}

void Engine::Quit ()
{
    printf("\nStopping game engine.\n");
    int world_objects = 0;
    int world_tiles = 0;
    int world_units = 0;
    int world_things = 0;
    int map_number = 0;
    for (Map::MapIterator m = vec_maps_.begin(); m != vec_maps_.end(); m++) {
        int tiles = (int) m->vec_tiles2d_.size();
        world_objects += tiles;
        world_tiles += tiles;
        m->vec_tiles2d_.clear();
        int units = (int) m->vec_units_.size();
        world_objects += units;
        world_units += units;
        m->vec_units_.clear();
        int things = (int) m->vec_things_.size();
        world_objects += things;
        world_things += things;
        m->vec_things_.clear();
        map_number++;
    }
    printf("%d tiles, %d units, and %d things offloaded from %d maps.\n", world_tiles, world_units, world_things, map_number);
    printf("%d total world objects offloaded.\n", world_objects);
    surface_manager_.Quit();
    Mix_FreeMusic(music_);
    printf("Music released.\n");
    TTF_Quit();
    printf("Shutting down font API.\n");
    SDL_Quit();
    printf("Shutting down SDL.\n");
    printf("\nGame engine stopped.\n\n");
}

// WORLD

void Engine::GenerateRooms ( Map &m, int num_chunks_x, int num_chunks_y ) {
    for (int y = 0; y < num_chunks_y; y++) {
        for (int x = 0; x < num_chunks_x; x++) {
            bool match = false;
            while (!match) {
                int room_x = GetRandomNumber(1 + (chunk_width_ * x), chunk_width_ + (chunk_width_ * x));
                int room_y = GetRandomNumber(1 + (chunk_height_ * y), chunk_height_ + (chunk_height_ * y));
                int room_w = GetRandomNumber(5, chunk_width_ - 1);
                int room_h = GetRandomNumber(5, chunk_height_ - 1);
                if (room_x + room_w < (chunk_width_ + (chunk_width_ * x) - 1) && room_y + room_h < (chunk_height_ + (chunk_height_ * y) - 1)) {
                    match = true;
                    Thing::ThingIterator t = m.vec_things_.begin();
                    while (t != m.vec_things_.end()) {
                        if (t->get_x() > room_x && t->get_x() < room_x + room_w) {
                            if (t->get_y() > room_y && t->get_y() < room_y + room_h) {
                                t = m.vec_things_.erase(t);
                            } else ++t;
                        } else ++t;
                    }
                    Room room (room_x, room_y, room_w, room_h);
                    room.set_is_connected(false);
                    room.set_chunk_coordinates( x, y );
                    room.set_connection_coordinates( GetRandomNumber(room_x, room_x + room_w), GetRandomNumber(room_y, room_y + room_h));
                    m.CreateRoom(room);
                }
            }
        }
    }
}


void Engine::CreateWorld()
{
    sprintf(output_buffer_, "Creating world...");
    PrintSystemMessage(0, 0, output_buffer_);
    printf("\nCreating world. This may take a while depending on your settings...");
    set_map_dimensions( surface_manager_.get_screen_width() * 2 / tile_size_,
                        surface_manager_.get_screen_height() * 2 / tile_size_,
                        2 );
    printf("\n - Planting grass and spilling water...");
    for (int z = 0; z > -map_depth_; z--) {
        Map map ( map_width_, map_height_, z );
        for (int y = 0; y < map_height_; y++) {
            sprintf(output_buffer_, " - Planting grass and spilling water: %.4g percent complete...     ", 100 * ((float) y/map_height_));
            PrintSystemMessage(0, 1, output_buffer_);
            for (int x = 0; x < map_width_; x++) {
                Tile tile;
                tile.set_coordinates( x, y, z );
                if (z == 0) {
                    if (y == 0 || y == map_height_-1) {
                        //tile.set_type ( WATER );
                        tile.ImportDetailsOfObject(map_objects_.at("water"));
                    } else if (x == 0 || x == map_width_-1) {
                        //tile.set_type ( WATER );
                        tile.ImportDetailsOfObject(map_objects_.at("water"));
                    } else {
                        int random = GetRandomNumber(0, 1000);
                        if (random < 900) {
                            tile.set_type ( GRASS );
                        } else if (random > 900 && random < 999) tile.set_type ( DIRT );
                        else tile.ImportDetailsOfObject(map_objects_.at("water")); //tile.set_type ( WATER );
                    }
                } else {
                    tile.set_type( DIRT );
                }
                map.SetTile(x, y, tile);
            }
        }
        vec_maps_.push_back(map);
    }
    sprintf(output_buffer_, " - Planting grass and trees: Done!                              ");
    PrintSystemMessage(0, 1, output_buffer_);
    printf("\n - Placing rocks and stones.");
    int things_total = 0;
    int units_total = 0;
    for (Map::MapIterator m = vec_maps_.begin(); m != vec_maps_.end(); m++) {
        if (m->get_z() == -map_depth_/2 || m->get_z() == -map_depth_/4) printf(".");
        if (m->get_z() < 0) {
            Thing block;
            block.set_category( BLOCK );
            for (int y = 0; y < map_height_; y++) {
                sprintf(output_buffer_, " - Placing rocks and stones: %.4g percent complete...     ", 100 * ((float) y/map_height_));
                PrintSystemMessage(0, 2, output_buffer_);
                for (int x = 0; x < map_width_; x++) {
                    int random = GetRandomNumber(0, 100);
                    if (random > 80) block.set_type(MUDSTONE);
                    else if (random < 80) {
                        if (random > -10 + (m->get_z()*-1)) block.set_type(GREYSTONE);
                        else block.set_type(BLACKSTONE);
                    }
                    block.set_coordinates( x, y, m->get_z() );
                    m->CreateThing(block);
                    things_total++;
                }
            }
        }
    }

    sprintf(output_buffer_, " - Placing rocks and stones: Done!                         ");
    PrintSystemMessage(0, 2, output_buffer_);
    printf("\n - Carving caves, adding baddies.");
    for (Map::MapIterator m = vec_maps_.begin(); m != vec_maps_.end(); m++) {
        GenerateRooms(*m, map_width_/chunk_width_, map_height_/chunk_height_);
        Unit enemy;
        for (int y = 0; y < map_height_; y++) {
            sprintf(output_buffer_, " - Carving caves, growing trees, summoning baddies (level %d): %.4g percent complete...     ", m->get_z(), 100 * ((float) y/map_height_));
            PrintSystemMessage(0, 3, output_buffer_);
            for (int x = 0; x < map_width_; x++) {
                int random = GetRandomNumber(0, 1000);
                if (random > 700 && m->GetTileTypeOfCoords(x, y) == GRASS) {
                    Thing tree;
                    tree.set_coordinates(x, y, m->get_z());
                    tree.set_is_passable(false);
                    tree.set_category(FLORA);
                    tree.set_type(TREE_PINE);
                    m->CreateThing(tree);
                    things_total++;
                    //sprintf(output_buffer_, "  + %d things added.                      ", things_total);
                    //PrintSystemMessage(0, 4, output_buffer_);
                }
                if ( random > (975 - (m->get_z() * -100)) ) {
                    if (!m->CheckIfCoordsOccupiedByThing(x, y) && m->GetTileTypeOfCoords(x, y) != WATER) {
                        enemy.set_coordinates( x, y, m->get_z() );
                        int enemy_type = GetRandomNumber(0, 100);
                        if (enemy_type > 90 - m->get_z()*-10) enemy.set_type(SLIME_RED);
                        else enemy.set_type(SLIME_BLUE);
                        m->CreateUnit(enemy);
                        units_total++;
                        //sprintf(output_buffer_, "  + %d units added.                      ", units_total);
                        //PrintSystemMessage(0, 5, output_buffer_);
                    }
                }
            }
        }
    }

    printf("\nWorld with %d levels of %d tiles each created for a total of %d tiles.\n", map_depth_, map_height_*map_width_,
           map_depth_*map_height_*map_width_);
    printf(" - %d things exist in the world.\n", things_total);
    printf(" - %d units exist in the world.\n\n", units_total);
}

WorldObject& Engine::DetectObject ( Unit &detecting_unit )
{
    unsigned int detected_object = 0;
    int direction_offset[2] = { 0, 0 };
    switch (detecting_unit.get_direction()) {
    case SOUTH: {
        direction_offset[0] = 0;
        direction_offset[1] = 1;
    }
    break;
    case WEST: {
        direction_offset[0] = -1;
        direction_offset[1] = 0;
    }
    break;
    case NORTH: {
        direction_offset[0] = 0;
        direction_offset[1] = -1;
    }
    break;
    case EAST: {
        direction_offset[0] = 1;
        direction_offset[1] = 0;
    }
    break;
    default: {
        printf("Error: Could not detect collision. Detecting unit direction could not be determined.");
        return detecting_unit;
    }
    }
    Map::MapIterator m = GetMapIteratorByZ(detecting_unit.get_z());
    for (detected_object = 0; detected_object < m->vec_units_.size(); detected_object++) {
        if (m->vec_units_.at(detected_object).get_z() == detecting_unit.get_z()) {
            if (m->vec_units_.at(detected_object).get_x() == detecting_unit.get_x() + direction_offset[0] ) {
                if (m->vec_units_.at(detected_object).get_y() == detecting_unit.get_y() + direction_offset[1]) {
                    return m->vec_units_.at(detected_object);
                }
            }
        }
    }
    for (detected_object = 0; detected_object < m->vec_things_.size(); detected_object++) {
        if (m->vec_things_.at(detected_object).get_z() == detecting_unit.get_z()) {
            if (m->vec_things_.at(detected_object).get_x() == detecting_unit.get_x() + direction_offset[0] ) {
                if (m->vec_things_.at(detected_object).get_y() == detecting_unit.get_y() + direction_offset[1]) {
                    return m->vec_things_.at(detected_object);
                }
            }
        }
    }
    return detecting_unit;
}

bool Engine::DetectCollision ( Unit unit )
{
    int z = -player_.get_z();
    int x = unit.get_x();
    int y = unit.get_y();
    Map::MapIterator m = GetMapIteratorByZ(unit.get_z());
    int offset[2] = { 0, 0 };
    switch (unit.get_direction()) {
    case NORTH: {
        offset[0] = 0;
        offset[1] = -1;
        break;
    }
    case EAST: {
        offset[0] = 1;
        offset[1] = 0;
        break;
    }
    case SOUTH: {
        offset[0] = 0;
        offset[1] = 1;
        break;
    }
    case WEST: {
        offset[0] = -1, offset[1] = 0;
        break;
    }
    default: {
        printf("[ERROR]: Could not detect collision. Direction of detecting unit could not be determined.\n");
        return false;
    }
    }
    if (!unit.get_is_player()) {
        if (player_.get_x() == unit.get_x() + offset[0] && player_.get_y() == unit.get_y() + offset[1]) return false;
    }
    if (!vec_maps_[z].vec_tiles2d_[x+offset[0]][y+offset[1]].get_is_passable()) return false;
    for (Thing::ThingIterator th = m->vec_things_.begin(); th != m->vec_things_.end(); th++) {
        if ( (th->get_x() == unit.get_x() + offset[0]) && (th->get_y() == unit.get_y() + offset[1]) ) return th->get_is_passable();
    }
    for (Unit::UnitIterator u = m->vec_units_.begin(); u != m->vec_units_.end(); u++) {
        if ( (u->get_x() == unit.get_x() + offset[0]) && (u->get_y() == unit.get_y() + offset[1]) ) return u->get_is_passable();
    }
    return true;
}

Unit Engine::MoveUnit ( Unit &unit, int direction )
{
    unit.set_previous_coordinates(unit.get_x(), unit.get_y());
    if ( !unit.get_is_player() ) {
        if ( unit.get_direction() == direction ) {
            if (num_turns_ - unit.get_last_turn_acted() > unit.get_attributes().get_base_speed() ) {
                switch (direction) {
                case EAST: {
                    if ((unit.get_x()+1 == player_.get_x()) && (unit.get_y() == player_.get_y()) ) {
                        unit.AttackObject(player_);
                    }
                }
                break;
                case WEST: {
                    if ((unit.get_x()-1 == player_.get_x()) && (unit.get_y() == player_.get_y()) ) {
                        unit.AttackObject(player_);
                    }
                }
                break;
                case SOUTH: {
                    if ((unit.get_y()+1 == player_.get_y()) && (unit.get_x() == player_.get_x()) ) {
                        unit.AttackObject(player_);
                    }
                }
                break;
                case NORTH: {
                    if ((unit.get_y()-1 == player_.get_y()) && (unit.get_x() == player_.get_x()) ) {
                        unit.AttackObject(player_);
                    }
                }
                break;
                }
            }

        } else {
            unit.set_direction(direction);
            return unit;
        }
    }

    if ( unit.get_direction() == direction ) {
        int direction_offset[2] = { 0, 0 };
        switch (unit.get_direction()) {
        case SOUTH: {
            direction_offset[0] = 0;
            direction_offset[1] = 1;
        }
        break;
        case WEST: {
            direction_offset[0] = -1;
            direction_offset[1] = 0;
        }
        break;
        case NORTH: {
            direction_offset[0] = 0;
            direction_offset[1] = -1;
        }
        break;
        case EAST: {
            direction_offset[0] = 1;
            direction_offset[1] = 0;
        }
        break;
        default: {
            printf("Error: Direction of unit '%s' could not be determined.", unit.get_name());
            return unit;
        }
        }
        if ( unit.get_x() + direction_offset[0] < 0 ) return unit;
        else if ( unit.get_y() + direction_offset[1] < 0 ) return unit;
        else if ( unit.get_x() + direction_offset[0] > ((map_width_*tile_size_)-tile_size_)/tile_size_ ) return unit;
        else if ( unit.get_y() + direction_offset[1] > ((map_height_*tile_size_)-tile_size_)/tile_size_ ) return unit;
        if ( DetectObject(unit) == unit ) {
            if ( DetectCollision(unit) ) {
                unit.set_x(unit.get_x() + direction_offset[0]);
                unit.set_y(unit.get_y() + direction_offset[1]);
            }
        } else {
            if (unit.get_is_player()) unit.AttackObject(DetectObject(unit));
        }
    } else unit.set_direction(direction);

    return unit;
}

// GRAPHICS

void Engine::DrawThing ( Thing thing )
{
    SDL_Rect src_offset = { thing.get_tileset_x(), thing.get_tileset_y(), tile_size_, tile_size_ };
    SDL_Rect dst_offset = { thing.get_x()*tile_size_, thing.get_y()*tile_size_, tile_size_, tile_size_ };
    int sw = (surface_manager_.get_screen_width()/tile_size_)/2;
    int sh = (surface_manager_.get_screen_height()/tile_size_)/2;
    int x = thing.get_x();
    int y = thing.get_y();
    int p_x = player_.get_x();
    int p_y = player_.get_y();
    if (x >= (p_x - sw) && x <= (p_x + sw)) {
        if (y >= (p_y - sh) && y <= (p_y + sw)) {
            if (x <= p_x && y <= p_y) {
                dst_offset.x = tile_size_ * (sw - (p_x - x));
                dst_offset.y = tile_size_ * (sh - (p_y - y));
            }
            if (x <= p_x && y >= p_y) {
                dst_offset.x = tile_size_ * (sw - (p_x - x));
                dst_offset.y = tile_size_ * (sh + (y - p_y));
            }
            if (x >= p_x && y >= p_y) {
                dst_offset.x = tile_size_ * (sw + (x - p_x));
                dst_offset.y = tile_size_ * (sh + (y - p_y));
            }
            if (x >= p_x && y <= p_y) {
                dst_offset.x = tile_size_ * (sw + (x - p_x));
                dst_offset.y = tile_size_ * (sh - (p_y - y));
            }
        }
    }
    if ( thing.get_is_revealed() ) {
        switch (thing.get_category()) {
        case FLORA: {
            SDL_BlitSurface( surface_manager_.get_tileset("flora"), &src_offset , surface_manager_.get_screen(), &dst_offset );
        }
        break;
        case WEAPON: {
            SDL_BlitSurface( surface_manager_.get_tileset("weapon"), &src_offset , surface_manager_.get_screen(), &dst_offset );
        }
        break;
        case BLOCK: {
            SDL_BlitSurface( surface_manager_.get_tileset("blocks"), &src_offset , surface_manager_.get_screen(), &dst_offset );
        }
        break;
        default:
            SDL_BlitSurface( surface_manager_.get_tileset("flora"), &src_offset , surface_manager_.get_screen(), &dst_offset );
        }
    }
    dst_offset.w = tile_size_;
    dst_offset.h = tile_size_;
    dst_offset.x = thing.get_x()*tile_size_;
    dst_offset.y = thing.get_y()*tile_size_;

}

void Engine::DrawTile ( Tile tile )
{
    SDL_Rect src_offset = { tile.get_tileset_x(), tile.get_tileset_y(), tile_size_, tile_size_ };
    SDL_Rect dst_offset = { 0, 0, tile_size_, tile_size_ };
    int sw = (surface_manager_.get_screen_width()/tile_size_)/2;
    int sh = (surface_manager_.get_screen_height()/tile_size_)/2;
    int x = tile.get_x();
    int y = tile.get_y();
    int p_x = player_.get_x();
    int p_y = player_.get_y();
    if (tile.get_x() >= (player_.get_x() - sw) && tile.get_x() <= (player_.get_x() + sw)) {
        if (tile.get_y() >= (player_.get_y() - sh) && tile.get_y() <= (player_.get_y() + sw)) {
            if (x <= p_x && y <= p_y) {
                dst_offset.x = tile_size_ * (sw - (p_x - x));
                dst_offset.y = tile_size_ * (sh - (p_y - y));
            }
            if (x <= p_x && y >= p_y) {
                dst_offset.x = tile_size_ * (sw - (p_x - x));
                dst_offset.y = tile_size_ * (sh + (y - p_y));
            }
            if (x >= p_x && y >= p_y) {
                dst_offset.x = tile_size_ * (sw + (x - p_x));
                dst_offset.y = tile_size_ * (sh + (y - p_y));
            }
            if (x >= p_x && y <= p_y) {
                dst_offset.x = tile_size_ * (sw + (x - p_x));
                dst_offset.y = tile_size_ * (sh - (p_y - y));
            }
        }
    }
    if ( !tile.get_is_revealed() ) {
        src_offset.x = 288;
        src_offset.y = 0;
    }
    SDL_BlitSurface( surface_manager_.get_tileset("terrain"), &src_offset , surface_manager_.get_screen(), &dst_offset );
    if ( tile.get_is_under_fog() && tile.get_is_revealed() ) {
        src_offset.x = 288;
        src_offset.y = 32;
        //Map::MapIterator m = GetMapIteratorByZ(tile.get_z());
        //if (!m->CheckIfCoordsOccupiedByThing(tile.get_x(), tile.get_y())) {
        SDL_BlitSurface( surface_manager_.get_tileset("terrain"), &src_offset , surface_manager_.get_screen(), &dst_offset );
        //}
    }
    //SDL_Color white = {255, 255, 255};
    //sprintf(output_buffer_, "(%d,", tile.get_x());
    //DrawText(dst_offset.x, dst_offset.y, output_buffer_, white);
    //sprintf(output_buffer_, "%d)", tile.get_y());
    //DrawText(dst_offset.x, dst_offset.y+surface_manager_.get_font_size()+2, output_buffer_, white);
}

void Engine::DrawUnit ( Unit unit )
{
    SDL_Rect src_offset = { unit.get_tileset_x(), unit.get_tileset_y(), tile_size_, tile_size_ };
    SDL_Rect dst_offset = { unit.get_x()*tile_size_, unit.get_y()*tile_size_, tile_size_, tile_size_ };
    int sw = (surface_manager_.get_screen_width()/tile_size_)/2;
    int sh = (surface_manager_.get_screen_height()/tile_size_)/2;
    int x = unit.get_x();
    int y = unit.get_y();
    int p_x = player_.get_x();
    int p_y = player_.get_y();
    if (x >= (p_x - sw) && x <= (p_x + sw)) {
        if (y >= (p_y - sh) && y <= (p_y + sw)) {
            if (x <= p_x && y <= p_y) {
                dst_offset.x = tile_size_ * (sw - (p_x - x));
                dst_offset.y = tile_size_ * (sh - (p_y - y));
            }
            if (x <= p_x && y >= p_y) {
                dst_offset.x = tile_size_ * (sw - (p_x - x));
                dst_offset.y = tile_size_ * (sh + (y - p_y));
            }
            if (x >= p_x && y >= p_y) {
                dst_offset.x = tile_size_ * (sw + (x - p_x));
                dst_offset.y = tile_size_ * (sh + (y - p_y));
            }
            if (x >= p_x && y <= p_y) {
                dst_offset.x = tile_size_ * (sw + (x - p_x));
                dst_offset.y = tile_size_ * (sh - (p_y - y));
            }
        }
    }
    SDL_Surface *unit_tileset;
    if ( unit.get_is_revealed() ) {
        switch ( unit.get_direction() ) {
        case SOUTH:
            src_offset.x += tile_size_*unit.get_frame();
            src_offset.y += 0;
            break;
        case WEST:
            src_offset.x += tile_size_*unit.get_frame();
            src_offset.y += tile_size_;
            break;
        case NORTH:
            src_offset.x += tile_size_*unit.get_frame();
            src_offset.y += tile_size_*2;
            break;
        case EAST:
            src_offset.x += tile_size_*unit.get_frame();
            src_offset.y += tile_size_*3;
        }
    }
    if ( unit.get_is_player() ) {
        unit_tileset = surface_manager_.get_tileset("player");
        dst_offset.x = surface_manager_.get_screen_width()/2;
        dst_offset.y = surface_manager_.get_screen_height()/2;
    } else unit_tileset = surface_manager_.get_tileset("mobs");
    SDL_BlitSurface( unit_tileset, &src_offset , surface_manager_.get_screen(), &dst_offset );
}

void Engine::DrawWindow ( Window window )
{
    SDL_Color white = { 255, 255, 255 };
    SDL_Color blue = { 0, 0, 255 };
    SDL_Rect border_rect = { window.get_x(), window.get_y(), window.get_w(), window.get_h() };
    SDL_Rect window_rect = { window.get_x()+window.get_default_window_border_size(),
                             window.get_y()+(window.get_default_window_border_size()*2)+16,
                             window.get_w()-(window.get_default_window_border_size()*2),
                             window.get_h()-(window.get_default_window_border_size()*4)-16
                           };
    int border_color = SDL_MapRGB(surface_manager_.get_screen()->format, 255, 255, 255);
    int window_color = SDL_MapRGB(surface_manager_.get_screen()->format, 0, 0, 255);
    SDL_FillRect(surface_manager_.get_screen(), &border_rect, border_color);
    SDL_FillRect(surface_manager_.get_screen(), &window_rect, window_color);
    if (window.get_window_title_text()) {
        DrawText(window.get_x()+window.get_default_window_border_size(),
                 window.get_y()+window.get_default_window_border_size(), window.get_window_title_text(), blue);
    }
    int text_line = 1;
    for (Interface::TextAreaIterator t = window.vec_text_areas_.begin(); t != window.vec_text_areas_.end(); t++) {
        DrawText(window.get_x()+window.get_default_window_border_size()+window.get_default_window_padding(),
                 (window.get_y() + t->get_h()) + (t->get_h()*text_line), t->get_text(), white);
        text_line++;
    }
    int button_column = 0;
    for (Interface::ButtonIterator b = window.vec_buttons_.begin(); b != window.vec_buttons_.end(); b++) {
        DrawText(b->get_x(), b->get_y(), b->get_text(), white);
        button_column += b->get_w() + 20;
    }
    int menu_option = 0;
    for (Interface::MenuIterator m = window.vec_menus_.begin(); m != window.vec_menus_.end(); m++) {
        for ( std::vector<const char*>::iterator opt = m->vec_options_.begin(); opt != m->vec_options_.end(); opt++ ) {
            DrawText(20 + window.get_x()+window.get_default_window_border_size()+window.get_default_window_padding(),
                     window.get_y()+window.get_default_window_padding()+window.get_default_menu_padding()+(16*text_line), m->vec_options_.at(menu_option), white);
            text_line++;
            menu_option++;
        }
        menu_option = 0;
    }
    if (window.vec_window_sprites_.size() > 0) {
        for (Interface::WindowSpriteIterator s = window.vec_window_sprites_.begin(); s != window.vec_window_sprites_.end(); s++) {
            SDL_Rect source_rectangle = s->get_source_rectangle();
            SDL_Rect destination_rectangle = s->get_destination_rectangle();
            switch (s->get_tileset_category()) {
            case FLORA: {
                SDL_BlitSurface( surface_manager_.get_tileset("flora"), &source_rectangle , surface_manager_.get_screen(), &destination_rectangle );
            }
            break;
            case WEAPON: {
                SDL_BlitSurface( surface_manager_.get_tileset("weapon"), &source_rectangle , surface_manager_.get_screen(), &destination_rectangle );
            }
            break;
            case BLOCK: {
                SDL_BlitSurface( surface_manager_.get_tileset("block"), &source_rectangle , surface_manager_.get_screen(), &destination_rectangle );
            }
            break;
            default: {
                SDL_BlitSurface( surface_manager_.get_tileset("flora"), &source_rectangle , surface_manager_.get_screen(), &destination_rectangle );
            }
            break;
            }
        }
    }
}

// INTERFACE

int Engine::DisplayWindow ( Window &window )
{
    printf("Displaying '%s' window with dimensions (%d, %d, %d, %d)... ", window.get_window_title_text(), window.get_x(), window.get_y(),
           window.get_w(), window.get_h());
    DrawWindow(window);
    UpdateScreen();
    int input = 0;
    int selector_color = SDL_MapRGB(surface_manager_.get_screen()->format, 255, 255, 255);
    if (window.vec_buttons_.size() > 0) {
        int num_buttons = (int) window.vec_buttons_.size() - 1;
        int value[num_buttons];
        int button_x[num_buttons];
        int button_y[num_buttons];
        for (unsigned int button = 0; button < window.vec_buttons_.size(); button++) {
            printf("\n   - adding '%s' button option... ", window.vec_buttons_.at(button).get_text());
            value[button] = window.vec_buttons_.at(button).get_value();
            button_x[button] = window.vec_buttons_.at(button).get_x() - window.get_default_window_padding()/2 - 10;
            button_y[button] = window.vec_buttons_.at(button).get_y();
        }
        int choice = 0;
        do {
            DrawWindow(window);
            SDL_Rect selector_rect = { button_x[choice], button_y[choice], 10, 10 };
            SDL_FillRect(surface_manager_.get_screen(), &selector_rect, selector_color);
            switch (GetPlayerInput()) {
            case LEFT: {
                if (choice > 0) choice--;
                else {
                    choice = num_buttons;
                }
            }
            break;
            case RIGHT: {
                if (choice < num_buttons) choice++;
                else {
                    choice = 0;
                }
            }
            break;
            case ENTER: {
                printf("input [%d] received!\n", value[choice]);
                return value[choice];
            }
            break;
            }
            UpdateScreen();
        } while (!input);
    } else if (window.vec_menus_.size() > 0) {
        for (unsigned int menu = 0; menu < window.vec_menus_.size(); menu++) {
            unsigned int choice = 0;
            do {
                DrawWindow(window);
                SDL_Rect selector_rect = { window.vec_menus_.at(menu).get_x() -20, window.vec_menus_.at(menu).get_y()+((choice+1) * 16), 10, 10 };
                SDL_FillRect(surface_manager_.get_screen(), &selector_rect, selector_color);
                switch (GetPlayerInput()) {
                case UP: {
                    if (choice > 0) choice--;
                    else {
                        choice = window.vec_menus_.at(menu).vec_options_.size() - 1;
                    }
                }
                break;
                case DOWN: {
                    if (choice < window.vec_menus_.at(menu).vec_options_.size() - 1) choice++;
                    else {
                        choice = 0;
                    }
                }
                break;
                case ENTER: {
                    printf("input [%d] received!\n", choice );
                    return choice;
                }
                break;
                }
                UpdateScreen();
            } while (input != ESC);
        }
    } else {
        do {
            input = GetPlayerInput();
        } while (input != ESC);
    }
    window.set_is_destroyed(true);
    return 0;
}

int Engine::DisplayUnitInventory ( Unit &unit )
{
    Window window;
    window.set_w(surface_manager_.get_screen_width()-40);
    window.set_h(surface_manager_.get_screen_height()-100);
    window.set_x(20);
    window.set_y(20);
    window.set_window_title_text("Inventory");
    if (unit.vec_things_.size() > 0) {
        int x_offset = 10;
        int y_offset = 1;
        std::vector<int> vec_button_x;
        std::vector<int> vec_button_y;
        unsigned int num_buttons = unit.vec_things_.size() - 1;
        int total_weight_of_things = 0;
        for (unsigned int button = 0; button < unit.vec_things_.size(); button++) {
            total_weight_of_things += unit.vec_things_.at(button).get_weight();
            WindowSprite window_sprite;
            window_sprite.set_source_rectangle(unit.vec_things_.at(button).get_tileset_x(), unit.vec_things_.at(button).get_tileset_y(),
                                               tile_size_, tile_size_);
            window_sprite.set_destination_rectangle
            (window.get_x()+window.get_default_window_padding()+x_offset,
             window.get_y()+window.get_default_window_padding()+16+y_offset,
             tile_size_, tile_size_);
            window_sprite.set_tileset_category(unit.vec_things_.at(button).get_category());
            window.AddWindowSprite(window_sprite);
            vec_button_x.push_back(window.get_x()+x_offset);
            vec_button_y.push_back(window.get_y()+16+y_offset+tile_size_/2);
            x_offset += tile_size_+window.get_default_window_padding()+10;
            if (x_offset > window.get_w()-tile_size_*2) {
                x_offset = 10;
                y_offset += tile_size_+window.get_default_window_padding();
            }
        }
        unsigned int choice = 0;
        int input = 0;
        int selector_color = SDL_MapRGB(surface_manager_.get_screen()->format, 255, 255, 255);
        do {
            DrawWindow(window);
            SDL_Rect selector_rect = { vec_button_x[choice], vec_button_y[choice], 10, 10 };
            SDL_FillRect(surface_manager_.get_screen(), &selector_rect, selector_color);
            SDL_Color white = { 255, 255, 255 };
            DrawText(window.get_x()+16*3, window.get_h()-16*3, unit.vec_things_.at(choice).get_name(), white);
            char description[100] = "";
            strcpy(description, unit.vec_things_.at(choice).get_description());
            if (unit.vec_things_.at(choice).get_is_equipped()) strcat(description, " Equipped.");
            char att[100] = "";
            sprintf(att, " Weight: %d", unit.vec_things_.at(choice).get_weight());
            strcat(description, att);
            DrawText(window.get_x()+16, window.get_h()-16, description, white);
            switch (GetPlayerInput()) {
            case LEFT: {
                if (choice > 0) choice--;
                else {
                    choice = num_buttons;
                }
            }
            break;
            case RIGHT: {
                if (choice < (unsigned) num_buttons) choice++;
                else {
                    choice = 0;
                }
            }
            break;
            case ENTER: {
                Menu menu;
                menu.vec_options_.push_back("EQUIP OR PUT AWAY");
                menu.vec_options_.push_back("DROP");
                menu.vec_options_.push_back("DO NOTHING");
                Window item_window;
                item_window.set_window_title_text(unit.vec_things_.at(choice).get_name());
                item_window.set_w(interface_.get_default_window_width());
                item_window.set_x(vec_button_x[choice]);
                if (item_window.get_w() + item_window.get_x() > window.get_x() + window.get_w()) {
                    item_window.set_x(vec_button_x[choice] - item_window.get_w());
                }
                item_window.set_y(vec_button_y[choice]);
                item_window.AddMenu(menu);
                item_window.AddTextArea("Do what with the item?");
                item_window = interface_.CreateWindow(item_window);
                int menu_choice = DisplayWindow(item_window);
                switch (menu_choice) {
                case 0: {
                    if (unit.vec_things_.at(choice).get_is_equippable()) {
                        if (unit.vec_things_.at(choice).get_is_equipped()) unit.vec_things_.at(choice).set_is_equipped(false);
                        else unit.vec_things_.at(choice).set_is_equipped(true);
                    } else {
                        Window equip_message;
                        equip_message.set_window_title_text("Oops");
                        equip_message.AddTextArea("This item cannot be equipped. Sorry.");
                        equip_message.AddButton(0, "OK");
                        equip_message = interface_.CreateWindow(equip_message);
                        DisplayWindow(equip_message);
                    }
                }
                break;
                case 1: {
                    Window confirm_drop;
                    confirm_drop.set_window_title_text("Drop item");
                    confirm_drop.AddTextArea("Do you really want to drop this item?");
                    confirm_drop.AddButton(0, "NO");
                    confirm_drop.AddButton(1, "YES");
                    confirm_drop = interface_.CreateWindow(confirm_drop);
                    if (DisplayWindow(confirm_drop)) {
                        unit.vec_things_.at(choice).set_coordinates(unit.get_x(), unit.get_y(), unit.get_z());
                        Map::MapIterator m = GetMapIteratorByZ(player_.get_z());
                        m->vec_things_.push_back(unit.vec_things_.at(choice));
                        unit.vec_things_.erase(unit.vec_things_.begin() + choice);
                        return 0;
                    }
                }
                break;
                }
            }
            break;
            case ESC: {
                return 1;
            }
            break;
            }
            UpdateScreen();
        } while (!input);

    } else {
        Window empty_message;
        empty_message.set_window_title_text("Inventory");
        empty_message.AddTextArea("A moth flutters. Empty.");
        empty_message.AddButton(0, "OK");
        empty_message = interface_.CreateWindow(empty_message);
        DisplayWindow(empty_message);
    }
    return 1;
}

int Engine::GetPlayerInput ( )
{
    SDL_Event key_event;
    while (SDL_PollEvent(&key_event)) {
        switch (key_event.type) {
        case SDL_KEYDOWN:
            switch (key_event.key.keysym.sym) {
            case SDLK_DOWN:
                return DOWN;
                break;
            case SDLK_LEFT:
                return LEFT;
                break;
            case SDLK_UP:
                return UP;
                break;
            case SDLK_RIGHT:
                return RIGHT;
                break;
            case SDLK_RETURN:
                return ENTER;
                break;
            case SDLK_SPACE:
                return SPACE;
                break;
            case SDLK_q:
                return GREATER_THAN;
                break;
            case SDLK_w:
                return LESSER_THAN;
                break;
            case SDLK_e:
                return EXAMINE;
                break;
            case SDLK_ESCAPE:
                return ESC;
                break;
            default:
                return 0;
                break;
            }
        }
    }
    return 0;
}

int Engine::HandlePlayerInput ( )
{
    switch (GetPlayerInput()) {
    case DOWN: {
        MoveUnit(player_, SOUTH);
        return 1;
    }
    break;
    case LEFT: {
        MoveUnit(player_, WEST);
        return 1;
    }
    break;
    case UP: {
        MoveUnit(player_, NORTH);
        return 1;
    }
    break;
    case RIGHT: {
        MoveUnit(player_, EAST);
        return 1;
    }
    break;
    case SPACE: {
        int input = 0;
        do {
            input = DisplayUnitInventory(player_);
        } while (!input);
        //player_.set_line_of_sight(100);
        printf("Player has a total of %d things.\n", (int) player_.vec_things_.size());
        return 1;
    }
    break;
    case GREATER_THAN: {
        if (player_.get_z() == 0) SetMusic("audio/music_caves.ogg");
        if (player_.get_z() > (map_depth_-1)*-1 ) player_.set_z(player_.get_z() - 1);
        return 1;
    }
    break;
    case LESSER_THAN: {
        if (player_.get_z() == -1) SetMusic("audio/music_world.ogg");
        if (player_.get_z() < 0) player_.set_z(player_.get_z() + 1);
        DetectObject(player_);
        return 1;
    }
    break;
    case EXAMINE: {
        //ToggleFullscreen();
        if (is_turn_based_) is_turn_based_ = false;
        else is_turn_based_ = true;
        player_.set_line_of_sight(25);
        Window ex_window;
        ex_window.set_window_title_text(vec_maps_[-player_.get_z()].vec_tiles2d_[player_.get_x()][player_.get_y()].get_name());
        ex_window.AddTextArea(vec_maps_[-player_.get_z()].vec_tiles2d_[player_.get_x()][player_.get_y()].get_description());
        ex_window.set_coordinates(0, 0);
        ex_window.AddButton(0, "OK");
        ex_window = interface_.CreateWindow(ex_window);
        DisplayWindow(ex_window);
        return 1;
    }
    break;
    case ESC: {
        Window esc_window;
        esc_window.set_window_title_text("Menu");
        esc_window.AddTextArea("Select an option:");
        esc_window.set_coordinates(0, 0);
        Menu esc_menu;
        esc_menu.vec_options_.push_back("RETURN TO GAME");
        esc_menu.vec_options_.push_back("QUIT");
        esc_window.AddMenu(esc_menu);
        esc_window = interface_.CreateWindow(esc_window);
        switch (DisplayWindow(esc_window)) {
        case 0: {
            return 1;
        }
        break;
        case 1: {
            Window quit_window;
            quit_window.set_window_title_text("Quit");
            quit_window.AddTextArea("Do you really want to quit the game?");
            quit_window.AddTextArea("All progress will be lost. There is");
            quit_window.AddTextArea("currently no means of saving");
            quit_window.AddTextArea("progress in the game.");
            quit_window.AddButton(0, "NO");
            quit_window.AddButton(1, "YES");
            quit_window = interface_.CreateWindow(quit_window);
            int choice = DisplayWindow(quit_window);
            if (choice == 1) is_running_ = false;
            return 1;
        }
        break;
        }
    }
    break;
    default:
        return 0;
    }
    return 0;
}

// PROCESSING

int Engine::RenderTiles ()
{
    bool match_found = false;
    for (Map::MapIterator m = vec_maps_.begin(); m != vec_maps_.end(); m++) {
        for (int y = 0; y < map_height_; y++) {
            for (int x = 0; x < map_width_; x++) {
                if (m->get_z() == player_.get_z()) {
                    match_found = true;
                    if (player_.CheckIfUnitCanSee(x, y)) {
                        m->vec_tiles2d_[x][y].set_is_revealed(true);
                        m->vec_tiles2d_[x][y].set_is_under_fog(false);
                        m->vec_tiles2d_[x][y].reset_time_since_last_seen();
                    }
                    if (m->vec_tiles2d_[x][y].get_is_revealed()) {
                        if (m->vec_tiles2d_[x][y].get_time_since_last_seen() > fog_time_) {
                            /**if (m->vec_tiles2d_[x][y].get_z() < 0)**/ m->vec_tiles2d_[x][y].set_is_under_fog(true);
                        }
                    }
                    if (CheckIfOnScreen(m->vec_tiles2d_[x][y])) DrawTile(m->vec_tiles2d_[x][y]);
                }
            }
        }
    }
    if (!match_found) return 1;
    else return 0;
}

int Engine::RenderThings ()
{
    unsigned int lel = 0;
    Map::MapIterator m = GetMapIteratorByZ((player_.get_z()));
    Thing::ThingIterator t = m->vec_things_.begin();
    while ( t != m->vec_things_.end()) {
        if ( t->get_is_destroyed() ) {
            Thing thing = m->vec_things_.at(lel);
            thing.set_is_destroyed(false);
            player_.GiveThing(thing);
            t = m->vec_things_.erase(t);
        } else {
            if (player_.CheckIfUnitCanSee (t->get_x(), t->get_y())) {
                t->set_is_revealed(true);
                t->reset_time_since_last_seen();
            }
            if (t->get_is_revealed() && CheckIfOnScreen(t)) DrawThing (*t);
            ++t;
        }
        lel++;
    }

    return 0;
}

int Engine::RenderUnits ()
{
    Map::MapIterator m = GetMapIteratorByZ(player_.get_z());
    Unit::UnitIterator i = m->vec_units_.begin();
    while ( i != m->vec_units_.end() ) {
        if ( i->get_is_destroyed() ) {
            i = m->vec_units_.erase(i);
        } else {
            int random_seed = 1 * (int) m->vec_units_.size() * time(NULL);
            if ( i->get_z() == player_.get_z() ) {
                if (i->get_time_since_last_animated() > frame_rate_) {
                    if (!is_paused_) {
                        AnimateUnit(*i);
                        i->reset_time_since_last_animated();
                    }
                }
                if (player_.CheckIfUnitCanSee (i->get_x(), i->get_y())) {
                    i->set_is_revealed(true);
                    i->reset_time_since_last_seen();
                }
                if (i->get_is_revealed()) {
                    if (i->get_z() <= 0) {
                        if (i->get_time_since_last_seen() < fog_time_ && CheckIfOnScreen(i)) DrawUnit (*i);
                    } else {
                        if (CheckIfOnScreen(i)) DrawUnit(*i);
                    }
                }
            }
            if ( (i->get_z() == player_.get_z()) && (!i->get_is_player()) && CheckIfOnScreen(i) ) {
                if (!is_paused_) {
                    if (num_turns_ - i->get_last_turn_acted() > i->get_attributes().get_base_speed() ) {
                        if (i->CheckIfUnitCanSee (player_.get_x(), player_.get_y())) {
                            i->set_target_coordinates(player_.get_x(), player_.get_y());
                            i->set_target_object(player_);
                        } else {
                            int random_x = GetRandomNumber(0, map_width_);
                            int random_y = GetRandomNumber(0, map_height_);
                            int random_direction_chance = GetRandomNumber(0, 100);
                            if (random_direction_chance > 80) i->set_target_coordinates(random_x, random_y);
                        }
                        MoveUnit(*i, i->GetNewDirection(i->get_target_x(), i->get_target_y(), player_.get_direction()));
                        i->set_last_turn_acted(num_turns_);
                    }
                }
            }
            random_seed++;
            ++i;
        }
    }
    DrawUnit(player_);
    if (player_.get_attributes().get_hit_points() <= 0) {
        Window window;
        window.set_window_title_text("Game over");
        window.AddTextArea("You are dead.");
        window.AddButton(0, "OK");
        window = interface_.CreateWindow(window);
        DisplayWindow(window);
        is_running_ = false;
    }
    return 0;
}
