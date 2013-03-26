#include "Button.h"

struct Button* initButton(){
	struct Button* b = (struct Button*)malloc(sizeof(struct Button));
	b->name = NULL;
	b->x_pos = 0;
	b->y_pos = 0;
	b->buttonType = 3;
	return b;
}

struct Button* initMenuButton(int x, char* name, int color){
	struct Button* b = initButton();
	b->name = name;
	b->x_pos = x;
	b->y_pos = 1; // all menu buttons have to be drawn at y = 1
	b->draw = drawMenuButton;
	b->buttonType = menu;
	//b->bg_color = color;
	return b;
}

struct Button* initSongButton(int x, int y, char* name, int color){
	struct Button* sb = initButton();
	sb->name = name;
	sb->x_pos = x;
	sb->y_pos = y;
	sb->buttonType = song;
	sb->draw = drawMenuButton;
	return sb;
}

void drawMenuButton(struct Button* this){
	// right now just write
	alt_up_char_buffer_string(char_buffer, this->name, this->x_pos, this->y_pos);
}

void dummyDraw(struct Button* this){

}



