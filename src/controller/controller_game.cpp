/**
 * @file controller_game.cpp
 * @author Ethan Van Ruyskensvelde (Main developer)
 * @brief Class that manage the current game
 */

#include "controller_game.hpp"
#include "../global_variables.hpp"

#include <allegro5/allegro.h>

#include <iostream>
#include <ostream>

using namespace std;

// TODO : Arrêter toutes les ressoucrces quand y a le message de game over ou de win (autres affichages allegro et le backend) 
// TODO : Problème lors du chargement d'un niveau

// ### Constructor ###
ControllerGame::ControllerGame() : gameBoard_{make_shared<GameBoard>()} {
    displayGame_ = make_shared<DisplayGame>(gameBoard_);
    if (!al_init()) { // initialize allegro
        cerr << "Failed to initialize Allegro" << endl;
        exit(-1);
    }

    if (!al_install_keyboard()) {
        cerr << "Failed to install the keyboard" << endl;
        exit(-1);
    }

    if (!al_install_mouse()) {
        cerr << "Failed to install the mouse" << endl;
        exit(-1);
    }

    timer_ = al_create_timer(1.0 / 360); // TODO: check the FPS we want for allegro
    if (!timer_) {
        cerr << "Failed to create timer" << endl;
        exit(-1);
    }

    queue_ = al_create_event_queue();
    if (!queue_) {
        cerr << "Failed to create queue" << endl;
        exit(-1);
    }

    al_register_event_source(
        queue_,
        al_get_display_event_source(
            displayGame_->getDisplay())); // register the display event source
    al_register_event_source(
        queue_,
        al_get_keyboard_event_source()); // register the keyboard event source
    al_register_event_source(
        queue_, al_get_mouse_event_source()); // register the mouse event source
    al_register_event_source(
        queue_,
        al_get_timer_event_source(timer_)); // register the timer event source

    loadLevel(); // at the start of the game we start a level // TODO: check if good
}

// ### Destructor ###
ControllerGame::~ControllerGame() {}

// ### Public methods ###
void ControllerGame::process() {
    while (!done_) {

        currentTime_ = al_get_time(); // Actual time
        double deltaTime = currentTime_ - lastTime_; // Time between two frames
        lastTime_ = currentTime_;                    // Update the last time

        al_get_mouse_state(&mouseState_); // get the mouse state

        // check if the mouse is in the window // TODO: check si c'est utile
        if (mouseState_.x < 0) {
            mouseState_.x = 0;
        } else if (mouseState_.x > static_cast<int>(SCREEN_WIDTH)) {
            mouseState_.x = static_cast<int>(SCREEN_WIDTH);
        }

        gameBoard_->setRacketAtX(static_cast<double>(
            mouseState_.x)); // move the racket with the mouse

        gameBoard_->update(deltaTime);


        checkGameOver(); // check if the player has lifes // TODO : check if it's
                     // the right place to do that
        checkWin();  // check if the player has won
        
        al_wait_for_event(queue_, nullptr);

        while (al_get_next_event(queue_, &event_)) { // get the next event
            checkEventType();
        }

        if (draw_) {
            drawGame();
        }
    }
}


// ### Private methods ###

void ControllerGame::drawGame() {
    al_start_timer(timer_);
    draw_ = false;

    displayGame_->draw(); // draw the pieces
}

void ControllerGame::checkGameOver() {
    if (gameBoard_->getLife() == 0) {
        al_stop_timer(timer_);
        gameBoard_->saveCurrentScore();
        gameBoard_->clearVectors(); // TODO: check if we do that ?
        gameBoard_->resetTheLife();
        gameBoard_->resetTheScore();
        displayGame_->gameOver();
        waitKeyToRestart();
    }   
}
void ControllerGame::checkWin() {
    if (gameBoard_->getNbBricks()
        == 0) { // if there is no more bricks // TODO: not working
        al_stop_timer(timer_);
        gameBoard_->saveCurrentScore();
        gameBoard_->clearVectors(); // TODO: check if we do that ?
        gameBoard_->resetTheLife();
        gameBoard_->resetTheScore();
        displayGame_->gameWin();
        waitKeyToRestart();
    }
}

