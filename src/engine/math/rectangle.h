#ifndef RECTANGLE_H
#define RECTANGLE_H

typedef struct rectangle {
	int x, y;
	int width, height;
} Rectangle;

Rectangle rectangle_init(int x, int y, int width, int height);

#endif