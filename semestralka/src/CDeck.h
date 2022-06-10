#pragma once
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
    CDeck ( const string & name );
    CDeck ( void ) = default;
    /**
     * @brief Add card to deck.
     * 
     * @param card to add
     */
    void addCard ( shared_ptr<CCard> card );
    /**
     * @brief Remove card from deck.
     * 
     * @return shared_ptr<CCard> Removed card
     */
    shared_ptr<CCard> drawCard ( void );
    /**
     * @brief Remove a card at index
     * 
     * @param i card index
     * @return shared_ptr<CCard> removed card
     */
    shared_ptr<CCard> drawCardAt ( size_t i );
    /**
     * @brief Shuffle cards using std::random_shuffle
     */
    void shuffleCards ( void );
    /**
     * @brief Get amount of cards in deck.
     */
    size_t size ( void ) const { return m_Content.size(); }
    /**
     * @brief Call for a card to be rendered in win
     * 
     * @param win where to render
     * @param i card index
     */
    void renderCard ( WINDOW * win, size_t & i ) const;
    /**
     * @brief Save deck inside directory
     * 
     * @param dirName Save directory name
     */
    void saveDefinitions ( fs::path & cardDefinitionDir ) const;
    /**
     * @brief Set the data from which the deck was created
     */
    void setData ( map<string,string> & data ) { m_Data = data; }
    /**
     * @brief Send key value pairs of card and it's amount m_Data into stream
     */
    void printData ( ostream & os );
    /**
     * @brief Send key value pairs of card and it's amount inside m_Content into stream, useful for printing key value pairs of the current hand
     */
    void printHand ( ostream & os );
    /**
     * @brief Get the Name of object
     * 
     * @return string name
     */
    string getName ( void ) const { return m_Name; }
    /**
     * @brief Get card at index
     * 
     * @param i card index
     * @return shared_ptr<CCard> card
     */
    shared_ptr<CCard> getCardAt ( size_t i ) const { return m_Content[i]; }
  protected:
    /**
     * @brief Card content of deck
     */
    deque<shared_ptr<CCard>> m_Content;
    /**
     * @brief Data from which the deck was built, useful for quick dumping deck definitions inside a file and building a deck from data as key value pairs.
     */
    map<string,string> m_Data;
    string m_Name;
};