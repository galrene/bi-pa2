#pragma once
#include <map>
#include <memory>
#include "CCard.h"
using namespace std;

class CDeck {
  protected:
    // map<shared_ptr<CCard>, int> m_Deck;
  public:
    void addCard ( shared_ptr<CCard> & card );
    void removeCard ( shared_ptr<CCard> & card );
    void drawCard ( shared_ptr<CCard> & card );
    /* from file */
    void load ( string path );
    /* to file */
    void save ( string path );
};
