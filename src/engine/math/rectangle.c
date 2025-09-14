#include "rectangle.h"

Rectangle rectangle_init(int x, int y, int width, int height) {
	Rectangle r = {0};
	r.x = x;
	r.y = y;
	r.width = width;
	r.height = height;

	return r;
}
