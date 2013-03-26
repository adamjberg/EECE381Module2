/*
 * Graphic.h
 *
 *  Created on: 2013-03-21
 *      Author: danny
 */

#ifndef GRAPHIC_H_
#define GRAPHIC_H_
#include "Global.h"
#define BACKGROUND 0
struct Image {
	int* buffer;
	int x, y, prev_x, prev_y;
	int height, width;
	struct Image* prev;
	struct Image* next;
	struct Image* first;
	struct Image* end;
};

void initVGA();
struct Image* initImage(int*, int, int, int);
void killImage(struct Image*);
void addImage(struct Image*, struct Image*);
void setImagePos(struct Image*, int, int);
struct Image* loadSDImage(char*);
int getColor(int, int, int);
int getColor555(int);
void draw(int, int, struct Image*);
void draw_notransparent(int, int, struct Image*);
void drawBox(int, int, int, int, int);
void drawHorizontalLine(int, int, int ,int);
void drawVerticalLine(int, int, int, int);
#endif /* GRAPHIC_H_ */
