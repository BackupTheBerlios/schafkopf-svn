
#include "game.h"

#include "communication.h"

#include <unistd.h>
#include <sys/socket.h>
#include <sstream>

using namespace std;

game::game(int socket)
{
    this->socket = socket;
    initialized_players = 0;
}

void game::connect()
{
    int sock;
    ostringstream str;

    while(initialized_players < PLAYERS)
    {
        sock = wait_for_request();
        if( process_player_connect(sock) )
            continue;
        str.str("");
        players[ initialized_players ].socket = sock; 
        str << "Player " << initialized_players+1;
        players[ initialized_players ].name = str.str();
    }
}

void game::play()
{
    if( initialized_players < PLAYERS )
        throw game::not_initialized();
}

int game::wait_for_request()
{
    fd_set rd;
    int max_fd;
    int i;

    FD_ZERO(&rd);
    FD_SET(socket, &rd);
    max_fd = socket;
    for(i=0; i<initialized_players; i++)
    {
        if( player_sockets[i] > max_fd )
            max_fd = player_sockets[i];
        FD_SET( player_sockets[i], &rd );
    }
    
    for(;;)
    {
        i = select(max_fd + 1, &rd, NULL, NULL, NULL)
        if( i == 0 )
            continue;
        if( i < 0 )
            throw connection_failed();

        if( FD_ISSET( socket, &rd ) )
            return accept( socket, NULL, NULL );
        for(i=0; i<initialized_players; i++)
        {
            if( FD_ISSET( player_sockets[i], &rd ) )
                return player_sockets[i];
        }
    }
}

bool game::process_player_connect(int sock)
{
    int i;
    socket_cmd cmd;
    
    for( i=0; i<initialized_players; i++)
    {
        if( player_sockets[i] == sock )
        {
            receive_command(sock, &cmd);
            if( cmd->type == CMD_CONNECT )
            {
                player_sockets[i].name = cmd->connect.name;
                return true;
            }
        }
    }
    return false;
}

