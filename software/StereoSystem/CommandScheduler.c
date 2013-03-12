/*
 * CommandScheduler.c
 *
 *  Created on: 2013-03-08
 *      Author: danny
 */

#include "CommandScheduler.h"

struct CmdScheduler* initCmdScheduler() {
	struct CmdScheduler* this = (struct CmdScheduler*)malloc(sizeof(struct CmdScheduler));
	this->cmds = initQueue();
	return this;
}

/*
 * Add command to the scheduler
 */
void addCmd(struct CmdScheduler* this, struct Command* cmd) {
	enqueue(this->cmds, (void*)cmd);
}
/*
 * This function schedule a command in the waiting list to be executed
 */
void cmdProcessing(struct CmdScheduler* this) {
	if(this->cmds->size <= 0 || this == NULL) return;
	int num_cmds = this->cmds->size;
	struct Command* cmd;
	while(num_cmds > 0) {
		cmd = dequeue(this->cmds);
		switch(cmd->cmd_index) {
		case 1:
			play(cmd->parameters[0], atoi(cmd->parameters[1]));
			break;
		case 2:
			pause();
			break;
		case 3:
			stop();
			break;
		case 4:
			setVolume(atoi(cmd->parameters[0]));
			break;
		case 5:
			seek(atoi(cmd->parameters[0]));
			break;
		case 6:
			next();
			break;
		case 7:
			prev();
			break;
		case 8:
			createPlaylist(cmd->parameters[0]);
			break;
		case 9:
			modifyPlaylistName(atoi(cmd->parameters[0]), cmd->parameters[1]);
			break;
		case 10:
			shuffle(atoi(cmd->parameters[0]));
			break;
		case 11:
			removeList(atoi(cmd->parameters[0]));
			break;
		case 12:
			play_playlist(atoi(cmd->parameters[0]));
			break;
		case 13:
			repeatPlaylist(atoi(cmd->parameters[0]));
			break;
		default:
			break;
		}
		killCmd(&cmd);
		num_cmds--;
	}
}