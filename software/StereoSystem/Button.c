#include "Button.h"

struct Button* initButton(){
	struct Button* b = (struct Button*)malloc(sizeof(struct Button));
	b->name = NULL;
	b->range = NULL;
	b->x_pos = 0;
	b->y_pos = 0;
	b->buttonType = 3;
	b->collide = NULL;
	b->draw = NULL;
	b->Panel = NULL;
	b->isClicked = 0;
	b->startAnimate = 0;
	b->frame = 0;
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
	b->buttonType = menu;
	return b;
}

struct Button* initSongButton(int x, int y, char* name, int id, struct Frame* panel){
	struct Button* sb = initButton();
	sb->name = name;
	sb->Panel = panel;
	sb->range = initRange(x, y, 79, 10);
	sb->x_pos = x;
	sb->y_pos = y;
	sb->draw = drawTxtButton;
	sb->collide = songButtonCollide;
	sb->id = id;
	return sb;
}

void killSongButton(struct Button** this) {
	if(this == NULL || (*this) == NULL) return;
	free((*this)->range);
	(*this)->range = NULL;
	(*this)->Panel = NULL;
	(*this)->name = NULL;
	free((*this));
	*this = NULL;
}

struct Button* initPlaylistButton(int x, int y, char* name, int id, struct Frame* panel){
	struct Button* pb = initButton();
	pb->name = name;
	pb->x_pos = x;
	pb->y_pos = y;
	pb->draw = drawTxtButton;
	pb->Panel = panel;
	pb->range = initRange(x, y, 79, 10);
	pb->collide = playlistButtonCollide;
	pb->id = id;
	return pb;
}

// action button type - used for drawing position later
// 0:PLAY, 1:STOP, 2:PAUSE, 3:PREV, 4:NEXT
struct Button* initActionButton(int type, struct Frame* f){
	struct Button* ab = initButton();
	ab->x_pos = getXActionBtn(type);
	ab->y_pos = getYActionBtn(type);
	ab->range = initRange(ab->x_pos, ab->y_pos, 20, 20);
	ab->type = type;
	ab->Panel = f;
	ab->draw = drawActionButton;
	ab->collide = actionButtonCollide;
	ab->buttonType = action;
	return ab;
}

struct Button* initVolumeButton(int x, int y, int type, struct Frame* panel){
	struct Button* vb = initButton();
	vb->x_pos = x;
	vb->y_pos = y;
	vb->range = initRange(vb->x_pos, vb->y_pos, 20, 18);
	vb->type = type;
	vb->Panel = panel;
	vb->draw = drawActionButton;
	vb->collide = volumeButtonCollide;
	return vb;
}

void drawTxtButton(struct Button* this){
	alt_up_char_buffer_string(char_buffer, this->name, this->x_pos, this->y_pos);
}
void drawActionButton(struct Button* this){
	draw(this->x_pos, this->y_pos, this->stats[0]);
}

