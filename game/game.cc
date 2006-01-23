
#include "game.h"

#include "communication.h"

#include <unistd.h>
#include <sys/socket.h>
#include <sstream>

#define IDX_REL(p) ((p+round) % PLAYERS)
#define PLAYER_REL(p) players[ IDX_REL(p) ]

using namespace std;

game::game(int socket)
{
    this->socket = socket;
    initialized_players = 0;
    round = 0;
}

void game::connect()
{
    int sock;
    socket_cmd cmd;
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
        createCmdPlayerInfo( &cmd, initialized_players, str.str().c_str() );
        initialized_players++;
        broadcast( &cmd );
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
        if( players[i].socket > max_fd )
            max_fd = players[i].socket;
        FD_SET( players[i].socket, &rd );
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
            if( FD_ISSET( players[i].socket, &rd ) )
                return players[i].socket;
        }
    }
}

bool game::process_player_connect(int sock)
{
    int i;
    socket_cmd cmd;
    
    for( i=0; i<initialized_players; i++)
    {
        if( players[i].socket == sock )
        {
            receive_command(sock, &cmd);
            if( cmd->type == CMD_CONNECT )
            {
                players[i].name = cmd->connect.name;
                createCmdPlayerInfo(&cmd, i, players[i].name);
                broadcast(&cmd);
                return true;
            }
        }
    }
    return false;
}
        
void game::broadcast(socket_cmd *cmd)
{
    int i;
    
    for( i=0; i<initialized_players; i++)
    {
        if(!send_command( players[i].socket, cmd ))
            throw connection_failed();
    }
}

void game::reset()
{
    int player;

    for(player=0; player<PLAYERS; player++)
    {
        players[player].playing = false;
        players[player].doubled = 0;
        players[player].cards.clear();
    }
}

void game::give_cards(int num_cards, cardlist &cards)
{
    int i, player;
    socket_cmd cmd;

    for(player=0, i=0; player<PLAYERS; player++)
    {
        for(; i<num_cards; i++)
        {
            createCmdGiveCard(&cmd, &cards[i]);
            if(!send_command( PLAYER_REL(player).socket, &cmd))
                throw connection_failed(); 
        }
    }
    cards.erase( card.begin(), cards.begin()[i] );
}

void game::ask_double()
{
    int player;
    socket_cmd cmd;
    reply_double rep; 

    for(player=0; player<PLAYERS; player++)
    {
        if( PLAYER_REL(player).playing )
            continue;
        createCmdDouble(&cmd);
        if(!send_command( PLAYER_REL(player).socket, &cmd, &rep))
            throw connection_failed();
        if( rep->doublecmd.want_double )
        {
            PLAYER_REL(player).doubled++;
            createCmdPlayerDoubles( &cmd, IDX_REL(player) ); 
            broadcast( &cmd );
        }
    }
}

void game::ask_game()
{
    int player;
    bool wants_play[4];
    socket_cmd cmd;
    rules::gametype gt;
    card::cardcolor gc;
    int playing;
    reply_wantplay rep_wantplay;
    reply_requestgame rep_game;

    memset(wants_play, 0, 4*sizeof(bool));
    gt = rules::UNSET_GAME;
    gc = card::NOCOLOR;
    playing = -1;
    
    for(player=0; player<PLAYERS; player++)
    {
        createCmdWantPlay(&cmd);
        if( !send_command( PLAYER_REL(player).socket, &cmd, &rep_wantplay ) )
            throw connection_failed();
        if(rep->wants_play.want_play)
            want_play[IDX_REL(player)] = true;
        createCmdPlayerWantsPlay(&cmd, IDX_REL(player));
        broadcast(&cmd);
    }
    
    createCmdRequestGame(&cmd);
    for(player=0; player<PLAYERS; player++)
    {
        if(!wants_play[IDX_REL(player)])
            continue;
        if( !send_command( PLAYER_REL(player).socket, &cmd, &rep_game ) )
            throw connection_failed();
        if(rep_game.type > gt && rep_game.type <= rules::SOLO)
        {
            gt = (rules::gametype) rep_game.type;
            gc = (card::cardcolor) rep_game.color;
            playing = IDX_REL(player);
        }
    }
    init_playing(playing, gt, gc);
    createCmdGameInfo(&cmd, playing, gt, gc);
    broadcast(&cmd);
}
        
void game::init_playing(int player, rules::gametype t, card::cardcolor c)
{
    cardlist::iterator it;
    int i;
    
    players[player].playing = true;
    rules.type(t);
    rules.color(c);

    if(t == rules::RUFSPIEL)
    {
        for(i=0;i<PLAYERS;i++)
        {
            for( it=players[i].cards.begin(); it!=players[i].cards.end(); it++ )
            {
                if( it->type() == card::SAU && it->color==c )
                {
                    players[i].playing = true;
                    return;
                }
            }
        }
    }
}

