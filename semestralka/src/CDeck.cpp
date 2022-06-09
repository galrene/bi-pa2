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
shared_ptr<CCard> CDeck::drawCardAt ( size_t i ) {
    if ( i >= m_Content.size() )
        return nullptr;
    shared_ptr<CCard> card = m_Content.at(i);
    m_Content.erase( m_Content.begin() + i );
    return card;
}
shared_ptr<CCard> CDeck::getCardAt ( size_t i ) {
    return m_Content[i];
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

void CDeck::printHand ( ofstream & ofs ) {
    map<string,int> hand;
    for ( const auto & card : m_Content )
        hand[card->getName()]++;
    ofs << "[deck]" << endl;
    for ( const auto & [ def, cnt ] : hand )
        ofs << def << " = " << cnt << endl;
}
void CDeck::printData ( ofstream & ofs ) {
    ofs << "[deck]" << endl;
    for ( const auto & [ def, cnt ] : m_Data )
        ofs << def << " = " << cnt << endl;
}

void CDeck::saveDefinitions ( fs::path & cardDefinitionDir ) {
    CSaver s;
    s.saveCards ( m_Content, cardDefinitionDir ); // saves card definitions
}