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
	return b;
}

struct Button* initMenuButton(int x, char* name, int color, int type, struct Frame* mainFrame){
	struct Button* b = initButton();
	b->range = initRange(x, 1, 5, 10);
	b->name = name;
	b->x_pos = x;
	b->y_pos = 1; // all menu buttons have to be drawn at y = 1
	b->draw = drawMenuButton;
	b->type = type;
	b->collide = menuButtonCollide;
	b->mainPanel = mainFrame;
	return b;
}

struct Button* initSongButton(int x, int y, char* name, int color){
	struct Button* sb = initButton();
	sb->name = name;
	sb->x_pos = x;
	sb->y_pos = y;
	sb->draw = drawSongButton;
	return sb;
}
struct Button* initPlaylistButton(int x, int y, char* name, int color){
	struct Button* pb = initButton();
	pb->name = name;
	pb->x_pos = x;
	pb->y_pos = y;
	pb->draw = drawMenuButton;
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

void drawMenuButton(struct Button* this){
	// right now just write
	alt_up_char_buffer_string(char_buffer, this->name, this->x_pos, this->y_pos);
}

void drawSongButton(struct Button* this){
	// TODO: draw with different gradient background color,
	// may need to add integer as parameter.
	alt_up_char_buffer_string(char_buffer, this->name, this->x_pos, this->y_pos);
}

void drawActionButton(struct Button* this){
	draw(this->x_pos, this->y_pos, this->stats[0]);
}

void dummyDraw(struct Button* this){

}

void menuButtonCollide(struct Button* this){
	// 0:ALLSONGS, 1:PLAYLISTS
	switch(this->type){
	case 0:
		allSongsMenuButtonCollide(this);
		break;
	case 1:
		playlistMenuButtonCollide(this);
		break;
	default:
		break;
	}
}
void playlistMenuButtonCollide(struct Button* this){
	clearSongPanel();
	if (this->mainPanel->elements[3] == NULL){
		printf("Playlist frame is NULL\n");
		return;
	}
	this->mainPanel->elements[3]->drawFrame(this->mainPanel->elements[3]);
}
void allSongsMenuButtonCollide(struct Button* this){
	clearSongPanel();
	if (this->mainPanel->elements[2] == NULL){
		printf("All Songs frame is NULL\n");
		return;
	}
	this->mainPanel->elements[2]->drawFrame(this->mainPanel->elements[2]);
}

void actionButtonCollide(struct Button* this){
	switch(this->type){
	case 0:
		playButtonCollide(this);
		break;
	default:
		break;
	}
}

void playButtonCollide(struct Button* this){
	printf("Play button is clicked\n");
}

void dummyCollide(struct Button* this){
	printf("ERROR:Dummy Collide is called.\n");
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


