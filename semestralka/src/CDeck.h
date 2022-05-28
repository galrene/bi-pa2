#pragma once
#include "CCard.h"
#include <map>
#include <memory>
using namespace std;

class CDeck {
  protected:
    map<shared_ptr<CCard>, int> m_Deck;
  public:
    void addCard ( CCard & card );
    void removeCard ( CCard & card );
    void drawCard ( CCard & card );
    /* from file */
    void load ( string path );
    /* to file */
    void save ( string path );
};