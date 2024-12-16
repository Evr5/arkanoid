/**
 * @file racket.cpp
 * @author Ethan Van Ruyskensvelde (Main developer)
 * @brief Define the racket class
 * @date 26/11/2024
 *
 */

#include "racket_ui.hpp"
#include <allegro5/color.h>

// TODO : Removing magic numbers and set Global variables

// ### Constructor ###
// TODO: do we still need the default constructor ?
RacketUi::RacketUi() : Rectangle(Point(0, 0), 0, 0, COLOR_WHITE) {}

RacketUi::RacketUi(Point center, float width, float height, ALLEGRO_COLOR color)
    : Rectangle(center, width, height, color) {}

// ### Public methods ###
void RacketUi::draw() {
    center_.y =
        SCREEN_HEIGHT
        - center_.y; // Invert the y axis to match the screen with the backend
    Rectangle::draw();
}
