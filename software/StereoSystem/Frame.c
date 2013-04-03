#include "Frame.h"

struct Frame* initFrame(){
	struct Frame* f = (struct Frame*)malloc(sizeof(struct Frame));
	f->element_size = 0;
	f->currentPlaylist = 0;
	f->elements = NULL;
	return f;
}

struct Frame* initMainFrame(){
	struct Frame* f = initFrame();
	f->elements = (struct Frame**)malloc(6*sizeof(struct Frame*));
	f->elements[0] = initMenuFrame(f);
	f->elements[1] = initActionFrame(f);
	f->elements[2] = initSongPanel(f);
	f->elements[3] = initPlaylistPanel(f);
	f->elements[4] = initScrollFrame(f);
	f->elements[5] = initVolumeFrame(f);
	f->buttons = NULL;
	f->bg_image = NULL;
	f->mainFrame = NULL;
	f->drawFrame = drawMainFrame;
	f->element_size = 6;
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
	sp->buttons = (struct Button**)malloc(15*sizeof(struct Button*));
	sp->drawFrame = drawSongPanel;
	while((sp->bg_image = loadSDImage("AND2.BMP")) == NULL);
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

void killSongPanel(struct Frame** frame) {
	if(frame == NULL || *frame == NULL) return;
}
// buttons start from 1
struct Frame* initPlaylistPanel(struct Frame* frame){
	int i = 1, j = 0;
	int init_playlist_y = 4;
	struct Frame* pp = initFrame();
	pp->elements = (struct Frame**)malloc(sizeof(struct Frame*)); // to display songs in that list
	pp->element_size = 1;
	pp->elements[0] = NULL;
	pp->buttons = (struct Button**)malloc(30*sizeof(struct Button*));
	pp->drawFrame = drawPlaylistPanel;
	pp->button_size = db.num_of_lists; // starts from 1
	pp->mainFrame = frame;
	while((pp->bg_image = loadSDImage("AND2.BMP")) == NULL);
	pp->buttons[0] = NULL;
	for (i = 1; i <= MAX_LISTS; i++){
		if(j == db.num_of_lists) break;
		if(db.used_list_index[i] != 0) {
			pp->buttons[j+1] = initPlaylistButton(61, init_playlist_y, db.playlists[i]->list_name, i, pp);
			printf("Playlist %d name is %s\n", i, db.playlists[i]->list_name);
			init_playlist_y += 3;
			j++;
		}
	}
	return pp;
}

struct Frame* initScrollFrame(struct Frame* this){
	struct Frame* sf = initFrame();
	sf->buttons = (struct Button**)malloc(2*sizeof(struct Button*));
	sf->bg_image = NULL;
	sf->button_size = 2;
	sf->buttons[0] = initScrollButton(265, 184, 0, this);
	while((sf->buttons[0]->stats[0] = loadSDImage("UP1.BMP")) == NULL);
	sf->buttons[1] = initScrollButton(285, 184, 1, this);
	while((sf->buttons[1]->stats[0] = loadSDImage("DOWN1.BMP")) == NULL);
	sf->drawFrame = drawScrollFrame;
	sf->mainFrame = this;
	return sf;
}

struct Frame* initVolumeFrame(struct Frame* this){
	struct Frame* vf = initFrame();
	vf->buttons = (struct Button**)malloc(2*sizeof(struct Button*));
	vf->buttons[0] = initVolumeButton(5, 198, 0, this);
	while((vf->buttons[0]->stats[0] = loadSDImage("VUP.BMP")) == NULL);
	vf->buttons[0]->stats[1] = vf->buttons[0]->stats[0];
	vf->buttons[1] = initVolumeButton(5, 218, 1, this);
	while((vf->buttons[1]->stats[0] = loadSDImage("VDOWN.BMP")) == NULL);
	vf->buttons[1]->stats[1] = vf->buttons[1]->stats[0];
	vf->bg_image = NULL;
	vf->button_size = 2;
	vf->drawFrame = drawVolumeFrame;
	vf->mainFrame = this;
	return vf;
}
//button starts from 1
struct Frame* initSongInListPanel(struct Frame* f, int list_id){
	struct Frame* psp = initFrame();
	if (db.playlists[list_id] == NULL) {return NULL;}
	// TODO: what if the list does not have any songs
	// song number is 0?
	// up and down is not implemented yet
	// max of 14 songs per list for now
	if (db.playlists[list_id]->num_of_songs > 14){
		psp->button_size = 14;
	} else {
		psp->button_size = db.playlists[list_id]->num_of_songs;
	}
	psp->buttons = (struct Button**)malloc((psp->button_size+1)*sizeof(struct Button*));
	int i = 0;
	int y = 4;
	psp->buttons[0] = NULL;
	for (i = 1; i <= psp->button_size; i++){
		psp->buttons[i] = initSongButton(61, y, db.songs[db.index_list_order[list_id][i]]->song_name, db.index_list_order[list_id][i], psp);
		//psp->buttons[i] = initSongButton(61, y, "h", 1, psp);
		y = y + 3;
	}
	psp->mainFrame = f;
	psp->drawFrame = drawSongInListPanel;
	while((psp->bg_image = loadSDImage("AND2.BMP")) == NULL);
	return psp;
}

void killSongInListPanel(struct Frame** this) {
	if(this == NULL || *this == NULL) return;
	int i;
	for(i = 1; i <= (*this)->button_size; i++) {
		killSongButton(&((*this)->buttons[i]));
	} killImage((*this)->bg_image);
	(*this)->mainFrame = NULL;
	(*this)->drawFrame = NULL;
	free((*this)->buttons);
	(*this)->buttons = NULL;
	free((*this));
	(*this) = NULL;
}
/**
 * Draws all elements of mainFrame. All backgrounds are
 * loaded and drawn in this function.
 */
void drawMainFrame(struct Frame* this){
	int i = 0;
	// do not draw the last element (playlist)
	for (i = 0; i < this->element_size; i++){
		if (i == 3) {continue;}
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
	int total_lists = 0;
	if(db.num_of_lists > 14)
		total_lists = 14;
	else
		total_lists = db.num_of_lists;
	for (i = 1; i <= total_lists; i++){
		this->buttons[i]->draw(this->buttons[i]);
	}
}

void drawScrollFrame(struct Frame* this){
	int i;
	/*int y = 184;
	for (i = 0; i < 11; i++, y++){
	drawHorizontalLine(241, y, 78, 0xFFFFFF);
	}*/
	this->buttons[0]->draw(this->buttons[0]);
	this->buttons[1]->draw(this->buttons[1]);
	int y = 184;
	for (i = 0; i < 10; i++, y++){
	drawHorizontalLine(241, y, 24, 0xFFFFFF);
	drawHorizontalLine(275, y, 10, 0xFFFFFF);
	drawHorizontalLine(295, y, 24, 0xFFFFFF);
	}
	drawHorizontalLine(241, 194, 78, 0xFFFFFF);
}

void drawVolumeFrame(struct Frame* this){
	this->buttons[0]->draw(this->buttons[0]);
	this->buttons[1]->draw(this->buttons[1]);
	alt_up_char_buffer_string(char_buffer, "     ", 7, 54);
	alt_up_char_buffer_string(char_buffer, "100", 8, 54); //draw 100 initially, it will get updated
}

void drawSongInListPanel(struct Frame* songInListPanelFrame){
	draw_notransparent(241, 13, songInListPanelFrame->bg_image);
	clearSongPanel();
	//mouse->frame->currentPanel = 2;
	int i = 0;
	if (songInListPanelFrame->button_size > 0){
		for (i = 1; i <= songInListPanelFrame->button_size; i++){
			songInListPanelFrame->buttons[i]->draw(songInListPanelFrame->buttons[i]);
		}
	}
	//alt_up_char_buffer_string(char_buffer, "List is clicked", 61, 4);
}

void clearSongPanel(){
	int y = SONG_Y_POSITION;
	int i = 0;
	for (i = 0; i < 15; i++){
		alt_up_char_buffer_string(char_buffer, "               ", 61, y);
		y += 3;
	}
}

/**
 * Has to be called after mouse gets initialized
 */
void drawAllSongs(){

	clearSongPanel();
	draw_notransparent(241, 13, mouse->frame->elements[2]->bg_image);
	int i = 0;
	int y_pos = 4;
	int totalSong = db.num_of_songs;
	// up and down is not implemented yet
	// just show from ID 1 to 14
	if (db.num_of_songs > 14) {
		totalSong = 14;
	}
	for (i = 1; i <= totalSong; i++){
		alt_up_char_buffer_string(char_buffer, db.songs[i]->song_name, 61, y_pos);
		y_pos = y_pos + 3;
	}
	mouse->frame->currentPanel = 0;
}

void drawAllLists(){
	clearSongPanel();
	draw_notransparent(241, 13, mouse->frame->elements[3]->bg_image);
	int i = 0, j = 0;
	int y_pos = 4;
	int totalList = db.num_of_lists;
	// up and down is not implemented yet
	// just show from ID 1 to 14
	if (db.num_of_lists > 14){
		totalList = 14;
	}
	for (i = 1; i < MAX_LISTS; i++){
		if(j >= totalList) break;
		if(db.used_list_index[i] == 1) {
			alt_up_char_buffer_string(char_buffer, db.playlists[i]->list_name, 61, y_pos);
			y_pos = y_pos + 3;
			j++;
		}
	}
	mouse->frame->currentPanel = 1;
}

void drawAllSongsInList(int list_id){
	if (mouse->frame->elements[3]->buttons[list_id]->id != list_id){
		printf("No playlist with such ID\n");
		return;
	}
	// TODO: call kill elements[0] on this playlist here
	if (mouse->frame->elements[3]->elements[0] != NULL){
		killSongInListPanel(&(mouse->frame->elements[3]->elements[0]));
	}
	clearSongPanel();
	playlistButtonCollide(mouse->frame->elements[3]->buttons[list_id]);
	draw_notransparent(241, 13, mouse->frame->elements[3]->bg_image);
	mouse->frame->currentPanel = 2;
}

