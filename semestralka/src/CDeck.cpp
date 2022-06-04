#include "CDeck.h"

void CDeck::addCard ( shared_ptr<CCard> & card ) {
    m_Content.push_back ( card );
}
shared_ptr<CCard> CDeck::drawCard ( void ) {
    shared_ptr<CCard> card = m_Content.front();
    m_Content.pop_front();
    return card;
}

void CDeck::shuffleCards ( void ) {
    random_shuffle ( m_Content.begin(), m_Content.end() );
}