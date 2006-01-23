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
 * Player wants to double?
 */
struct cmd_double
{
};

struct reply_double
{
    unsigned char want_double;
};

/**
 * Player wants to play?
 */
struct cmd_wantplay
{
};

struct reply_wantplay
{
    unsigned char want_play;
};

/** 
 * Info of game currently played
 */
struct cmd_gameinfo
{
    unsigned char player;
    unsigned char type;       // type of game if set, rules::type() is used
    unsigned char color;      // color of game if set, rules::color() is used
};

/**
 * Info when player changes name. Sent to all players
 */
struct cmd_playerinfo
{
    int player;
    char name[PLAYER_NAME_LENGTH];
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
 * Info if player wants to play
 */
struct cmd_playerwantsplay
{
    unsigned char player;
};

/** 
 * Info when game is over.
 */
struct cmd_gamefinished
{
    unsigned char doublecnt;            // How often doubled?
    unsigned char gametype;             // What kind of game?
    unsigned char values_winner;        // How many points for winners?
    unsigned char values_looser;        // How many points for loosers?
    unsigned char winners[4];           // 1 if player wins, 0 if player losses
    unsigned char worth;                // How much is game worth?
};

/**
 * What game wants player to play?
 */
struct cmd_requestgame
{
};

struct reply_requestgame
{
    unsigned char type;
    unsigned char color;
};

/** 
 * Id's for all commands
 */
typedef enum command_type 
{
    CMD_CONNECT = 1, 
    CMD_GIVECARD = 2, 
    CMD_PLAY = 3, 
    CMD_DOUBLE = 4, 
    CMD_WANTPLAY = 5,
    CMD_GAMEINFO = 6, 
    CMD_PLAYERDOUBLES = 7, 
    CMD_PLAYERINFO = 8,
    CMD_CARDPLAYED = 9, 
    CMD_PLAYERWANTSPLAY = 10,
    CMD_GAMEFINISHED = 11,
    CMD_REQUESTGAME = 12,
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
        cmd_wantplay wantplay;
        cmd_gameinfo gameinfo;
        cmd_playerdoubles playerdoubles;
        cmd_playerinfo playerinfo;
        cmd_cardplayed cardplayed;
        cmd_playerwantsplay playerwantsplay;
        cmd_gamefinished gamefinished;
        cmd_requestgame requestgame;
    };
};

/** 
 * Generator functions for commands
 */

void createCmdConnect(socket_cmd *cmd, const char *name);
void createCmdGiveCard(socket_cmd *cmd, card *card);
void createCmdPlay(socket_cmd *cmd);
void createCmdDouble(socket_cmd *cmd);
void createCmdWantPlay(socket_cmd *cmd);
void createCmdGameInfo(socket_cmd *cmd, int player, rules::gametype type, card::cardcolor color);
void createCmdPlayerDoubles(socket_cmd *cmd, int player);
void createCmdPlayerInfo(socket_cmd *cmd, int player, const char *name);
void createCmdCardPlayed(socket_cmd *cmd, int player, card *card);
void createCmdPlayerWantsPlay(socket_cmd *cmd, int player);
void createCmdRequestGame(socket_cmd *cmd);
/*
 * This function is an exception: it does only fill the socket_cmd header
 */
void createCmdGameFinished(socket_cmd *cmd);

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

