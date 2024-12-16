/**
 * @file life.cpp
 * @author Ethan Van Ruyskensvelde (Main developer)
 * @brief Define the Life class
 * @date 16/11/2024
 *
 */

#include "life_counter.hpp"
#include "../../global_variables.hpp"
#include <iostream>

// ### Constructors ###

LifeCounter::LifeCounter() : numLifes_(INITIAL_NUM_LIFES) {}

LifeCounter::LifeCounter(unsigned numLifes) : numLifes_(numLifes) {}

// ### Destructor ###

LifeCounter::~LifeCounter() = default;

// #### Reset Value ####

void LifeCounter::reset() { numLifes_ = INITIAL_NUM_LIFES; }

// ### Arithmetic Operators ###

const LifeCounter &LifeCounter::operator--() {
    numLifes_--;
    return *this;
}

const LifeCounter &LifeCounter::operator++() {
    std::cout << "numLifes_ : " << numLifes_ << std::endl;
    numLifes_--;
    return *this;
}

const LifeCounter &LifeCounter::operator+=(unsigned numLife) {
    numLifes_ -= numLife;
    return *this;
}

// ### Setters ###

void LifeCounter::setNumLifes(const uint8_t numLifes) { numLifes_ = numLifes; }

// ### Conversion Operators ###

LifeCounter::operator unsigned() const { return numLifes_; }
