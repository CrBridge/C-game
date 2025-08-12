#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL.h>

#include <stdlib.h>
#include <stdio.h>

#include "util.h"
#include "types.h"

bool* previous_keyboard_state = NULL;
const bool* current_keyboard_state = NULL;
int num_keys = 0;
int tmp = 0;

void init_keyboard_state(void) {
	current_keyboard_state = SDL_GetKeyboardState(&num_keys);
	previous_keyboard_state = malloc(num_keys * sizeof(bool));
	memcpy(previous_keyboard_state, current_keyboard_state, num_keys * sizeof(bool));
}

void update_current_keyboard_state(void) {
	current_keyboard_state = SDL_GetKeyboardState(NULL);
}

void update_previous_keyboard_state(void) {
	memcpy(previous_keyboard_state, current_keyboard_state, num_keys * sizeof(bool));
}

bool is_key_up(SDL_Scancode key) {
	return !current_keyboard_state[key];
}

bool is_key_down(SDL_Scancode key) {
	return current_keyboard_state[key];
}

bool is_key_pressed(SDL_Scancode key) {
	return current_keyboard_state[key] &&
		!previous_keyboard_state[key];
}

bool is_key_released(SDL_Scancode key) {
	return !current_keyboard_state[key] &&
		previous_keyboard_state[key];
}

#endif