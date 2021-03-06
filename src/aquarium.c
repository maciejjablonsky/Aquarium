#include <SDL2/SDL_events.h>
#include "aquarium.h"
#include "object.h"
#include "motion/cartesian_point.h"
#include "memory_handling.h"
#include "exit_codes.h"
typedef enum {
    AQUARIUM_NO_MEMORY, AQUARIUM_FISHES_FAIL, AQUARIUM_DISPLAY_FAIL, AQUARIUM_CLOCK_FAIL
} aquarium_error_code_t;


static aquarium_t *delete_failed_aquarium(aquarium_t *this, aquarium_error_code_t error_code);

aquarium_error_code_t get_aquarium_error_code(aquarium_t *this);

static aquarium_t *initialize_aquarium_from_config(aquarium_t *this, config_parser_t *config_parser);

aquarium_t *new_aquarium(display_initial_data_t *display_initial_data, fishes_initial_data_t *fish_initial_data,
                         size_t amount_of_fishes) {
    aquarium_t *this = new_object(sizeof(aquarium_t));
    if (is_not_created(this)) {
        return delete_failed_aquarium(this, AQUARIUM_NO_MEMORY);
    }

    this->display = new_display(display_initial_data);
    this->fishes = new_fishes(fish_initial_data, amount_of_fishes);
    this->clock = new_time_handler();
    this->action = false;
    if (IS_ALL_CREATED(this->fishes, this->display, this->clock)) {
        return this;
    } else {
        return delete_failed_aquarium(this, get_aquarium_error_code(this));
    }
}

aquarium_t *new_aquarium_from_config(config_parser_t *config_parser) {
    aquarium_t *this = new_object(sizeof(aquarium_t));
    if (is_not_created(this)) {
        return delete_failed_aquarium(this, AQUARIUM_NO_MEMORY);
    }
    initialize_aquarium_from_config(this, config_parser);
    if (IS_ALL_CREATED(this->fishes, this->display, this->clock)) {
        return this;
    } else {
        return delete_failed_aquarium(this, get_aquarium_error_code(this));
    }
}

aquarium_t *initialize_aquarium_from_config(aquarium_t *this, config_parser_t *config_parser) {
    this->fishes = new_fishes_from_config(config_parser);
    this->clock = new_time_handler();
    this->action = false;
    this->display = new_display_from_config(config_parser);
    return this;
}

static aquarium_t *delete_failed_aquarium(aquarium_t *this, aquarium_error_code_t error_code) {
    switch (error_code) {
        case AQUARIUM_NO_MEMORY:
            MEMORY_NOT_ALLOCATED_MESSAGE();
            break;
        case AQUARIUM_FISHES_FAIL:
            NEW_OBJECT_FAILURE(FISHES_T_NAME);
            break;
        case AQUARIUM_DISPLAY_FAIL:
            NEW_OBJECT_FAILURE(DISPLAY_T_NAME);
            break;
        case AQUARIUM_CLOCK_FAIL:
            NEW_OBJECT_FAILURE(TIME_HANDLER_T_NAME);
            break;
        default:
            break;
    }
    return delete_aquarium(this);
}

aquarium_error_code_t get_aquarium_error_code(aquarium_t *this) {
    if (is_not_created(this)) return AQUARIUM_NO_MEMORY;
    else if (is_not_created(this->display)) return AQUARIUM_DISPLAY_FAIL;
    else if (is_not_created(this->fishes)) return AQUARIUM_FISHES_FAIL;
    else if (is_not_created(this->clock)) return AQUARIUM_CLOCK_FAIL;
    else return -1;
}


aquarium_t *delete_aquarium(aquarium_t *this) {
    if (is_not_created(this)) {
        return NULL;
    }
    this->fishes = delete_fishes(this->fishes);
    this->display = delete_display(this->display);
    this->clock = delete_time_handler(this->clock);
    if (IS_ALL_DELETED(this->fishes, this->display, this->clock)) {
        return delete_object(this);
    } else {
        DELETE_OBJECT_FAILURE(AQUARIUM_T_NAME);
        exit(EXIT_FREE_MEMORY_ERROR);
    }
}

void run_aquarium(aquarium_t *aquarium) {
    reset_time_handler(aquarium->clock);
    start_action(aquarium);
    update_time_handler(aquarium->clock);
    while (is_aquarium_running(aquarium)) {
        handle_events(aquarium);
        update_time_handler(aquarium->clock);
        move_each_fish_in_aquarium(aquarium->fishes, aquarium->clock, get_aquarium_dimensions(aquarium));
        show_aquarium_contents(aquarium->display, aquarium->fishes, aquarium->clock);
    }
}

bool start_action(aquarium_t *aquarium) {
    return aquarium->action = true;
}

bool stop_action(aquarium_t *aquarium) {
    return aquarium->action = false;
}

bool is_aquarium_running(aquarium_t *aquarium) {
    return aquarium->action;
}

void handle_events(aquarium_t *aquarium) {
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event) > 0) {
        dispatch_event(aquarium, &sdl_event);
    }
}

void dispatch_event(aquarium_t *aquarium, SDL_Event *sdl_event) {
    switch (sdl_event->type) {
        case SDL_QUIT:
            stop_action(aquarium);
            break;
        case SDL_KEYDOWN:
            dispatch_pressed_key(aquarium, sdl_event);
            break;
    }
}

void dispatch_pressed_key(aquarium_t *aquarium, SDL_Event *sdl_event) {
    switch (sdl_event->key.keysym.sym) {
        case SDLK_ESCAPE:
            stop_action(aquarium);
            break;
        case SDLK_UP:
            multiply_fishes_velocity(aquarium->fishes, 1.2);
            break;
        case SDLK_DOWN:
            multiply_fishes_velocity(aquarium->fishes, 0.8);
            break;
        default:
            break;
    }
}

cartesian_point_t get_aquarium_dimensions(aquarium_t *aquarium) {
    cartesian_point_t dimensions;
    int x, y;
    SDL_GetWindowSize(aquarium->display->window, &x, &y);
    dimensions.x = x;
    dimensions.y = y;
    return dimensions;
}



