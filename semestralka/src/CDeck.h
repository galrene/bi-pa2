#pragma once
#ifndef CDeck_H
#define CDeck_H
#include <deque>
#include <memory>
#include <algorithm>
#include <string>
#include <ncurses.h>

#include "CCard.h"
#include "CSaver.h"
using namespace std;

class CDeck {
  public:
    CDeck ( string name );
    CDeck ( void ) = default;
    void addCard ( shared_ptr<CCard> card );
    shared_ptr<CCard> drawCard ( void );
    shared_ptr<CCard> getCardAt ( size_t i );
    shared_ptr<CCard> drawCardAt ( size_t i );
    string getName ( void );
    void shuffleCards ( void );
    size_t size ( void );
    void renderCard ( WINDOW * win, size_t & i );
    
    // void load ( string path );
    /**
     * @brief Save deck inside directory
     * 
     * @param dirName Save directory name
     */
    void saveDefinitions ( fs::path & cardDefinitionDir );
    void setData ( map<string,string> & data ) { m_Data = data; }
    /**
     * @brief Send raw data from m_Data into stream
     */
    void printData ( ostream & os );
    /**
     * @brief Send raw data create from m_Content into stream
     */
    void printHand ( ostream & os );

  protected:
    deque<shared_ptr<CCard>> m_Content;
    map<string,string> m_Data;
    string m_Name;
};
#endif