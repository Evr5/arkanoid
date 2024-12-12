/**
 * @file brik.cpp
 * @author Ethan Van Ruyskensvelde (Main developer)
 * @brief Define the class Brik
 * @date 27/11/2024
 *
 */

#include "brik_ui.hpp"

// ### Constructor ###
BrikUi::BrikUi(Point center, float width, float height, ALLEGRO_COLOR color)
    : Rectangle(center, width, height, color) {}

// ### Public methods ###
void BrikUi::draw() {
    center_.x = center_.x + 50; // Move from 50 because the screen is 100 larger so it is centered
    center_.y =
        1000
        - center_.y; // Invert the y axis to match the screen with the backend
    Rectangle::draw();
}
