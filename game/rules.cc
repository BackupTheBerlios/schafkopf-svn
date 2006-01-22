
#include "rules.h"

rules::rules(rules::gametype type, card::cardcolor color)
{
    m_type = type;
    m_color = color;
}

bool rules::cardValid(const cardlist &played, const cardlist &player_cards, const card &c) const
{
    const card first;
    cardlist::const_iterator it;

    // no card played
    if(!played.size())
        return true;
    
    first = played[0];
    
    // First card and c are trumps
    if( is_trump(first) && is_trump(c) )
        return true;

    // First card an c have same color
    if( first.color() == c.color() )
    {
        // Searching SAU?
        if( first.color() == color() && 
            type() == rules::RUFSPIEL && 
            c.type() != card::SAU )
            return false;
        return true;
    }

    // First is trump and player doesn't have a trump
    if( is_trump(first) )
    {
        for( it=player_cards.begin(); it!=player_cards.end(); it++ )
        {
            if( is_trump( *it ) )
                return false;
        }
    }

    // First is color and player doesn't have the color
    for( it=player_cards.begin(); it!=player_cards.end(); it++)
    {
        if( first.color() == it->color() )
            return false;
    }
    return true;
}

int rules::trick(const cardlist &cards) const
{
    int highest;
    int i;
    
    if( !cards.size() )
        throw rules::invalid_cardlist();
    highest = 0;
    for( i=1; i<cards.size(); i++ )
    {
        if( !is_trump( cards[0] ) && cards[0].color() != cards[i].color() )
            continue;
        if( is_higher( cards[highest], cards[i] ) )
            highest = i;
    }
    return highest;
}
        
int rules::count(const cardlist &cards) const
{
    cardlist::const_iterator it;
    int vals;

    for( vals=0, it=cards.begin(); it!=cards.end(); it++)
        vals += it->value();
    return vals;
}

bool rules::is_trump(const card &c) const
{
    switch( type() )
    {
        case RUFSPIEL:
        case RAMSCH:
            if( c->color() == card::HERZ )
                return true;
            else if( c->type() == card::OBER || c->type == card::UNTER )
                return true;
            return false;
            
        case SOLO:
            if( c->color() == color() )
                return true;
            else if( c->type() == card::OBER || c->type == card::UNTER )
                return true;
            return false;

        case WENZ:
            if( c->color() == color() )
                return true;
            else if( c->type == card::UNTER )
                return true;
            return false;

        case GEIER:
            if( c->color() == color() )
                return true;
            else if( c->type == card::OBER )
                return true;
            return false;

        case DACHS:
            if( c->type == card::SAU || c->type == card::ZEHN )
                return true;
            return false;
    }
}
            
bool rules::is_higher(const card &c1, const card &c2) const
{
    if( !is_trump(c1) && is_trump(c2) )
        return true;
    else if( is_trump(c1) && is_trump(c2) )
    {
        if(c1->type() == c2->type())
            return c2->color() < c1->color();
        else
            return c2->type() < c1->type(); 
    }
    else if( c1->color() != c2->color() )
        return false;
    else
        return c2->type < c1->type;
}

