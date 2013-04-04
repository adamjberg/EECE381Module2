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
			play(atoi(cmd->parameters[0]), atoi(cmd->parameters[1]), atoi(cmd->parameters[2]));
			break;
		case 2:
			pause(atoi(cmd->parameters[0]));
			break;
		case 3:
			stop();
			break;
		case 4:
			setVolume(atoi(cmd->parameters[0]), atoi(cmd->parameters[1]));
			break;
		case 5:
			seek(atoi(cmd->parameters[0]));
			break;
		case 6:
			next(atoi(cmd->parameters[0]));
			break;
		case 7:
			prev(atoi(cmd->parameters[0]));
			break;
		case 8:
			createPlaylist(cmd->parameters[0]);
			break;
		case 9:
			//empty command for DE2
			break;
		case 10:
			//empty command for DE2
			break;
		case 11:
			selectList(atoi(cmd->parameters[0]));
			break;
		case 12:
			//empty command for DE2
			break;
		case 13:
			addSongToList(atoi(cmd->parameters[0]), atoi(cmd->parameters[1]));
			break;
		case 14:
			//empty
			break;
		case 15:
			removeSongFromList(atoi(cmd->parameters[0]), atoi(cmd->parameters[1]));
			break;
		case 16:
			//empty
			break;
		case 17:
			repeatList(atoi(cmd->parameters[0]));
			break;
		case 18:
			modifyListName(atoi(cmd->parameters[0]), cmd->parameters[1]);
			break;
		case 19:
			removeList(atoi(cmd->parameters[0]));
			break;
		case 20:
			playSongFromAllSongs(atoi(cmd->parameters[0]), atoi(cmd->parameters[1]), atoi(cmd->parameters[2]));
			break;
		case 21:
			openAllSongPanel();
			break;
		case 22:
			openPlaylistsPanel();
			break;
		case 23:
			openSongsFromList(atoi(cmd->parameters[0]));
			break;
		case 24:
			playSongFromList(atoi(cmd->parameters[0]), atoi(cmd->parameters[1]));
			break;
		case 25:
			setPitch(atoi(cmd->parameters[0]), atoi(cmd->parameters[1]), atoi(cmd->parameters[2]));
			break;
		case 26:
			setPlaybackSpeed(atoi(cmd->parameters[0]), atoi(cmd->parameters[1]));
			break;
		case 27:
			reloadSongById(atoi(cmd->parameters[0]));
			break;
		default:
			break;
		}
		killCmd(&cmd);
		num_cmds--;
	}
}