void menuButtonCollide(struct Button* this){
	// 0:ALLSONGS, 1:PLAYLISTS
	draw_notransparent(241, 13, this->Panel->mainFrame->elements[2]->bg_image);
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
	this->Panel->mainFrame->currentPanel = this->type;
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

void volumeButtonCollide(struct Button* this){
	//printf("db current song id is %d", db.curr_song_id);
	int song_id = db.curr_song_id;
	int vol = db.songs[song_id]->volume;
	if (song_id == 0){return;}
	if (db.songs[song_id]->sound == NULL){return;}
	switch(this->type){
	case 0:
		if(vol >= 100) return;
		syncSetVol(song_id, db.songs[song_id]->volume + 1);
		break;
	case 1:
		if(vol <= 0) return;
		syncSetVol(song_id, db.songs[song_id]->volume - 1);
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
	if (mouse->frame->currentPanel == 0 || (mouse->frame->currentPanel == 2 && db.playlists[db.curr_playlist_id]->num_of_songs > 0)){
		if (db.curr_song_id == 0 || db.songs[db.curr_song_id]->sound == NULL){
			syncPlay(1, db.songs[1]->volume, 0);
			highlightButton(this->Panel->mainFrame->elements[2]->buttons[1]);
			printf("Play button is clicked\n");
			updateVolumeValue(1);
		} else {
			syncPlay(db.curr_song_id, db.songs[db.curr_song_id]->volume, convertToMS(db.songs[db.curr_song_id]->sound->position));
			highlightButton(this->Panel->mainFrame->elements[2]->buttons[db.curr_song_id]);
			printf("Play button is clicked\n");
			updateVolumeValue(db.curr_song_id);
		}
	}
}

void pauseButtonCollide(struct Button* this){
	if (mouse->frame->currentPanel != 1){
		syncPause(db.curr_song_id);
		printf("Pause button is clicked.\n");
		updateVolumeValue(db.curr_song_id);
	}
}

void stopButtonCollide(struct Button* this){
	if (mouse->frame->currentPanel != 1){
		syncStop();
		printf("Stop button is clicked\n");
		updateVolumeValue(db.curr_song_id);
	}
}

void prevButtonCollide(struct Button* this){
	if(db.curr_song_id <= 1 || mouse->frame->currentPanel == 1) return;
	syncPrev(db.curr_song_id);
	updateVolumeValue(db.curr_song_id - 1);
	//if(this == NULL) return;
	//highlightButton(this->Panel->mainFrame->elements[2]->buttons[db.curr_song_id-1]);
	printf("Prev button is clicked.\n");
}

void nextButtonCollide(struct Button* this){
	if(db.curr_song_id >= db.num_of_songs || db.curr_song_id == 0) return;
	syncNext(db.curr_song_id);
	updateVolumeValue(db.curr_song_id + 1);
	//if(this == NULL) return;
	//highlightButton(this->Panel->mainFrame->elements[2]->buttons[db.curr_song_id+1]);
	printf("Next button is clicked.\n");
}

void songButtonCollide(struct Button* this){
	if (mouse->frame->currentPanel == 0){
		playSongsFromSongPanel(this->id, db.songs[this->id]->volume, db.songs[this->id]->pos);
	} else if (mouse->frame->currentPanel == 2){
		db.curr_playlist_id = mouse->frame->elements[3]->currentPlaylist;
		highlightSongInList(db.curr_playlist_id, this->id);
		syncStop();
		syncPlay(this->id, db.songs[this->id]->volume, 0);
		updateVolumeValue(this->id);
	}

}

void highlightButton(struct Button* this){
	draw_notransparent(241, 13, this->Panel->bg_image);
	int i = 0;
	int y = 4* this->y_pos - 2;
	int x = 4* this->x_pos - 3;
	for (i = 0; i < 10; i++, y++){
		drawHorizontalLine(x, y, 76, 0xe711ce);
	}

}

/**
 * This function will highlight a song in given list,
 * provided that the LIST is opened.
 * If the list is not opened, it will not do anything.
 */
void highlightSongInList(int list_id, int song_id){
	if (mouse->frame->currentPanel != 2){return;}
	if (mouse->frame->elements[3]->currentPlaylist != list_id) {return;}
	if (!songButtonExistsInList(list_id, song_id)){
		printf("No such song button on the list panel VGA.\n");
		return;
	}
	draw_notransparent(241, 13, mouse->frame->elements[2]->bg_image);
	// TODO: check if the playlist element is the right one? not sure
	highlightButton(mouse->frame->elements[3]->elements[0]->buttons[db.index_list_song[list_id][song_id]]);
}

/**
 * This function checks if the song BUTTON exists, not if the song exists
 * in the playlist. It is possible where song exists at order 17,
 * but we only have 14 buttons.
 */
bool songButtonExistsInList(int list_id, int song_id){
	int i = 0;
	int totalSong = db.playlists[list_id]->num_of_songs;
	if (totalSong > 14){
		totalSong = 14;
	}
	for (i = 1; i <= totalSong; i++){
		if (db.index_list_order[list_id][i] == song_id){
			return true;
		}
	}
	return false;
}

void playlistButtonCollide(struct Button* this){
	//db.curr_playlist_id = this->id;
	mouse->frame->elements[3]->currentPlaylist = this->id;
	highlightButton(this);
	this->Panel->mainFrame->currentPanel = 2;
	mouse->frame->currentPanel = 2;
	//mouse->frame->elements[3]->elements[0] = initSongInListPanel(mouse->frame->mainFrame, list_id);
	//mouse->frame->elements[3]->drawFrame(mouse->frame->elements[3]->elements[0]);
	this->Panel->elements[0] = initSongInListPanel(this->Panel->mainFrame, this->id);
	this->Panel->elements[0]->drawFrame(this->Panel->elements[0]);
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

void animateButton(struct Button* this, int stats_index){
	draw(getXActionBtn(this->type), getYActionBtn(this->type), this->stats[stats_index]);
}

void updateVolumeValue(int song_id){
	char temp[4];
	if (song_id == 0){
		song_id = 1;
	}
	memset(temp, 0 , sizeof(temp)/sizeof(temp[0]));
	sprintf(temp, "%d", db.songs[song_id]->volume);
	alt_up_char_buffer_string(char_buffer, "                    ", 7, 54);
	alt_up_char_buffer_string(char_buffer, temp, 8, 54);
}

/**
 * Highlight + play song from All Songs Panel only.
 */
void playSongsFromSongPanel(int song_id, int vol, int pos){
	syncStop();
	syncSelectList(0);
	syncPlay(song_id, vol, pos);
	if (querySongButtonFromID(song_id) != NULL && mouse->frame->currentPanel == 0){
		highlightButton(querySongButtonFromID(song_id));
	}
	updateVolumeValue(song_id);
}

struct Button* querySongButtonFromID(int song_id){
	int i = 1;
	if (song_id > 14){
		return NULL;
	}
	for (i = 1; i < mouse->frame->elements[2]->button_size; i++){
		if (mouse->frame->elements[2]->buttons[i]->id == song_id){
			return mouse->frame->elements[2]->buttons[i];
		}
	}
	return NULL;
}


struct Button* getPlaylistButtonFromID(int list_id){
	int i = 1;
	for (i = 1; i <= mouse->frame->elements[3]->button_size; i++){
		if (mouse->frame->elements[3]->buttons[i]->id == list_id){
			return mouse->frame->elements[3]->buttons[i];
		}
	}
	printf("list id is %d and our playlist button size is %d\n", list_id, mouse->frame->elements[3]->button_size);
	return NULL;
}

void highlightSongWithID(int id){
	if (querySongButtonFromID(id) != NULL){
		highlightButton(querySongButtonFromID(id));
	}
}


