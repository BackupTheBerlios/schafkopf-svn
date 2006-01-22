#ifndef __CARD_H__
#define __CARD_H__

#include <exception>
#include <vector>

class card;

typedef std::vector<card> cardlist;

class card
{
    public:
        class invalid_color : public std::exception
        {
            public:
                invalid_color() : std::exception() {}
                const char *what() const throw() 
                {
                    return "card: invalid color";
                }
        };
        class invalid_type: public std::exception
        {
            public:
                invalid_type() : std::exception() {}
                const char *what() const throw() 
                {
                    return "card: invalid type";
                }
        };
        
        typedef enum cardtype { NOTYPE = -1, SAU = 0, ZEHN = 1, KOENIG = 2, OBER = 3, UNTER = 4, NEUN = 5, ACHT = 6, SIEBEN = 7 };
        typedef enum cardcolor { NOCOLOR = -1, EICHEL = 0, GRAS = 1, HERZ = 2, SCHELLEN = 3 };

        
        card(cardtype type, cardcolor color);
        card();
        
        cardtype type() const { return m_type; }
        void type(cardtype type);

        cardcolor color() const { return m_color; }
        void color(cardcolor color);

        int value() const { return m_value; }

        bool operator==(const card &src);

    private:
        cardtype m_type;
        cardcolor m_color;
        int m_value;
};

#endif
