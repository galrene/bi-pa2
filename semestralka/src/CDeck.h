#pragma once
#ifndef CDeck_H
#define CDeck_H
#include <deque>
#include <memory>
#include <algorithm>
#include <string>
#include <ncurses.h>

#include "CCard.h"
using namespace std;

class CDeck {
  public:
    CDeck ( string name );
    CDeck ( void ) {}
    void addCard ( const shared_ptr<CCard> & card );
    shared_ptr<CCard> & drawCard ( void );
    shared_ptr<CCard> getCardAt ( size_t i );
    shared_ptr<CCard> drawCardAt ( size_t i );
    string getName ( void );
    void shuffleCards ( void );
    size_t size ( void );
    void renderCard ( WINDOW * win, size_t & i );
    
    // void load ( string path );
    // void save ( string path );
  protected:
    deque<shared_ptr<CCard>> m_Content;
    string m_Name;
};
#endif