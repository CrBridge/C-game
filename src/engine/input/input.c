#include "input.h"

bool* previous_keyboard_state = NULL;
const bool* current_keyboard_state = NULL;
int num_keys = 0;

void input_init_keyboard_state(void) {
	current_keyboard_state = SDL_GetKeyboardState(&num_keys);
	previous_keyboard_state = malloc(num_keys * sizeof(bool));
	memcpy(previous_keyboard_state, current_keyboard_state, num_keys * sizeof(bool));
}

void input_update_current_keyboard_state(void) {
	current_keyboard_state = SDL_GetKeyboardState(NULL);
}

void input_update_previous_keyboard_state(void) {
	memcpy(previous_keyboard_state, current_keyboard_state, num_keys * sizeof(bool));
}

bool input_is_key_up(SDL_Scancode key) {
	return !current_keyboard_state[key];
}

bool input_is_key_down(SDL_Scancode key) {
	return current_keyboard_state[key];
}

bool input_is_key_pressed(SDL_Scancode key) {
	return current_keyboard_state[key] &&
		!previous_keyboard_state[key];
}

bool input_is_key_released(SDL_Scancode key) {
	return !current_keyboard_state[key] &&
		previous_keyboard_state[key];
}
