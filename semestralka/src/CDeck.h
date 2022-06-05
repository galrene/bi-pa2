#pragma once
#include <deque>
#include <memory>
#include <algorithm>

#include "CCard.h"
using namespace std;

class CDeck {
  public:
    CDeck ( string name );
    CDeck ( void ) {}

    void addCard ( shared_ptr<CCard> & card );
    shared_ptr<CCard> drawCard ( void );
    string getName ( void );
    void shuffleCards ( void );
    size_t size ( void );

    // void load ( string path );
    // void save ( string path );
  protected:
    deque<shared_ptr<CCard>> m_Content;
    string m_Name;
};
