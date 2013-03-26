/*
 * Cursor.c
 *
 *  Created on: 2013-01-25
 *      Author: Danny
 */

#include "Cursor.h"

struct Cursor* initCursor(int x, int y) {
	struct Cursor* this = (struct Cursor*)malloc(sizeof(struct Cursor));
	this->super = initObject(initRange(x, y, 10, 10), loadSDImage("AR01.BMP"), (void*)this);
	int* image = (int*)malloc(sizeof(int)*100);
	memset(image, 0, sizeof(int)*100);
	this->overlapImg = initImage(image, 0, 10, 10);
	return this;
}

void updateCursor(struct Cursor* this, int x, int y) {
	if((x == this->super->r->x && y == this->super->r->y) || this == NULL) return;
	draw_notransparent(this->super->r->x, this->super->r->y, this->overlapImg);
	setXY(this->super, x, y);
	memOverlap(this);
	draw(this->super->r->x, this->super->r->y, this->super->img);
	/*checkButtonCollision(this);
		if(this->createdObj != NULL) {
			setXY_noBound(this->createdObj, x, y);
			if(this->hit == 0) {
				if(this->super->x > 245) {
					((struct SpaceShip*)(this->createdObj->subObject))->weapon = NULL;
					safefree(&(this->createdObj->subObject));
					//this->createdObj->subObject=NULL;
					killObject(this->createdObj);
					this->createdObj = NULL;
					this->buttonPushed = 0;
					setStats(this->selectedObj, 0);
					drawBox(this->super->x, this->super->y, this->super->x+20, this->super->y+20, 0, this);
					((struct Button*)(this->selectedObj->subObject))->redrawing = true;
					((struct Button*)(this->selectedObj->subObject))->env->redraw = true;
					gamePanel.redrawing = true;
					int i;
					for(i = 0; i < 3; i++)
						gamePanel.elements[i]->redrawing = true;
					this->selectedObj->isSelected = 0;
					this->selectedObj = this->super;
				} else {
					addToEnv(this->env, this->createdObj);
					addObjToCollide(this->collidable, this->createdObj);
					this->createdObj->isShowing = 1;
					this->createdObj = NULL;
					this->buttonPushed = 0;
					setStats(this->selectedObj, 0);
					((struct Button*)(this->selectedObj->subObject))->redrawing = true;
					((struct Button*)(this->selectedObj->subObject))->env->redraw = true;
					this->selectedObj->isSelected = 0;
					this->selectedObj = this->super;
				}
			}
		}
		setXY_noBound(this->super, x, y);*/
}

void memOverlap(struct Cursor* this) {
	if(this->super->r->x < 0 || this->super->r->y < 0 || this->super->r->x > 315 || this->super->r->y > 235) return;
	int i, j;
	for(i = 0; i < 10; i++) {
		for(j = 0; j < 10; j++) {
		    *(this->overlapImg->buffer+i+10*j)=IORD_16DIRECT(pixel_buffer->buffer_start_address, (i+this->super->r->x+(j+this->super->r->y)*320)*2);
		}
	}
}

int getCursorX(struct Cursor* this) {
	return this->super->r->x;
}

int getCursorY(struct Cursor* this) {
	return this->super->r->y;
}
/*
void cursorAnimate(struct animation** curr, int x, int y, int size, int isForward, struct Object* super, struct Cursor* this) {
	//if(x < 0 || y <0 || x > 315 || y > 235) return;
	if(this->buttonPushed == 1) {
		draw_nontransparent(super->animate_prev_x, super->animate_prev_y, this->dragOverlapImg, 20, NULL);
		memOverlap(this);
		dragOverlap(this);
		draw(x,y,this->createdObj->currImg->image,20);
		this->createdObj->currImg = this->createdObj->currImg->next;
		this->createdObj->animate_prev_x = x;
		this->createdObj->animate_prev_y = y;
	} else {
		draw_nontransparent(super->animate_prev_x, super->animate_prev_y, this->overlapImg, 10, NULL);
		memOverlap(this);
		dragOverlap(this);
}
	super->animate_prev_x = x;
	super->animate_prev_y = y;
	draw(this->super->x, this->super->y, this->super->currImg->image, 10);
}*/
/*
void dragOverlap(struct Cursor* this) {
	if(this->super->x < 0 || this->super->y < 0 || this->super->x > 310 || this->super->y > 230) return;
	int i, j;
		for(i = 0; i < 20; i++) {
		    for(j = 0; j < 20; j++) {
		    	*(this->dragOverlapImg+i+20*j)=IORD_16DIRECT(pixel_buffer->buffer_start_address, (i+this->super->x+(j+this->super->y)*320)*2);
		    }
		}
}*/

