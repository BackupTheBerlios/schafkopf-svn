#ifndef __RULES_H__
#define __RULES_H__

#include <vector>
#include <exception>

#include "card.h"

class rules
{
    public:
        class invalid_cardlist : public std::exception
        {
            public:
                invalid_cardlist() : std::exception() {}
                const char *what() const throw() 
                {
                    return "rules: cardlist invalid";
                }
        };

        // Supported games
        typedef enum gametype { UNSET_GAME, RUFSPIEL, SOLO, WENZ, GEIER, RAMSCH , DACHS};
        
        /**
         * Create rules for a game
         * \param type Type of game to play
         * \param color Rufspiel: color of SAU to play with
         *              Solo: Color which is trump
         */
        rules(rules::gametype type, card::cardcolor color);
        
        /**
         * Checks if card can be played
         * \param played cards on table
         * \param player_cards cards of player 
         * \param c card to be played
         */
        bool cardValid(const cardlist &played, const cardlist &player_cards, const card &c) const;

        /**
         * Returns index of card which takes the trick
         */
        int trick(const cardlist &cards) const;

        /**
         * Counts values of cards
         */
        int count(const cardlist &cards) const;

        /** Properties */
        rules::gametype type() const { return m_type; }
        card::cardcolor color() const { return m_color; }

    private:
        rules::gametype m_type;
        card::cardcolor m_color;

        bool is_trump(const card &c) const;

        // True, if c2 higher than c1
        bool is_higher(const card &c1, const card &c2) const;
};

#endif
