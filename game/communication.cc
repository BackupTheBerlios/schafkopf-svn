
#include "communication.h"

#include <unistd.h>
#include <string.h>

socketwaiter send_waitfunction = NULL;
socketwaiter receive_waitfunction = NULL;

void createCommandConnect(socket_cmd *cmd, const char *name)
{
    cmd->type = CMD_CONNECT; 
    cmd->replycnt = 0;
    strncpy( cmd->connect.name, name, PLAYER_NAME_LENGTH );
}

void createCommandGiveCard(socket_cmd *cmd, card *card)
{
    cmd->type = CMD_GIVECARD;
    cmd->replycnt = 0;
    cmd->givecard.cardtype = (unsigned char) card->type();
    cmd->givecard.cardcolor = (unsigned char) card->color();
}

void createCommandPlay(socket_cmd *cmd)
{
    int i;
    
    cmd->type = CMD_PLAY;
    cmd->replycnt = sizeof(reply_play);
}

void createCommandDouble(socket_cmd *cmd)
{
    cmd->type = CMD_DOUBLE;
    cmd->replycnt = sizeof(reply_double);
}

void createCmdGameInfo(socket_cmd *cmd, rules::gametype type, card::cardcolor color)
{
    cmd->type = CMD_GAMEINFO;
    cmd->replycnt = 0;
    cmd->gameinfo.type = (unsigned char) type;
    cmd->gameinfo.color = (unsigned char) color; 
}

void createCmdPlayerDoubles(socket_cmd *cmd, int player)
{
    cmd->type = CMD_PLAYERDOUBLES;
    cmd->replycnt = 0;
    cmd->playerdoubles.player = (unsigned char) player;
}

void createCmdCardPlayed(socket_cmd *cmd, int player, card *card)
{
    cmd->type = CMD_CARDPLAYED;
    cmd->replycnt = 0;
    cmd->cardplayed.player = (unsigned char) player;
    cmd->cardplayed.type = (unsigned char) card->type();
    cmd->cardplayed.color = (unsigned char) card->color();
}

void createCmdPlayerConnected(socket_cmd *cmd, const char *name)
{
    cmd->type = CMD_PLAYERCONNECTED;
    cmd->replycnt = 0;
    strcpy(cmd->playerconnected.name, name);
}

bool send_command(int socket, socket_cmd *cmd, unsigned char *reply, void *param)
{
    if( write(socket, cmd, sizeof(socket_cmd)) != sizeof(socket_cmd) )
        return false;
    if(cmd->replycnt)
    {
        if(send_waitfunction)
        {
            if(!(*send_waitfunction)(socket, cmd->replycnt, param))
                return false;
        }
        if( read(socket, reply, cmd->replycnt) != cmd->replycnt )
            return false;
    }
    return true;
}

bool send_reply(int socket, unsigned char *reply, int cnt)
{
    if( write(socket, reply, cnt) != cnt )
        return false;
    return true;
}

bool receive_command(int socket, socket_cmd *cmd, void *param)
{
    if(receive_waitfunction)
    {
        if(!(*receive_waitfunction)(socket, sizeof(cmd), param))
            return false;
    }
    if( read(socket, cmd, sizeof(cmd) ) != sizeof(cmd) )
        return false;
    return true;
}

