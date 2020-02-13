//
// Created by foreverhungry on 12.02.2020.
//

#ifndef AQUARIUM_AQUARIUM_H
#define AQUARIUM_AQUARIUM_H
#include "display.h"
#include "fishes.h"
#include "time_handler.h"
#include <SDL2/SDL_rect.h>

typedef struct  {
    display_t * display;
    fishes_t * fishes;
    time_handler_t * clock;
    bool action;
} aquarium_t;

aquarium_t *create_aquarium(display_initial_data_t *display_initial_data, fishes_initial_data_t *fish_initial_data,
                            size_t amount_of_fishes);
aquarium_t * destroy_aquarium(aquarium_t * this);

void run_aquarium(aquarium_t * aquarium);
bool start_action(aquarium_t * aquarium);
bool stop_action(aquarium_t * aquarium);
bool is_aquarium_running(aquarium_t * aquarium);
void handle_events(aquarium_t * aquarium);
void apply_aquarium_behaviour(aquarium_t * aquarium);
void dispatch_event(aquarium_t * aquarium, SDL_Event * sdl_event);
void dispatch_pressed_key(aquarium_t *aquarium, SDL_Event *sdl_event);
cartesian_point_t get_aquarium_dimensions(aquarium_t * aquarium);
#endif //AQUARIUM_AQUARIUM_H
