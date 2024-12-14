/**
 * @file brik.hpp
 * @author Ethan Van Ruyskensvelde (Main developer)
 * @brief Define the class Brik
 * @date 27/11/2024
 *
 */

#ifndef BRIKUI_HPP
#define BRIKUI_HPP

#include "../figures/forme.hpp"

class BrikUi final : public Rectangle {
  public:
    BrikUi(Point center, float width, float height, ALLEGRO_COLOR color);
    virtual ~BrikUi() = default;

    void draw() override;
};

#endif // BRIKUI_HPP
