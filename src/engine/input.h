#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL.h>

//TODO: this isnt how this works. The code right now,
// is just copying addresses so both will forever be current_state
// I would need to instead use bool*, and use some stdlib functions
// to copy over the data or some such
typedef struct input {
	const bool* previous_keyboard_state;
	const bool* current_keyboard_state;
} Input;

void update_keyboard_state(Input* input) {
	input->previous_keyboard_state = input->current_keyboard_state;
	input->current_keyboard_state = SDL_GetKeyboardState(NULL);
}

bool is_key_pressed(Input* input, SDL_Scancode key) {
	return input->current_keyboard_state[key];
}

bool is_key_released(Input* input, SDL_Scancode key) {
	return (!input->current_keyboard_state[key]) &&
		(input->previous_keyboard_state[key]);
}

#endif