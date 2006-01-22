#include <stdlib.h>

#include "cardset.h"

const card cardset::all_cards[] = { 
            card(card::SAU, card::EICHEL),
            card(card::SAU, card::GRAS),
            card(card::SAU, card::HERZ),
            card(card::SAU, card::SCHELLEN),
            card(card::ZEHN, card::EICHEL),
            card(card::ZEHN, card::GRAS),
            card(card::ZEHN, card::HERZ),
            card(card::ZEHN, card::SCHELLEN),
            card(card::KOENIG, card::EICHEL),
            card(card::KOENIG, card::GRAS),
            card(card::KOENIG, card::HERZ),
            card(card::KOENIG, card::SCHELLEN),
            card(card::OBER, card::EICHEL),
            card(card::OBER, card::GRAS),
            card(card::OBER, card::HERZ),
            card(card::OBER, card::SCHELLEN),
            card(card::UNTER, card::EICHEL),
            card(card::UNTER, card::GRAS),
            card(card::UNTER, card::HERZ),
            card(card::UNTER, card::SCHELLEN),
            card(card::NEUN, card::EICHEL),
            card(card::NEUN, card::GRAS),
            card(card::NEUN, card::HERZ),
            card(card::NEUN, card::SCHELLEN),
            card(card::ACHT, card::EICHEL),
            card(card::ACHT, card::GRAS),
            card(card::ACHT, card::HERZ),
            card(card::ACHT, card::SCHELLEN),
            card(card::SIEBEN, card::EICHEL),
            card(card::SIEBEN, card::GRAS),
            card(card::SIEBEN, card::HERZ),
            card(card::SIEBEN, card::SCHELLEN) };
                                
cardset::cardset(cardset::type t)
{
    int i;
    
    cards = new card[(int) t];
    m_size = (int) t; 
    for(i=0;i<t;i++)
        cards[i] = all_cards[i];
}

cardset::~cardset()
{
    delete[] cards;
}

void cardset::permute()
{
    int *indizes;
    int i;
    int a;
    card *permuted;
       
    indizes = new int[size()];
    permuted = new card[size()];
   
    srandom(time(0));
    for(i=0;i<size();i++)
    {
        for(;;)
        {
            indizes[i] = random() % size();
            for(a=0;a<i;a++)
            {
                if( indizes[a] == indizes[i] )
                    continue;
            }
            break;
        }
    }
    for(i=0;i<size();i++)
        permuted[i] = cards[indizes[i]];
    delete[] cards;
    cards = permuted;
}

const card &cardset::operator[]( int index )
{
    if(index < 0 || index >= m_size)
        throw cardset::out_of_range();
    return cards[index];
}

