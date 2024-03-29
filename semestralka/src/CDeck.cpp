#include "CDeck.h"

CDeck::CDeck ( const string & name )
: m_Name ( name ) {}

void CDeck::addCard ( shared_ptr<CCard> card ) {
    m_Content.push_back ( card );
}
shared_ptr<CCard> CDeck::drawCard ( void ) {
    shared_ptr<CCard> card = m_Content.front();
    m_Content.pop_front();
    return card;
}
shared_ptr<CCard> CDeck::drawCardAt ( size_t i ) {
    if ( i >= m_Content.size() )
        return nullptr;
    shared_ptr<CCard> card = m_Content.at(i);
    m_Content.erase( m_Content.begin() + i );
    return card;
}
void CDeck::shuffleCards ( void ) {
    random_shuffle ( m_Content.begin(), m_Content.end() );
}
void CDeck::renderCard ( WINDOW * win, size_t & i ) const {
    m_Content[i]->render ( win );
}
void CDeck::printHand ( ostream & os ) {
    map<string,int> hand;
    for ( const auto & card : m_Content )
        hand[card->getName()]++;
    os << "[deck]" << endl;
    for ( const auto & [ def, cnt ] : hand )
        os << def << " = " << cnt << endl;
}
void CDeck::printData ( ostream & os ) {
    os << "[deck]" << endl;
    for ( const auto & [ def, cnt ] : m_Data )
        os << def << " = " << cnt << endl;
}
void CDeck::saveDefinitions ( fs::path & cardDefinitionDir ) const {
    CSaver s;
    s.saveCards ( m_CardDefinitons, cardDefinitionDir );
}