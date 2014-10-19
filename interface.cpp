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

#include "interface.h"

Window Interface::CreateWindow ( Window window )
{
    int longest_text_area = 1;
    for (TextAreaIterator t = window.vec_text_areas_.begin(); t != window.vec_text_areas_.end(); t++) {
        t->set_h(font_size_);
        window.set_h(window.get_h() + t->get_h() + font_line_spacing_);
        if (t->get_w() > longest_text_area) longest_text_area = t->get_w();
    }
    if (window.get_w() == 0) window.set_w(longest_text_area*(font_size_-4) + window.get_default_window_border_size()*2 + window.get_default_window_padding()*2);
    if (window.get_h() == 0) window.set_h( (window.vec_menus_.size() + window.vec_buttons_.size()
                                                + window.vec_text_areas_.size()) * font_size_ );
    if (window.get_x() == 0) window.set_x((screen_width_/2)-(window.get_w()/2));
    if (window.get_y() == 0) window.set_y((screen_height_/2)-(window.get_h()/2));
    if (window.get_window_title_text()) window.set_h(window.get_h() + font_size_);
    for (unsigned int menu_element = 0; menu_element < window.vec_menus_.size(); menu_element++) {
        for (unsigned int option_element = 0; option_element < window.vec_menus_.at(menu_element).vec_options_.size(); option_element++) {
            window.set_h(window.get_h() + font_size_ + font_line_spacing_ + default_menu_padding_ );
        }
        window.vec_menus_.at(menu_element).set_x(20 + window.get_x()+window.get_default_window_border_size()+window.get_default_window_padding());
        window.vec_menus_.at(menu_element).set_y(window.get_y()+window.get_default_window_padding()+window.get_default_menu_padding()+
                (font_size_* (int) window.vec_text_areas_.size() ));
    }
    int button_column = 0;
    for (ButtonIterator b = window.vec_buttons_.begin(); b != window.vec_buttons_.end(); b++) {
        b->set_h(b->get_h() + font_size_);
        if (b == window.vec_buttons_.begin()) {
            window.set_h(window.get_h() + b->get_h() + window.get_default_window_padding());
        }
        b->set_w(b->get_w() * (font_size_-4));
        b->set_x(window.get_x()+window.get_default_window_border_size() + window.get_default_window_padding() + button_column + b->get_h());
        b->set_y(((window.get_y() + window.get_h()) - b->get_h()/2) - window.get_default_window_padding());
        button_column += b->get_w() + 20;
    }
    printf("'%s' window with dimensions (%d, %d, %d, %d) created.\n", window.get_window_title_text(), window.get_x(), window.get_y(),
           window.get_w(), window.get_h());
    return window;
}

bool Window::operator== (Window &window)
{
    if (&window == this) return true;
    else {
        return false;
    }
}

bool Window::CheckIdentity (Window window)
{
    if (&window == this) return true;
    else {
        return false;
    }
}
