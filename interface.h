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

#ifndef INC_INTERFACE_H
#define INC_INTERFACE_H

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include <vector>

enum ButtonValues {
    QUIT = 1,
    CANCEL,
    YES,
    NO,
    OK,
    CONTINUE,
    OPEN
};

class TextArea;
class Button;
class Menu;
class WindowSprite;
class Window;

class Interface
{
protected:
    int font_size_;
    int font_line_spacing_;
    int screen_width_;
    int screen_height_;
    int default_text_area_padding_;
    int default_button_padding_;
    int default_menu_padding_;
    int default_window_width_;
    int default_window_height_;
    int default_window_border_size_;
    int default_window_padding_;
public:
    Interface () {
        default_text_area_padding_ = 		1;
        default_button_padding_ = 			5;
        default_menu_padding_ =             5;
        default_window_width_ = 			300;
        default_window_height_ =			200;
        default_window_border_size_ =		3;
        default_window_padding_ =			10;
    }
    typedef std::vector<TextArea> TextAreaVector;
    typedef std::vector<TextArea>::iterator TextAreaIterator;
    typedef std::vector<Button> ButtonVector;
    typedef std::vector<Button>::iterator ButtonIterator;
    typedef std::vector<Menu> MenuVector;
    typedef std::vector<Menu>::iterator MenuIterator;
    typedef std::vector<WindowSprite> WindowSpriteVector;
    typedef std::vector<WindowSprite>::iterator WindowSpriteIterator;
    typedef std::vector<Window> WindowVector;
    typedef std::vector<Window>::iterator WindowIterator;
    int get_font_size () {
        return font_size_;
    }
    void set_font_size ( int font_size ) {
        font_size_ = font_size;
    }
    int get_font_line_spacing () {
        return font_line_spacing_;
    }
    void set_font_line_spacing ( int font_line_spacing ) {
        font_line_spacing_ = font_line_spacing;
    }
    void set_screen_dimensions ( int width, int height ) {
        screen_width_ = width;
        screen_height_ = height;
    }
    int get_screen_width () {
        return screen_width_;
    }
    int get_screen_height () {
        return screen_height_;
    }
    int get_default_window_width() {
        return default_window_width_;
    }
    int get_default_window_height() {
        return default_window_height_;
    }
    int get_default_window_border_size() {
        return default_window_border_size_;
    }
    int get_default_window_padding() {
        return default_window_padding_;
    }
    int get_default_button_padding() {
        return default_button_padding_;
    }
    int get_default_menu_padding() {
        return default_menu_padding_;
    }
    int get_default_text_area_padding() {
        return default_text_area_padding_;
    }
    Window CreateWindow ( Window );
};

class Widget : public Interface
{
protected:
    int x_;
    int y_;
    int w_;
    int h_;
    int id_;
    const char* name_;
public:
    Widget () {
        x_ = 0;
        y_ = 0;
        w_ = 0;
        h_ = 0;
        id_ = 0;
    }
    void set_dimensions ( int x, int y, int w, int h ) {
        x_ = x;
        y_ = y;
        w_ = w;
        h_ = h;
    }
    void set_coordinates ( int x, int y ) {
        x_ = x;
        y_ = y;
    }
    void set_x ( int x ) {
        x_ = x;
    }
    int get_x () {
        return x_;
    }
    void set_y ( int y ) {
        y_ = y;
    }
    int get_y () {
        return y_;
    }
    void set_w ( int w ) {
        w_ = w;
    }
    int get_w () {
        return w_;
    }
    void set_h ( int h ) {
        h_ = h;
    }
    int get_h () {
        return h_;
    }
    int get_id () {
        return id_;
    }
    void set_id ( int id ) {
        id_ = id;
    }
    void set_name ( const char *name ) {
        name_ = name;
    }
    const char* get_name () {
        return name_;
    }
};

class Button : public Widget
{
    const char *text_;
    int value_;
public:
    Button () {
        text_ = "";
    }
    void set_text ( const char *text ) {
        text_ = text;
    }
    const char* get_text () {
        return text_;
    }
    void set_value ( int value ) {
        value_ = value;
    }
    int get_value () {
        return value_;
    }
};

class TextArea : public Widget
{
    const char *text_;
public:
    TextArea() {
        text_ = "";
    }
    void set_text ( const char *text ) {
        text_ = text;
    }
    const char* get_text () {
        return text_;
    }
};

class Menu : public Widget
{
    int type_;
    int item_width_;
    int item_height_;
public:
    Menu () {
        type_ = 0;
    }
    std::vector<const char*> vec_options_;
    int get_type () {
        return type_;
    }
    void set_type ( int type ) {
        type_ = type;
    }
    int get_item_width () {
        return item_width_;
    }
    void set_item_width ( int item_width ) {
        item_width_ = item_width;
    }
    int get_item_height () {
        return item_height_;
    }
    void set_item_height ( int item_height ) {
        item_height_ = item_height;
    }
};

class WindowSprite : public Widget
{
    int tileset_category_;
    SDL_Rect source_;
    SDL_Rect destination_;
    SDL_Surface *tileset_;
public:
    WindowSprite () {};
    int get_tileset_category () {
        return tileset_category_;
    }
    void set_tileset_category ( int tileset_category ) {
        tileset_category_ = tileset_category;
    }
    void set_source_rectangle ( int x, int y, int w, int h) {
        source_.x = x;
        source_.y = y;
        source_.w = w;
        source_.h = h;
    }
    void set_destination_rectangle ( int x, int y, int w, int h) {
        destination_.x = x;
        destination_.y = y;
        destination_.w = w;
        destination_.h = h;
    }
    void set_tileset ( SDL_Surface *tileset) {
        tileset_ = tileset;
    }
    SDL_Rect get_source_rectangle () {
        return source_;
    }
    SDL_Rect get_destination_rectangle () {
        return destination_;
    }
    SDL_Surface* get_tileset () {
        return tileset_;
    }
};

class Window : public Widget
{
    const char* window_title_text_;
    bool is_destroyed_;
public:
    Window () {
        x_ = 0;
        y_ = 0;
        w_ = 0;
        h_ = 0;
        window_title_text_ = " ";
        is_destroyed_ = false;
    }
    TextAreaVector vec_text_areas_;
    ButtonVector vec_buttons_;
    MenuVector vec_menus_;
    WindowSpriteVector vec_window_sprites_;
    void AddTextArea ( const char *text ) {
        TextArea text_area;
        text_area.set_text(text);
        text_area.set_w(strlen(text));
        text_area.set_h(get_default_text_area_padding()*2);
        vec_text_areas_.push_back(text_area);
    }
    void AddButton ( int value, const char *text ) {
        Button button;
        button.set_value(value);
        button.set_text(text);
        button.set_w(strlen(text));
        button.set_h(get_default_button_padding()*2);
        vec_buttons_.push_back(button);
    }
    void AddMenu ( Menu menu ) {
        vec_menus_.push_back(menu);
    }
    void AddWindowSprite ( WindowSprite window_sprite ) {
        vec_window_sprites_.push_back(window_sprite);
    }
    void set_window_title_text ( const char *text ) {
        window_title_text_ = text;
    }
    const char* get_window_title_text () {
        return window_title_text_;
    }
    void set_is_destroyed ( bool is_destroyed ) {
        is_destroyed_ = is_destroyed;
    }
    bool get_is_destroyed () {
        return is_destroyed_;
    }
    bool operator == (Window&);
    bool CheckIdentity (Window);
};

#endif
