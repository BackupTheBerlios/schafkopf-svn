
#include "card.h"

card::card(card::cardtype type, card::cardcolor color)
{
    card::type(type);    
    card::color(color);
}

card::card()
{
    m_type = card::NOTYPE;
    m_color = card::NOCOLOR;
}

void card::type(card::cardtype type)
{
    switch(type)
    {
        case SAU:
        case ZEHN:
        case KOENIG:
        case OBER:
        case UNTER:
        case NEUN:
        case ACHT:
        case SIEBEN: 
            break;
        default:
            throw card::invalid_type();
    }
    m_type = type;
}

void card::color(card::cardcolor color)
{
    if(color < card::EICHEL || color > card::SCHELLEN)
        throw card::invalid_color();
    m_color = color;
}

bool card::operator==(const card &src)
{
    return ( type() == src.type() && color() == src.color() );
}


