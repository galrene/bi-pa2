#pragma once
#include <deque>
#include <memory>
#include <algorithm>

#include "CCard.h"
using namespace std;

class CDeck {
  protected:
    deque<shared_ptr<CCard>> m_Content;
    string m_Name;
  public:
    CDeck ( string name );
    CDeck ( void ) {}

    string getName ( void );
    void addCard ( shared_ptr<CCard> & card );
    void shuffleCards ( void );
    shared_ptr<CCard> drawCard ( void );
    /* from file */
    void load ( string path );
    /* to file */
    void save ( string path );
};
