#ifndef __GAME_H__
#define __GAME_H__

#include <exception>
#include <string>

/**
 * Class for the game.
 * 
 * Connection sequence:
 *  - 4 Players must connect
 *  - 1 Controller must connect
 *  
 * Controller:
 *  - Sends quit messages
 *
 * Players:
 *  - Sends one connect message
 *  - Sends replies
 * 
 * Game:
 *  - Sends request to players
 *  - Sends game status changes to players
 *  - Processes controller messages
 */

class game
{
    public:
        class not_initialized
            : public std::exception
        {
            public:
                not_initialized() : exception() {}
                const char *what() const throw() 
                {
                    return "game was not initialized.";
                }
        };

        class connection_failed
            : public std::exception
        {
            public:
                connection_failed() : exception() {}
                const char *what() const throw()
                {
                    return "game: connection failed";
                }
        } 


        /** 
         * Creates a game. All communication is done over a socket
         */
        game(int socket);

        /** 
         * Waits for connections
         */
        void connect();

        /**
         * Play one game with players currently connected
         */
        void play();

    private:
        struct player_info
        {
            int socket;
            std::string name;
        }
        static const int PLAYERS = 4;

        int socket;
        int initialized_players;
        player_info players[PLAYERS];

        void wait_for_request();
        bool process_player_connect(int sock);
        int open_player_connection();
};

#endif
