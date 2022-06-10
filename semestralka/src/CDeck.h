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
    /**
     * @brief Add card to m_Content
     * 
     * @param card to add
     */
    void addCard ( shared_ptr<CCard> card );
    /**
     * @brief Remove card from m_Content
     * 
     * @return shared_ptr<CCard> Removed card
     */
    shared_ptr<CCard> drawCard ( void );
    /**
     * @brief m_Content getter
     * 
     * @param i card index
     * @return shared_ptr<CCard> card
     */
    shared_ptr<CCard> getCardAt ( size_t i );
    /**
     * @brief Remove a card at index
     * 
     * @param i card index
     * @return shared_ptr<CCard> removed card
     */
    shared_ptr<CCard> drawCardAt ( size_t i );
    /**
     * @brief Get the Name object
     * 
     * @return string 
     */
    string getName ( void );
    /**
     * @brief Shuffle cards using std::random_shuffle
     */
    void shuffleCards ( void );
    /**
     * @brief Get m_Content size
     */
    size_t size ( void );
    /**
     * @brief Call for a card to be rendered in win
     * 
     * @param win where to render
     * @param i card index
     */
    void renderCard ( WINDOW * win, size_t & i );
    /**
     * @brief Save deck inside directory
     * 
     * @param dirName Save directory name
     */
    void saveDefinitions ( fs::path & cardDefinitionDir );
    /**
     * @brief Set the data from which the deck was created
     */
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