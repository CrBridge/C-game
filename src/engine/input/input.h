#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL.h>

#include <stdlib.h>
#include <stdio.h>

#include "../util.h"
#include "../types.h"

void input_init_keyboard_state(void);

void input_update_current_keyboard_state(void);
void input_update_previous_keyboard_state(void);

bool input_is_key_up(SDL_Scancode key);
bool input_is_key_down(SDL_Scancode key);
bool input_is_key_pressed(SDL_Scancode key);
bool input_is_key_released(SDL_Scancode key);

#endif