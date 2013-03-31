#include "Frame.h"

struct Frame* initFrame(){
	struct Frame* f = (struct Frame*)malloc(sizeof(struct Frame));
	f->element_size = 0;
	return f;
}

struct Frame* initMainFrame(){
	struct Frame* f = initFrame();
	f->elements = (struct Frame**)malloc(4*sizeof(struct Frame*));
	f->elements[0] = initMenuFrame(f);
	f->elements[1] = initActionFrame(f);
	f->elements[2] = initSongPanel(f);
	f->elements[3] = initPlaylistPanel(f);
	f->buttons = NULL;
	f->bg_image = NULL;
	f->mainFrame = NULL;
	f->drawFrame = drawMainFrame;
	f->element_size = 4;
	f->button_size = 0;
	f->currentPanel = 0; // 0:SONGS, 1:PLAYLIST
	return f;
}

struct Frame* initMenuFrame(struct Frame* mainFrame){
	struct Frame* mf = initFrame();
	mf->buttons = (struct Button**)malloc(2*sizeof(struct Button*));
	mf->buttons[0] = initMenuButton(5, "All Songs", 0, mf);
	mf->buttons[1] = initMenuButton(35, "Playlists", 1, mf);
	mf->elements = NULL;
	mf->drawFrame = drawMenuFrame;
	mf->button_size = 2;
	mf->mainFrame = mainFrame;
	mf->bg_image = NULL;
	return mf;
}

struct Frame* initActionFrame(struct Frame* mainFrame){
	struct Frame* af = initFrame();
	while((af->bg_image = loadSDImage("GR3.BMP")) == NULL);
	af->buttons = (struct Button**)malloc(5*sizeof(struct Button*));
	af->buttons[0] = initActionButton(0, af);
	af->buttons[0]->range->height = 30;
	af->buttons[0]->range->width = 30;
	while((af->buttons[0]->stats[0] = loadSDImage("PLAY2.BMP")) == NULL); //size 30x30
	while((af->buttons[0]->stats[1] = loadSDImage("PLAY5.BMP")) == NULL);
	af->buttons[1] = initActionButton(1, af);
	while((af->buttons[1]->stats[0] = loadSDImage("STOP.BMP")) == NULL); //size 20x20
	while((af->buttons[1]->stats[1] = loadSDImage("STOP4.BMP")) == NULL); //size 20x20
	af->buttons[2] = initActionButton(2, af);
	while((af->buttons[2]->stats[0] = loadSDImage("PAUSE.BMP")) == NULL);
	while((af->buttons[2]->stats[1] = loadSDImage("PAUSE4.BMP")) == NULL);
	af->buttons[3] = initActionButton(3, af);
	while((af->buttons[3]->stats[0] = loadSDImage("PREV.BMP")) == NULL);
	while((af->buttons[3]->stats[1] = loadSDImage("PREV4.BMP")) == NULL);
	af->buttons[4] = initActionButton(4, af);
	while((af->buttons[4]->stats[0] = loadSDImage("NEXT.BMP")) == NULL);
	while((af->buttons[4]->stats[1] = loadSDImage("NEXT4.BMP")) == NULL);
	af->drawFrame = drawActionFrame;
	af->button_size = 5;
	af->mainFrame = mainFrame;
	return af;
}

struct Frame* initSongPanel(struct Frame* frame){
	int i = 1;
	int init_song_y = 4;
	struct Frame* sp = initFrame();
	sp->buttons = (struct Button**)malloc(51*sizeof(struct Button*));
	sp->drawFrame = drawSongPanel;
	while((sp->bg_image = loadSDImage("AND.BMP")) == NULL);
	sp->mainFrame = frame;
	if (db.num_of_songs > 14){
		sp->button_size = 14; // starts from 1!!
	} else {
		sp->button_size = db.num_of_songs;
	}
	sp->buttons[0] = NULL;
	for (i = 1; i <= sp->button_size; i++){
		sp->buttons[i] = initSongButton(61, init_song_y, db.songs[i]->song_name, i, sp);
		init_song_y = init_song_y+3;
	}
	return sp;
}

struct Frame* initPlaylistPanel(struct Frame* frame){
	int i = 1, j = 0;
	int init_playlist_y = 4;
	struct Frame* pp = initFrame();
	pp->buttons = (struct Button**)malloc(51*sizeof(struct Button*));
	pp->drawFrame = drawPlaylistPanel;
	pp->button_size = db.num_of_lists; // starts from 1
	pp->mainFrame = frame;
	while((pp->bg_image = loadSDImage("AND.BMP")) == NULL);
	pp->buttons[0] = NULL;
	for (i = 1; i <= MAX_LISTS; i++){
		if(j == db.num_of_lists) break;
		if(db.used_list_index[i] != 0) {
			pp->buttons[j+1] = initPlaylistButton(61, init_playlist_y, db.playlists[i]->list_name, pp);
			printf("Playlist %d name is %s\n", i, db.playlists[i]->list_name);
			init_playlist_y += 3;
			j++;
		}
	}
	return pp;
}

/**
 * Draws all elements of mainFrame. All backgrounds are
 * loaded and drawn in this function.
 */
void drawMainFrame(struct Frame* this){
	int i = 0;
	// do not draw the last element (playlist)
	for (i = 0; i < this->element_size - 1; i++){
		this->elements[i]->drawFrame(this->elements[i]);
	}
	// TODO: put all backgrounds in here.
	// make a struct Image array in mainFrame
	//struct Image* bg = loadSDImage("AND.BMP");
	draw_notransparent(241, 13, this->elements[3]->bg_image);
	drawVerticalLine(240, 12, 183, 0xFFFFFF);
}

void drawMenuFrame(struct Frame* this){
	int x1, y1, x2, y2;
	drawHorizontalLine(0, 0, SCREEN_WIDTH-1, 0xFFFFFF);
	drawHorizontalLine(0, 11, SCREEN_WIDTH-1, 0xFFFFFF);
	x1 = 0; y1 = 1;
	x2 = x1 + 10;
	y2 = 11;
	while (x2 <= 320){
		drawBox(x1, y1, x2, y2, 0xe711ce);
		x1+=10;
		x2+=10;
	}
	this->buttons[0]->draw(this->buttons[0]);
	this->buttons[1]->draw(this->buttons[1]);
}

void drawActionFrame(struct Frame* this){
	drawHorizontalLine(0, 195, SCREEN_WIDTH-1, 0xFFFFFF);
	draw_notransparent(0, 196, this->bg_image);
	int i = 0;
	for (i = 0; i < this->button_size; i++){
		this->buttons[i]->draw(this->buttons[i]);
	}
}

void drawSongPanel(struct Frame* this){
	int i = 1;
	for (i = 1; i <= this->button_size; i++){
		this->buttons[i]->draw(this->buttons[i]);
	}
}

void drawPlaylistPanel(struct Frame* this){
	int i = 1;
	for (i = 1; i <= db.num_of_lists; i++){
		this->buttons[i]->draw(this->buttons[i]);
	}
}

void clearSongPanel(){
	int y = SONG_Y_POSITION;
	int i = 0;
	for (i = 0; i < 15; i++){
		alt_up_char_buffer_string(char_buffer, "               ", 61, y);
		y += 3;
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



