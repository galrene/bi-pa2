#include "CDeck.h"

CDeck::CDeck ( string name )
: m_Name ( name ) {}

string CDeck::getName ( void ) { return m_Name; }

void CDeck::addCard ( const shared_ptr<CCard> & card ) {
    m_Content.push_back ( card );
}
shared_ptr<CCard> & CDeck::drawCard ( void ) {
    shared_ptr<CCard> & card = m_Content.front();
    m_Content.pop_front();
    return card;
}

void CDeck::shuffleCards ( void ) {
    random_shuffle ( m_Content.begin(), m_Content.end() );
}

size_t CDeck::size ( void ) {
    return m_Content.size();
}

void CDeck::renderCard ( WINDOW * win, size_t & i ) {
    m_Content[i]->render ( win );
}