void ControllerGame::checkEventType() {
    if (event_.type
        == ALLEGRO_EVENT_DISPLAY_CLOSE) { // if the display is closed
        done_ = true;
    }

    else if (event_.type == ALLEGRO_EVENT_TIMER) {
        al_stop_timer(timer_);
        draw_ = true;

        if (key_[ALLEGRO_KEY_SPACE]) {
            // shootLazer(); TODO : shoot lazer when bonus
        }
    }

    else if (event_.type == ALLEGRO_EVENT_KEY_DOWN) {
        key_.set(event_.keyboard.keycode, true); // set the key pressed to true
    }

    else if (event_.type == ALLEGRO_EVENT_KEY_UP) {
        key_.reset(
            event_.keyboard
                .keycode); // set the key that is no longer pressed to false
    }
}

void ControllerGame::waitKeyToRestart() {
    al_wait_for_event(queue_, nullptr);
    while (!(event_.type == ALLEGRO_EVENT_KEY_DOWN)) { // wait for a key to be pressed
        al_get_next_event(queue_, &event_);
        if (event_.type
        == ALLEGRO_EVENT_DISPLAY_CLOSE) { // if the display is closed
            done_ = true;
            break;
        }
    }
    loadLevel(); // TODO: check if it's the right place to do that
}

void ControllerGame::loadLevel() {
    const std::vector<std::shared_ptr<Ball>> balls = {
        std::make_shared<Ball>(Vec2{450, 85}, Vec2{0, 1}, 10, 500)};

    const std::vector<std::shared_ptr<Brick>> bricks{
        Brick::makeBrick(Color::red,
                         BoundingBox{Vec2{50, 800}, Vec2{100, 775}}),
        Brick::makeBrick(Color::red,
                         BoundingBox{Vec2{100, 800}, Vec2{150, 775}}),
        Brick::makeBrick(Color::red,
                         BoundingBox{Vec2{150, 800}, Vec2{200, 775}}),
        Brick::makeBrick(Color::red,
                         BoundingBox{Vec2{200, 800}, Vec2{250, 775}}),
        Brick::makeBrick(Color::red,
                         BoundingBox{Vec2{250, 800}, Vec2{300, 775}}),
        Brick::makeBrick(Color::red,
                         BoundingBox{Vec2{300, 800}, Vec2{350, 775}}),
        Brick::makeBrick(Color::red,
                         BoundingBox{Vec2{350, 800}, Vec2{400, 775}}),
        Brick::makeBrick(Color::red,
                         BoundingBox{Vec2{400, 800}, Vec2{450, 775}}),
        Brick::makeBrick(Color::red,
                         BoundingBox{Vec2{450, 800}, Vec2{500, 775}}),
        Brick::makeBrick(Color::red,
                         BoundingBox{Vec2{500, 800}, Vec2{550, 775}}),
        Brick::makeBrick(Color::red,
                         BoundingBox{Vec2{550, 800}, Vec2{600, 775}})};    


    // with T=thickness, H=height, W=width
    const std::vector<std::shared_ptr<Border>> borders = {
        // TODO: Lucas doit voir : Les murs font partie de la grille
        // (0, 0) -> (0 + boardBoundingsThickness - 1, boardHeight - 1) // left wall
        std::make_shared<Border>(
            Border{BoundingBox{Vec2{0, 0},
                               Vec2{boardBoundingsThickness - 1, boardHeight -1 }}}),
        // (0, boardHeight - 1) -> (boardWidth - 1, boardHeight - boardBoundingsThickness - 1) // upper wall
        std::make_shared<Border>(
            BoundingBox{Vec2{0, boardHeight - 1},
                        Vec2{boardWidth -1 , boardHeight - boardBoundingsThickness - 1}}),
        // (boardWidth - boardBoundingsThickness - 1, 0) -> (boardWidth - 1, boardHeight - 1) // right wall
        std::make_shared<Border>(BoundingBox{
            Vec2{boardWidth - boardBoundingsThickness -1 , 0},
            Vec2{boardWidth - 1, boardHeight - 1}})};

    // Racket racket;
    // NOTE: doing this to get an iterator without having to rewrite it myself
    const std::vector<std::shared_ptr<Racket>> rackets{
        std::make_shared<Racket>(BoundingBox{Vec2{450, 50}, 100, 25})};

    gameBoard_->setRacket(rackets);
    gameBoard_->setBalls(balls);
    gameBoard_->setBorders(borders);
    gameBoard_->setBricks(bricks);

    al_start_timer(timer_);
    lastTime_ = al_get_time(); // get the time at the beginning of the game

}

