#ifndef __GAME_H__
#define __GAME_H__

#include <exception>
#include <string>

#include "rules.h"
#include "card.h"
#include "cardset.h"

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

            bool playing;
            int doubled;
            cardlist cards;
        }

        static const int PLAYERS = 4;

        int socket;
        int initialized_players;
        int round;
        player_info players[PLAYERS];
        rules schafkopf;
        cardset all_cards;

        /**
         * Communication helpers
         */
        void wait_for_request();
        bool process_player_connect(int sock);
        int open_player_connection();
        void broadcast(socket_cmd *cmd);

        /**
         * Game helpers
         */

        /**
         * Reset game specific player variables
         */
        void reset();

        /**
         * Give cards to all players
         * \param num_cards How many cards should every player get?
         * \param cards Where to take cards from
         */
        void give_cards(int num_cards, cardlist &cards);
        
        /**
         * Ask for double
         */
        void ask_double();

        /**
         * Ask for game
         */
        void ask_game();

        /**
         * let player play a card
         */
        void play_card(int player);

        /**
         * Set up playing players.
         */
        void init_playing(int player, rules::gametype t, card::cardcolor c);
};

#endif
