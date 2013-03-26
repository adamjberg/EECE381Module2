#include "Frame.h"

struct Frame* initFrame(){
	struct Frame* f = (struct Frame*)malloc(sizeof(struct Frame));
	f->element_size = 0;
	return f;
}

struct Frame* initMainFrame(){
	struct Frame* f = initFrame();
	f->elements = (struct Frame**)malloc(2*sizeof(struct Frame*));
	f->elements[0] = initMenuFrame();
	f->elements[1] = initActionFrame();
	f->drawFrame = drawMainFrame;
	f->element_size = 2;
	return f;
}

struct Frame* initMenuFrame(){
	struct Frame* mf = initFrame();
	mf->buttons = (struct Button**)malloc(2*sizeof(struct Button*));
	mf->buttons[0] = initMenuButton(5, "All Songs", 0x123456);
	mf->buttons[1] = initMenuButton(20, "Playlists", 0x123456);
	mf->drawFrame = drawMenuFrame;
	return mf;
}

struct Frame* initActionFrame(){
	struct Frame* af = initFrame();
	af->buttons = (struct Button**)malloc(5*sizeof(struct Button*));
	af->buttons[0] = initButton();
	af->buttons[0]->stats[0] = loadSDImage("PLAY2.BMP"); //size 30x30
	af->buttons[1] = initButton();
	af->buttons[1]->stats[0] = loadSDImage("STOP.BMP"); //size 20x20
	af->buttons[2] = initButton();
	af->buttons[2]->stats[0] = loadSDImage("PAUSE.BMP");
	af->buttons[3] = initButton();
	af->buttons[3]->stats[0] = loadSDImage("PREV.BMP");
	af->buttons[4] = initButton();
	af->buttons[4]->stats[0] = loadSDImage("NEXT.BMP");
	af->drawFrame = drawActionFrame;
	return af;
}

struct Frame* initSongPanel(){
	struct Frame* sp = initFrame();
	sp->buttons = (struct Button**)malloc(sizeof(struct Button*));
	sp->buttons[0] = initSongButton(45, 4, "Song1.WAV", 0x123456);
	return sp;
}

void drawMainFrame(struct Frame* this){
	int i = 0;
	for (i = 0; i < this->element_size; i++){
		this->elements[i]->drawFrame(this->elements[i]);
	}
}

void drawMenuFrame(struct Frame* this){
	int x1, y1, x2, y2;
	drawHorizontalLine(0, 0, SCREEN_WIDTH, 0xFFFFFF);
	drawHorizontalLine(0, 11, SCREEN_WIDTH, 0xFFFFFF);
	x1 = 0; y1 = 1;
	x2 = x1 + 10;
	y2 = 11;
	while (x2 <= 320){
		drawBox(x1, y1, x2, y2, 0xc714b2);
		x1+=10;
		x2+=10;
	}
	this->buttons[0]->draw(this->buttons[0]);
	this->buttons[1]->draw(this->buttons[1]);
}

void drawActionFrame(struct Frame* this){
	drawHorizontalLine(0, 195, SCREEN_WIDTH, 0xFFFFFF);
	draw(145, 200, this->buttons[0]->stats[0]);
	draw(120, 205, this->buttons[1]->stats[0]);
	draw(180, 205, this->buttons[2]->stats[0]);
	draw(95, 205, this->buttons[3]->stats[0]);
	draw(205, 205, this->buttons[4]->stats[0]);
}

void drawSongPanel(struct Frame* this){
	drawVerticalLine(240, 12, 183, 0xFFFFFF);
	// make song buttons
}


void getSongs(){
	int i = 0;
	int x_pos = 10;
	int y_pos = 10;
	for (i = 1; i <= db.num_of_songs; i++){
		alt_up_char_buffer_string(char_buffer, db.songs[i]->song_name, x_pos, y_pos);
		y_pos = y_pos + 2;
	}
}

void getPlaylists(){
	int i = 0;
	int x_pos = 10;
	int y_pos = 10;
	for (i = 1; i <= db.num_of_songs; i++){
		alt_up_char_buffer_string(char_buffer, db.playlists[i]->list_name, x_pos, y_pos);
		y_pos = y_pos+2;
	}
}

void displayLoadingScreenVGA(){
	struct Image* testImg;
	while ((testImg = loadSDImage("TEST.BMP")) == NULL);
	draw(35, 35, testImg);
	killImage(testImg);
	alt_up_char_buffer_clear(char_buffer);
	alt_up_char_buffer_string(char_buffer, "Initialization Completed", 27, 5);
}



