#include "Button.h"

struct Button* initButton(){
	struct Button* b = (struct Button*)malloc(sizeof(struct Button));
	b->name = NULL;
	b->range = NULL;
	b->x_pos = 0;
	b->y_pos = 0;
	b->buttonType = 3;
	b->collide = dummyCollide;
	b->draw = dummyDraw;
	b->Panel = NULL;
	b->isClicked = 0;
	return b;
}

struct Button* initMenuButton(int x, char* name, int type, struct Frame* menuFrame){
	struct Button* b = initButton();
	b->range = initRange(x, 1, 30, 10);
	b->name = name;
	b->x_pos = x;
	b->y_pos = 1; // all menu buttons have to be drawn at y = 1
	b->draw = drawTxtButton;
	b->type = type;
	b->collide = menuButtonCollide;
	b->Panel = menuFrame;
	return b;
}

struct Button* initSongButton(int x, int y, char* name, struct Frame* panel){
	struct Button* sb = initButton();
	sb->name = name;
	sb->Panel = panel;
	sb->range = initRange(x, y, 79, 10);
	sb->x_pos = x;
	sb->y_pos = y;
	sb->draw = drawTxtButton;
	sb->collide = songButtonCollide;
	return sb;
}
struct Button* initPlaylistButton(int x, int y, char* name, struct Frame* panel){
	struct Button* pb = initButton();
	pb->name = name;
	pb->x_pos = x;
	pb->y_pos = y;
	pb->draw = drawTxtButton;
	pb->Panel = panel;
	pb->range = initRange(x, y, 79, 10);
	pb->collide = playlistButtonCollide;
	return pb;
}

// action button type - used for drawing position later
// 0:PLAY, 1:STOP, 2:PAUSE, 3:PREV, 4:NEXT
struct Button* initActionButton(int type){
	struct Button* ab = initButton();
	ab->x_pos = getXActionBtn(type);
	ab->y_pos = getYActionBtn(type);
	ab->range = initRange(ab->x_pos, ab->y_pos, 20, 20);
	ab->type = type;
	ab->draw = drawActionButton;
	ab->collide = actionButtonCollide;
	return ab;
}

void drawTxtButton(struct Button* this){
	alt_up_char_buffer_string(char_buffer, this->name, this->x_pos, this->y_pos);
}
void drawActionButton(struct Button* this){
	draw(this->x_pos, this->y_pos, this->stats[0]);
}
void dummyDraw(struct Button* this){}

void menuButtonCollide(struct Button* this){
	// 0:ALLSONGS, 1:PLAYLISTS
	draw_notransparent(241, 13, this->Panel->mainFrame->elements[2]->bg_image);
	switch(this->type){
	case 0:
		allSongsMenuButtonCollide(this);
		this->Panel->mainFrame->currentPanel = 0;
		break;
	case 1:
		playlistMenuButtonCollide(this);
		this->Panel->mainFrame->currentPanel = 1;
		break;
	default:
		break;
	}
}
void playlistMenuButtonCollide(struct Button* this){
	clearSongPanel();
	if (this->Panel->mainFrame->elements[3] == NULL){
		printf("Playlist frame is NULL\n");
		return;
	}
	this->Panel->mainFrame->elements[3]->drawFrame(this->Panel->mainFrame->elements[3]);
}
void allSongsMenuButtonCollide(struct Button* this){
	clearSongPanel();
	if (this->Panel->mainFrame->elements[2] == NULL){
		printf("All Songs frame is NULL\n");
		return;
	}
	this->Panel->mainFrame->elements[2]->drawFrame(this->Panel->mainFrame->elements[2]);
}

void actionButtonCollide(struct Button* this){
	switch(this->type){
	case 0:
		playButtonCollide(this);
		break;
	case 1:
		stopButtonCollide(this);
		break;
	case 2:
		pauseButtonCollide(this);
		break;
	case 3:
		prevButtonCollide(this);
		break;
	case 4:
		nextButtonCollide(this);
		break;
	default:
		break;
	}
}

/**
 * helper function for debugging
 * this will draw the range of a button with white color
 */
void drawRange(struct Button* this){
	int i = 0;
	int y = this->y_pos;
	for (i = 0; i < this->range->height; i++, y++){
		drawHorizontalLine(this->x_pos, y, this->range->width, 0xffffff);
	}
}

void playButtonCollide(struct Button* this){
	drawRange(this);
	printf("Play button is clicked\n");
}

void pauseButtonCollide(struct Button* this){
	// TODO: range is still not correct!!
	drawRange(this);
	printf("Pause button is clicked.\n");
}

void stopButtonCollide(struct Button* this){
	drawRange(this);
	printf("Stop button is clicked\n");
}

void prevButtonCollide(struct Button* this){
	drawRange(this);
	printf("Prev button is clicked.\n");
}

void nextButtonCollide(struct Button* this){
	drawRange(this);
	printf("Next button is clicked.\n");
}

void dummyCollide(struct Button* this){
	printf("ERROR:Dummy Collide is called.\n");
}

void songButtonCollide(struct Button* this){
	// highlight the song
	draw_notransparent(241, 13, this->Panel->bg_image);
	int i = 0;
	int y = 4* this->y_pos - 2;
	int x = 4* this->x_pos - 3;
	for (i = 0; i < 10; i++, y++){
		drawHorizontalLine(x, y, 76, 0xe711ce);
	}
}

void playlistButtonCollide(struct Button* this){
	// highlight the playlist
	draw_notransparent(241, 13, this->Panel->bg_image);
	int i =0;
	int y = 4* this->y_pos - 2;
	int x = 4* this->x_pos - 3;
	for (i = 0; i < 10; i++, y++){
		drawHorizontalLine(x, y, 76, 0xe711ce);
	}
}

int getXActionBtn(int actionBtnType){
	int x = 0;
	switch(actionBtnType){
	case 0:
		x = 145;
		break;
	case 1:
		x = 120;
		break;
	case 2:
		x = 180;
		break;
	case 3:
		x = 95;
		break;
	case 4:
		x = 205;
		break;
	default:
		break;
	}
	return x;
}

int getYActionBtn(int actionBtnType){
	int y = 0;
	switch(actionBtnType){
	case 0:
		y = 200;
		break;
	default:
		y = 205;
		break;
	}
	return y;
}