/*
void collideCursor(void* this, void* obj, struct Collidable* c) {
	if(((struct Cursor*)(((struct Object*)this)->subObject))->hit == 1 && ((struct Object*)this)->x < 250) {
		if(((struct Cursor*)((struct Object*)this)->subObject)->selectedObj->class == button) {
			return;
		}
		((struct Cursor*)(((struct Object*)this)->subObject))->selectedObj->isSelected = 0;
		((struct Cursor*)(((struct Object*)this)->subObject))->selectedObj = (struct Object*)obj;

		if(((struct Object*)obj)->class == ship) {
			((struct Cursor*)(((struct Object*)this)->subObject))->pageButtons[1]->super->collide(((void*)(((struct Cursor*)(((struct Object*)this)->subObject))->pageButtons[1]->super)), this, NULL);
		}

	}
}

void checkButtonCollision(struct Cursor* this) {

	int i;
	if(info.start == 1) {
		for(i = 0; i < 3; i++) {
			if(inBoundary(this->curr_page->buttons[i]->r, this->super)) {
				this->curr_page->buttons[i]->obj1->startAnimate = true;
			} else
				this->curr_page->buttons[i]->obj1->startAnimate = false;
		}
	}
	if(this->hit == 0) {
		for(i = 0; i < this->curr_page->buttons_size; i++){
			this->curr_page->buttons[i]->super->isSelected = 0;
		}
		return;
	}
	if(this->selectedObj->class == button) {
		return;
	}
	this->r->x = this->super->x; this->r->y = this->super->y;
	for(i = 0; i < this->page_button_size; i++) {
		if(isContain(this->pageButtons[i]->r, this->super)) {
			this->selectedObj->isSelected = 0;
			this->selectedObj = this->super;
			this->pageButtons[i]->super->collide(((void*)(this->pageButtons[i]->super)), (void*)(this->super), NULL);
			return;
		}
	}
	if(this->curr_page == NULL) return;
	for(i = 0; i < this->curr_page->buttons_size; i++) {
		if(inBoundary(this->curr_page->buttons[i]->r, this->super)) {
			//collideButton(this, this->curr_page->buttons[i]);
			this->curr_page->buttons[i]->super->collide(((void*)(this->curr_page->buttons[i]->super)), (void*)(this->super), NULL);
			return;
		}
	}
}
void setFrameToCursor(struct Cursor* this, struct Frame* f) {
	this->curr_page = f;
}
void setPanelToCursor(struct Cursor* this, struct Frame* f) {
	int i;
	for(i = 0; i < f->total_elements_size; i++) {
		this->pages[i] = f->elements[i];
		this->page_size++;
	} this->curr_page = f->elements[1];
	for(i = 0; i < f->buttons_size; i++) {
		this->pageButtons[i] = f->buttons[i];
		this->page_button_size++;
	}
}

void clearPanelFromCursor(struct Cursor* this) {
	int i;
	for(i = this->page_size-1; i >= 0; i--) {
		this->pages[i] = NULL;
	} this->curr_page = NULL;
	for(i = this->page_button_size-1; i >= 0; i--) {
		this->pageButtons[i] = NULL;
	}
	this->page_size = this->page_button_size = 0;
	this->selectedObj = this->super;
}
*/
