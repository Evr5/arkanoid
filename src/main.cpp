/**
 * @file initialize_allegro.cpp
 * @author Ethan Van Ruyskensvelde (Main developper)
 * @brief Initialisation of Allegro at the beginning of the program
 * @date 24/11/2024
 *
 */

#include <allegro5/allegro.h>
#include <allegro5/display.h>
#include <allegro5/drawing.h>
#include <allegro5/events.h>
#include <allegro5/keyboard.h>
#include <allegro5/mouse.h>
#include <allegro5/system.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/timer.h>
#include <allegro5/keycodes.h>

#include <iostream>

#include "canvas/canvas.hpp"
#include "figures/point.hpp"
#include "init_allegro/initialize_allegro.hpp"
#include "colors/colors.hpp"
#include "figures/forme.hpp"


using namespace std;

static const int SCREEN_WIDTH = 1000;
static const int SCREEN_HEIGHT = 1000;
const double FPS = 60;

void drawWallGame(){
    // draw the walls of the game
    al_draw_filled_rectangle(20, 100, SCREEN_WIDTH-20, 110, COLOR_BLACK); // upper wall
    al_draw_filled_rectangle(20, 100, 30, SCREEN_HEIGHT-20, COLOR_BLACK); // left wall
    al_draw_filled_rectangle(SCREEN_WIDTH - 30, 100, SCREEN_WIDTH-20, SCREEN_HEIGHT-20, COLOR_BLACK); // right wall
    al_draw_filled_rectangle(20, SCREEN_HEIGHT - 30, SCREEN_WIDTH-20, SCREEN_HEIGHT-20, COLOR_BLACK); // bottom wall
}

int main(int /* argc */, char ** /* argv */){

    if (initialize_allegro() != 0){
        return -1;
    }
        
    ALLEGRO_FONT *font = al_load_ttf_font("/usr/share/fonts/TTF/Arial.TTF", 24, 0); // load the font
    if (!font){
        cerr << "Failed to load the font" << endl;
        return -1;
    }

    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display){
        cerr << "Failed to create a display" << endl;
        return -1;
    }

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS); 
    if (!timer){
        cerr << "Failed to create a timer" << endl;
        return -1;
    }

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    if (!queue){
        cerr << "Failed to create an event queue" << endl;
        return -1;
    }

    al_register_event_source(queue, al_get_display_event_source(display)); // register the display event source
    al_register_event_source(queue, al_get_keyboard_event_source()); // register the keyboard event source
    al_register_event_source(queue, al_get_mouse_event_source()); // register the mouse event source
    al_register_event_source(queue, al_get_timer_event_source(timer)); // register the timer event source

    bool done = false;
    bool draw = false;
    ALLEGRO_EVENT event;
    Canvas canvas;


    al_start_timer(timer);
    while (!done){
        al_wait_for_event(queue, nullptr); // wait for an event
        while (al_get_next_event(queue, &event)) { // get the next event
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { // if the display is closed
                done = true;
            } 
            else if (event.type == ALLEGRO_EVENT_TIMER) {
                draw = true;
                al_stop_timer(timer);
            }
            else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        canvas.moveBall(0, -10);
                        break;
                    case ALLEGRO_KEY_DOWN:
                        canvas.moveBall(0, 10);
                        break;
                    case ALLEGRO_KEY_LEFT:
                        canvas.moveBall(-10, 0);
                        break;
                    case ALLEGRO_KEY_RIGHT: 
                        canvas.moveBall(10, 0);
                        break;
                }

            }
        }

        if (draw){
            draw = false;
            al_start_timer(timer);
            al_clear_to_color(COLOR_WHITE);
            al_draw_text(font, COLOR_BLACK, SCREEN_WIDTH / 2,50, ALLEGRO_ALIGN_CENTER, "Arkanoid");
            drawWallGame();
            canvas.draw();
            al_flip_display(); // update the window display
        }
    }

    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
    return 0;
}