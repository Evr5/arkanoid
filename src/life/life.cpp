/**
 * @file life.hpp
 * @author Ethan Van Ruyskensvelde (Main developer)
 * @brief Define the Life class
 * @date 16/11/2024
 *
 */

#include "../global_variables.hpp"
#include <iostream>
#include "life.hpp"

#include <allegro5/allegro_primitives.h>

// ### Constructor ###

Life::Life() : nbLifes_(3) {
  isGaming_ = true;
}

Life::Life(uint8_t nbLifes) : nbLifes_(nbLifes) {
  isGaming_ = true;
}


// ### Destructor ###

Life::~Life() = default;


// ### Public Methods ###

void Life::removeOneLife() {
  nbLifes_--;
  if (nbLifes_ < 1) {
    isGaming_ = false;
    // TODO: we have to check the score in file score.txt and set the highest
    // score between the score in the file and the current score
    // TODO: we have to choose what to do when the game is over
  }
}

bool Life::currentlyGaming() const { return isGaming_; }

void Life::drawLife(ALLEGRO_BITMAP *heartImage) {
  int heartWidth = al_get_bitmap_width(heartImage);
  int heartHeight = al_get_bitmap_height(heartImage);

  float scaleHeartWidth = HEART_IMAGE_SIZE;
  float scaleHeartHeight = HEART_IMAGE_SIZE;

  float heartX = SCREEN_WIDTH / 4 + 40; // space between the text and the heart image
  
  for (float i = 0; i < static_cast<float>(this->nbLifes_); ++i) {
      // draw the heart image at the right of the text Life
      al_draw_scaled_bitmap(heartImage, 0, 0, static_cast<float>(heartWidth), static_cast<float>(heartHeight), heartX + i * (scaleHeartWidth + 5), 50, scaleHeartWidth, scaleHeartHeight, 0);
  }
}

ostream &operator<<(ostream &os, const Life &life) {
  for (int i = 0; i < life.nbLifes_; i++) {
    os << "Number of lifes: " << life.nbLifes_ << endl;
  }
  return os;
}

// ### Getters ###
uint8_t Life::getNbLifes() const { return nbLifes_; }


// ### Setters ###
void Life::setNbLifes(const uint8_t nbLifes) { nbLifes_ = nbLifes; }
void Life::setGaming(const bool isGaming) { isGaming_ = isGaming; }