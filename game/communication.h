#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include "card.h"
#include "rules.h"

#define PLAYER_NAME_LENGTH 30

/**
 * Command for player info. A player should send it after connect
 */
struct cmd_connect
{
    char name[PLAYER_NAME_LENGTH];
};

/**
 * Command sent from game to player at
 * the beginning of the game to give cards to the player
 */
struct cmd_givecard
{
    unsigned char cardtype;
    unsigned char cardcolor;
};

/**
 * Request for player card.
 * Command must return the card, the player whants to play
 */
struct cmd_play
{
};

struct reply_play
{
    unsigned char type;
    unsigned char color;
};

/** 
 * Player whants to double?
 */
struct cmd_double
{
};

struct reply_double
{
    unsigned char want_double;
};

/** 
 * Info of game currently played
 */
struct cmd_gameinfo
{
    unsigned char type;       // type of game if set, rules::type() is used
    unsigned char color;      // color of game if set, rules::color() is used
};

/**
 * Info if a player doubles
 */
struct cmd_playerdoubles
{
    unsigned char player;
};

/**
 * Info if a player plays a card
 */
struct cmd_cardplayed
{
    unsigned char player;
    unsigned char type;
    unsigned char color;
};

/**
 * Info if a player connected
 */
typedef cmd_connect cmd_playerconnected;


/** 
 * Id's for all commands
 */
typedef enum command_type 
{
    CMD_CONNECT = 1, 
    CMD_GIVECARD = 2, 
    CMD_PLAY = 3, 
    CMD_DOUBLE = 4, 
    CMD_GAMEINFO = 5, 
    CMD_PLAYERDOUBLES = 6, 
    CMD_CARDPLAYED = 7, 
    CMD_PLAYERCONNECTED = 8 
};

/**
 * Structure for all commands over socket
 * type is type of command ( see enum command_type )
 * replycnt specifies the number of bytes to be returned. 
 * Return values are always of type unsigned char
 */
struct socket_cmd
{
    unsigned char type;
    unsigned char replycnt;
    union
    {
        cmd_connect connect;
        cmd_givecard givecard;
        cmd_play play;
        cmd_double doublecmd;
        cmd_gameinfo gameinfo;
        cmd_playerdoubles playerdoubles;
        cmd_cardplayed cardplayed;
        cmd_playerconnected playerconnected;
    };
};

/** 
 * Generator functions for commands
 */

void createCmdConnect(socket_cmd *cmd, const char *name);
void createCmdGiveCard(socket_cmd *cmd, card *card);
void createCmdPlay(socket_cmd *cmd);
void createCmdDouble(socket_cmd *cmd);
void createCmdGameInfo(socket_cmd *cmd, rules::gametype type, card::cardcolor color);
void createCmdPlayerDoubles(socket_cmd *cmd, int player);
void createCmdCardPlayed(socket_cmd *cmd, int player, card *card);
void createCmdPlayerConnected(socket_cmd *cmd, const char *name);

typedef bool (*socketwaiter)(int socket, int cnt, void *param);

/**
 * If send_waitfunction is set, it is called to wait for reply
 * so communication can be interrupted by user
 */
extern socketwaiter send_waitfunction;

/**
 * If receive_waitfunction is set, it is called to wait for data
 * to be received
 */
socketwaiter receive_waitfunction;

/**
 * Sends a command over a socket
 */
bool send_command(int socket, socket_cmd *cmd, unsigned char *reply=NULL, void *param=NULL);

/**
 * Sends a reply
 */
bool send_reply(int socket, unsigned char *reply, int cnt);

/**
 * Waits for command and receives it
 * \return true if command read was successful, false if not
 */
bool receive_command(int socket, socket_cmd *cmd, void *param=NULL);

#endif

