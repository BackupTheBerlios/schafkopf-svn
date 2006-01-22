#ifndef __CARDSET_H__
#define __CARDSET_H__

#include <vector>
#include <exception>

#include <card.h>


class cardset
{
    public:
        class out_of_range : public std::exception
        {
            public:
                out_of_range() : std::exception() {}
                const char *what() const throw() 
                {
                    return "cardset: index out of range";
                }

        };
        
        /**
         * Type of cardset: normal or shord cards
         */
        typedef enum type { NORMAL=32, SHORT=20 };

        static const card cardset::all_cards[];


        cardset(type t = NORMAL);
        ~cardset();

        /** 
         * Permute cards
         */
        void permute();

        /**
         * How many cards are in cardset: 32 if NORMAL, 20 if SHORT
         */
        int size() const { return m_size; }

        /**
         * get gard
         */
        const card &operator[]( int index );

    private:
        card *cards;
        int m_size;
};

#endif
